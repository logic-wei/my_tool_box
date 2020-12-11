# binparse

Usefull tools help you analyze binary file.

## Quick start

If you got a binary file,you can create a file to describe a range of bytes in file.And then the `binparse` can show you human readable infomation rather than the fucking bytes.

For example elf file,you can create a file named `Elf32_Ehdr.json` to describe the elf header of this file.

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

And then parse the file test.o at offset 0:

```bash
binparse.py -o 0 -s structs/Elf32_Ehdr.json test.o
```

The more readable infomation than raw bytes you could get:

```bash
Elf32_Ehdr at 0 in test.o
├─0x0 0x0: e_ident 7f454c46010101000000000000000000 .ELF............
├─0x10 0x20: e_type 01002800 ..(.
├─0x14 0x28: e_machine 01000000 ....
├─0x18 0x30: e_version 00000000 ....
├─0x1c 0x38: e_entry 00000000 ....
├─0x20 0x40: e_phoff 70020000 p...
├─0x24 0x48: e_shoff 00000005 ....
├─0x28 0x50: e_flags 34000000 4...
├─0x2c 0x58: e_ehsize 00002800 ..(.
├─0x30 0x60: e_phentsize 0b000100 ....
├─0x34 0x68: e_phnum 00482de9 .H-.
├─0x38 0x70: e_shentsize 0db0a0e1 ....
├─0x3c 0x78: e_shnum 08009fe5 ....
└─0x40 0x80: e_shstrndx 00008fe0 ....
└─0x40 0x80: e_shstrndx 00008fe0 ....
```