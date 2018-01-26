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

    print("Output \n{}".format(line))
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
        line = expand("\"" + member + "\": \"" + value + "\"", 4, False)
        if last is False:
            line += ","
        line += "\n"
        return line

    def dump(self):
        entry = expand("{", 3)
        entry += self.member("index", self.index)
        entry += self.member("subindex", self.sub)
        entry += self.member("name", self.name)
        entry += self.member("code", self.code)
        entry += self.member("type", self.type)
        entry += self.member("category", self.category)
        entry += self.member("description", self.description)
        entry += self.member("pdo", self.pdo)
        entry += self.member("access", self.access)
        entry += self.member("default", self.default, True)
        entry += expand("}", 3, False)

        return entry

if __name__ == "__main__":
    main(sys.argv)
