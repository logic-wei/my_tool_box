#!/usr/bin/python3
import argparse as ap
import json
import struct


def bytes2readable(bytes):
    ret = ""
    for byte in bytes:
        if 32 <= byte <= 126:
            ret += chr(byte)
        else:
            ret += "."
    return ret


def bytes_unpack(bytes, type, size, endian):
    ret = ""
    fmt = "<"
    if endian == "big":
        fmt = ">"

    if type == "int8":
        fmt += "b"
    elif type == "int16":
        fmt += "h"
    elif type == "int32":
        fmt += "i"
    elif type == "int64":
        fmt += "l"
    elif type == "uint8":
        fmt += "B"
    elif type == "uint16":
        fmt += "H"
    elif type == "uint32":
        fmt += "I"
    elif type == "uint64":
        fmt += "L"
    elif type == "ascii":
        fmt += "" + str(size) + "s"
    else:
        return "<error>"

    if type == "ascii":
        data = struct.unpack(fmt, bytes)
        return data[0].decode("ascii", size)
    else:
        data = struct.unpack(fmt, bytes)
        return str(data[0])


def parse(binpath, offset, structpath, endian):
    pos = 0
    offset_in_struct = 0
    with open(structpath, "r") as structfile:
        struct = json.loads(structfile.read())
        print(struct["name"] + " at " + hex(offset) + " in " + binpath)

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

                if "type" in field:
                    readable = "==> " + bytes_unpack(data, field["type"], size, endian)
                else:
                    readable = "=> " + bytes2readable(data)
                print(prefix + hex(pos) + " " + hex(offset_in_struct) + ": " + name + ": " + data.hex() + " " + readable)

                pos += size
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
                           required=True,
                           help="specify a json file describe the struct.")
    argparser.add_argument("-e", "--endian",
                           choices=("big", "little"),
                           default="little",
                           help="specify endian to decode.")

    args = argparser.parse_args()
    parse(args.path, int(args.offset), args.struct, args.endian)


main()
