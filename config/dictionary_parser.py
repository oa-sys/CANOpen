import csv
import sys
import os
import configargparse

MOUNT_TYPE = "driver/mnt/canopen/Mount"
META_TYPE = "driver/mnt/canopen/Meta"
ENTRY_TYPE = "driver/mnt/canopen/Entry"

argparse = configargparse.ArgParser()
argparse.add('-i', '--input', type=str, required=True,
    help="Input - Dictionary config file. Example:\n-i sdo_dictionary.csv")
argparse.add('-o', '--output', type=str, required=True,
    help="Output - JSON config output Example:\n-o sdo.json")
argparse.add('--id', type=str, required=True,
    help="Object Identifier - Object identifier Example:\n-o config/can/master")
argparse.add('-t', '--type', type=str, required=True,
    help="Object Type. Example:\n-t driver/mnt/canopen/Mount")
argparse.add('-m', '--member', type=str, required=True,
    help="Object Member. Example:\n-m deviceInfo")

def expand(string, cnt, newline=True):
    line = ""
    for i in range(0, cnt):
        line += "\t"
    line += string
    if newline is True:
        line += "\n"
    line = line.expandtabs(4)

    return line

def write(file, line):
    out = open(file , "a")
    out.write(line)
    out.close()

def init_file(output, i, t, m):
    os.remove(output)

    line = "{\n"
    line += expand("\"id\": \"" + i + "\",", 1)
    line += expand("\"type\": \"" + MOUNT_TYPE + "\",", 1)
    line += expand("\"scope\": [", 1)

    write(output, line)

def close_file(output):
    line = expand("]", 1)
    line += "}"

    write(output, line)

def generate(config, output):
    csv_file = open(config, "rt")
    header = csv_file.readline()
    csv_file.seek(len(header))
    headers = header.split(",")
    headers = [s.strip(' ') for s in headers]
    headers = [s.strip('\n') for s in headers]

    entry_str = ""
    reader = csv.DictReader(csv_file, fieldnames=headers)

    cnt = len(list(reader))
    csv_file.seek(0)
    csv_file.readline()
    cnt -= 1;
    for row in reader:
        entry = ObjectEntry(row['Index'], row['SubIndex'], row['Name'],
                  row['ObjectCode'], row['DataType'], row['Category'],
                  row['Description'], row['PDO'], row['Access'], row['Default'])

        entry_str += entry.dump()
        if cnt > 0:
            entry_str += ","
            cnt = cnt - 1;
        entry_str += "\n"

    write(output, entry_str)

def main(argv):
    options = argparse.parse_args()

    config_csv = options.input
    config_json = options.output
    id = options.id
    type = options.type
    member = options.member

    config = open(config_csv, "r")
    config_lines = config.readlines()
    config.close()

    init_file(config_json, id, type, member)
    generate(config_csv, config_json)
    close_file(config_json)

class ObjectEntry:
    def __init__(self, index, sub, name, code, type, category,
                 description, pdo, access, default):
        self.index = index
        self.sub = sub
        self.name = name
        self.code = code
        self.type = type
        self.category = category
        self.description = description
        self.pdo = pdo
        self.access = access
        self.default = default

    def member(self, member, value, last=False, indent=6):
        line = expand("\"{}\": {}".format(member,value), indent, last)
        if last is False:
            line += ","
            line += "\n"
        return line

    def multiple_index(self, member, value):
        index_range = value.split("-")
        start = int(index_range[0], 16)
        end = int(index_range[1], 16)

        out = ""
        for x in range(start, end):
            name = "{}_{}".format(self.name, x)
            if member == "index":
                o = ObjectEntry(hex(x), self.sub, name, self.code,
                    self.type, self.category, self.description, self.pdo,
                    self.access, self.default)
                out += o.dump()
            elif member == "subindex":
                o = ObjectEntry(self.index, hex(x), name, self.code,
                    self.type, self.category, self.description, self.pdo,
                    self.access, self.default)
                out += o.dump()

            if x < end-1:
                out += ",\n"

        return out

    def _index(self, member, value, last=False):
        out = int(value, 16)
        return self.member(member, out, last)

    def _pdo(self, member, value, last=False):
        if value.lower() == "optional" or value.lower() == "yes":
            out = "true"
        else:
            out = "false"
        return self.member(member, out, last)

    def _string(self, member, value, last=False, indent=6):
        out = "\"{}\"".format(value)
        return self.member(member, out, last, indent)

    def _type(self, type_str):
        """ Convert to Corto type """
        t = "NULL"
        v = self.default
        if type_str == "BOOLEAN":
            t = "bool"
            v = self.default.lower()
        elif type_str == "REAL32":
            t = "float32"
            if not v:
                v = "0"
        elif type_str == "UNSIGNED8":
            t = "uint8"
            if not v:
                v = "0"
        elif type_str == "UNSIGNED16":
            t = "uint16"
            if not v:
                v = "0"
        elif type_str == "UNSIGNED32":
            t = "uint32"
            if not v:
                v = "0"
        elif type_str == "INTEGER8":
            t = "int8"
            if not v:
                v = "0"
        elif type_str == "INTEGER16":
            t = "int16"
            if not v:
                v = "0"
        elif type_str == "INTEGER32":
            t = "int32"
            if not v:
                v = "0"
        elif type_str == "STRING_VISIBLE":
            t = "string"
            v = "\"{}\"".format(self.default)
        elif type_str == "STRING_OCTET":
            t = "octet"
            v = "\"{}\"".format(self.default)
        elif type_str == "STRING_UNICODE":
            t = "string"
            v = "\"{}\"".format(self.default)
        elif type_str == "TIME_OF_DAY":
            t = "string"
            v = "\"{}\"".format(self.default)
        elif type_str == "TIME_DIFFERENCE":
            t = "string"
            v = "\"{}\"".format(self.default)
        elif type_str == "DOMAIN":
            t = "word"
            v = "0"

        type_escape = "\"{}\"".format(t)
        out = self.member("type", type_escape)

        out += self.member("value", v, True)
        return out

    def dump(self):
        entry = ""
        if "-" in self.index:
            entry += self.multiple_index("index", self.index)
        elif "-" in self.sub:
            entry += self.multiple_index("subindex", self.sub)
        else:
            entry = expand("{", 2)
            entry += self._string("id", self.name, False, 3)
            entry += self._string("type", ENTRY_TYPE, False, 3)
            entry += expand("\"value\":", 3)
            entry += expand("{", 3)
            entry += expand("\"v\":", 4)
            entry += expand("{", 4)
            entry += self._type(self.type)
            entry += expand("},", 4)
            entry += expand("\"meta\": ",4)
            entry += expand("{", 4)
            entry += self._string("type", META_TYPE, False, 5)
            entry += expand("\"value\":", 5)
            entry += expand("{", 5)
            entry += self._index("index", self.index)
            entry += self._index("subindex", self.sub)
            entry += self._string("code", self.code)
            entry += self._string("dataType", "TYPE_"+self.type)
            entry += self._string("category", self.category)
            entry += self._string("description", self.description)
            entry += self._pdo("pdo", self.pdo)
            entry += self._string("access", self.access, True)
            entry += expand("}", 5)
            entry += expand("}", 4)
            entry += expand("}", 3)
            entry += expand("}", 2, False)

        return entry

if __name__ == "__main__":
    main(sys.argv)
