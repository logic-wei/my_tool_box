#!/usr/bin/python3
import argparse as ap
import json


def bytes2readable(bytes):
    ret = ""
    for byte in bytes:
        if 32 <= byte <= 126:
            ret += chr(byte)
        else:
            ret += "."
    return ret

def parse(binpath, offset, structpath):
    pos = 0
    offset_in_struct = 0
    with open(structpath, "r") as structfile:
        struct = json.loads(structfile.read())
        print(struct["name"] + " at " + str(offset) + " in " + binpath)

        with open(binpath, "rb") as binfile:
            binfile.seek(offset)
            pos += offset
            i = 0
            for field in struct["fields"]:
                name = field["name"]
                size = field["size"]
                data = binfile.read(size)
                prefix = "├─"
                if i == len(struct["fields"]) - 1:
                    prefix = "└─"
                print(prefix + hex(pos) + " " + hex(offset_in_struct) + ": " + name + " " + data.hex() + " " + bytes2readable(data))
                pos += size
                offset_in_struct += size
                offset_in_struct += size
                i += 1


def main():
    argparser = ap.ArgumentParser()

    argparser.add_argument("path",
                           help="binary file to parse.")
    argparser.add_argument("-o", "--offset",
                           default=0,
                           help="struct offset in file.")
    argparser.add_argument("-s", "--struct",
                           help="specify a json file describe the struct.")

    args = argparser.parse_args()
    parse(args.path, int(args.offset), args.struct)


main()
