import csv
import sys
import os
import configargparse

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
    line += expand("\"type\": \"" + t + "\",", 1)
    line += expand("\"value\": {", 1)
    line += expand("\"" + m + "\" : [", 2)

    write(output, line)

def close_file(output):
    line = expand("]", 2)
    line += expand("}", 1)
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

    def member(self, member, value, last=False):
        line = expand("\"{}\": {}".format(member,value), 4, last)
        if last is False:
            line += ","
        line += "\n"
        return line

    def multiple_index(self, member, value):
        index_range = value.split("-")
        start = int(index_range[0], 16)
        end = int(index_range[1], 16)
        print("Start {} End {}".format(start, end))

        out = ""
        for x in range(start, end):
            if member == "index":
                o = ObjectEntry(hex(x), self.sub, self.name, self.code,
                    self.type, self.category, self.description, self.pdo,
                    self.access, self.default)
                out += o.dump()
            elif member == "subindex":
                o = ObjectEntry(self.index, hex(x), self.name, self.code,
                    self.type, self.category, self.description, self.pdo,
                    self.access, self.default)
                out += o.dump()

            if x < end:
                out += ",\n"

        return out

    def _index(self, member, value, last=False):
        val_str = str(value)
        if "-" in val_str:
            return self.multiple_index(member, val_str)
        else:
            out = int(value, 16)
            return self.member(member, out, last)

    def _string(self, member, value, last=False):
        out = "\"{}\"".format(value)
        return self.member(member, out, last)

    def _pdo(self, member, value, last=False):
        if value.lower() == "optional" or value.lower() == "yes":
            out = "true"
        else:
            out = "false"
        return self.member(member, out, last)

    def dump(self):
        entry = expand("{", 3)
        entry += self._index("index", self.index)
        entry += self._index("subindex", self.sub)
        entry += self._string("name", self.name)
        entry += self._string("code", self.code)
        entry += self._string("type", self.type)
        entry += self._string("category", self.category)
        entry += self._string("description", self.description)
        entry += self._pdo("pdo", self.pdo)
        entry += self._string("access", self.access)
        entry += self._string("default", self.default, True)
        entry += expand("}", 3, False)

        return entry

if __name__ == "__main__":
    main(sys.argv)
