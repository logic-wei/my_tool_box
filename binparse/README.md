# binparse

Highly customizable tools help you analyze binary file.

## Quick start

If you got a binary file,you can create a file to describe a range of bytes in file.And then the `binparse` can show you human readable infomation rather than the fucking bytes.

For example elf file.

As we known the elf header looks like:

```c
typedef struct {
　　unsigned char e_ident[EI_NIDENT];
　　Elf32_Half e_type;
　　Elf32_Half e_machine;
　　Elf32_Word e_version;
　　Elf32_Addr e_entry;
　　Elf32_Off e_phoff;
　　Elf32_Off e_shoff;
　　Elf32_Word e_flags;
　　Elf32_Half e_ehsize;
　　Elf32_Half e_phentsize;
　　Elf32_Half e_phnum;
　　Elf32_Half e_shentsize;
　　Elf32_Half e_shnum;
　　Elf32_Half e_shstrndx;
} Elf32_Ehdr;
```

You can create a file named `Elf32_Ehdr.json` to descibe it:

Elf32_Ehdr.json:

```json
{
    "name": "Elf32_Ehdr",
    "fields": [
        {
            "name": "e_ident",
            "size": 16
        },
        {
            "name": "e_type",
            "size": 4
        },
        {
            "name": "e_machine",
            "size": 4
        },
        {
            "name": "e_version",
            "size": 4
        },
        {
            "name": "e_entry",
            "size": 4
        },
        {
            "name": "e_phoff",
            "size": 4
        },
        {
            "name": "e_shoff",
            "size": 4
        },
        {
            "name": "e_flags",
            "size": 4
        },
        {
            "name": "e_ehsize",
            "size": 4
        },
        {
            "name": "e_phentsize",
            "size": 4
        },
        {
            "name": "e_phnum",
            "size": 4
        },
        {
            "name": "e_shentsize",
            "size": 4
        },
        {
            "name": "e_shnum",
            "size": 4
        },
        {
            "name": "e_shstrndx",
            "size": 4
        }
    ]
}

```

And then parse the elf file test.o at offset 0:

```bash
binparse.py -o 0 -s structs/Elf32_Ehdr.json test.o
```

The more readable infomation than raw bytes you could get:

```bash
Elf32_Ehdr at 0 in test.o
├─0x0 0x0: e_ident 7f454c46010101000000000000000000 .ELF............
├─0x10 0x10: e_type 01002800 ..(.
├─0x14 0x14: e_machine 01000000 ....
├─0x18 0x18: e_version 00000000 ....
├─0x1c 0x1c: e_entry 00000000 ....
├─0x20 0x20: e_phoff 04020000 ....
├─0x24 0x24: e_shoff 00000005 ....
├─0x28 0x28: e_flags 34000000 4...
├─0x2c 0x2c: e_ehsize 00002800 ..(.
├─0x30 0x30: e_phentsize 09000100 ....
├─0x34 0x34: e_phnum 0cd04de2 ..M.
├─0x38 0x38: e_shentsize 08009de5 ....
├─0x3c 0x3c: e_shnum 04109de5 ....
└─0x40 0x40: e_shstrndx 010080e0 ....
```

If you want to get more readable information,add a field `type` in json:

```json
{
    "name": "Elf32_Ehdr",
    "fields": [
        {
            "name": "e_ident",
            "size": 16
        },
        {
            "name": "e_type",
            "size": 4
        },
        {
            "name": "e_machine",
            "size": 4
        },
        {
            "name": "e_version",
            "size": 4
        },
        {
            "name": "e_entry",
            "size": 4
        },
        {
            "name": "e_phoff",
            "size": 4,
            "type": "uint32"
        },
        {
            "name": "e_shoff",
            "size": 4,
            "type": "uint32"
        },
        {
            "name": "e_flags",
            "size": 4
        },
        {
            "name": "e_ehsize",
            "size": 4,
            "type": "uint32"
        },
        {
            "name": "e_phentsize",
            "size": 4,
            "type": "uint32"
        },
        {
            "name": "e_phnum",
            "size": 4,
            "type": "uint32"
        },
        {
            "name": "e_shentsize",
            "size": 4,
            "type": "uint32"
        },
        {
            "name": "e_shnum",
            "size": 4,
            "type": "uint32"
        },
        {
            "name": "e_shstrndx",
            "size": 4,
            "type": "uint32"
        }
    ]
}
```

And then the value will be parsed whos `type` is specified(add option `-e` to specify the endian.):

```json
binparse.py -o 0 -s ~/.binparse/structs/Elf32_Ehdr.json -e little core
Elf32_Ehdr at 0x0 in core
├─0x0 0x0: e_ident: 7f454c46020101000000000000000000 => .ELF............
├─0x10 0x10: e_type: 04003e00 => ..>.
├─0x14 0x14: e_machine: 01000000 => ....
├─0x18 0x18: e_version: 00000000 => ....
├─0x1c 0x1c: e_entry: 00000000 => ....
├─0x20 0x20: e_phoff: 40000000 ==> 64
├─0x24 0x24: e_shoff: 00000000 ==> 0
├─0x28 0x28: e_flags: 00000000 => ....
├─0x2c 0x2c: e_ehsize: 00000000 ==> 0
├─0x30 0x30: e_phentsize: 00000000 ==> 0
├─0x34 0x34: e_phnum: 40003800 ==> 3670080
├─0x38 0x38: e_shentsize: 18000000 ==> 24
├─0x3c 0x3c: e_shnum: 00000000 ==> 0
└─0x40 0x40: e_shstrndx: 04000000 ==> 4
```

- The `=>` means convert to ascii if possible.
- The `==>` means convert to type user specified in json.

Here are the supported types:
- "int8"
- "int16"
- "int32"
- "int64"
- "uint8"
- "uint16"
- "uint32"
- "uint64"
- "ascii"
