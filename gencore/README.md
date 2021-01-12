# gencore

rtos core dump generator.


## Dependency

### ubuntu

```bash
sudo apt install libelf-dev
sudo apt install zlib1g-dev
```

## Build

```bash
cd gencore
mkdir build
cd build
cmake ..
make -j
```

## Example

1. Convert log to core file.

```bash
./build/gencore samples/log.txt -o core

```

2. Load core with gdb.

```bash
arm-linux-gnueabi-gdb [exe.elf] core
```

## Log format
[sample log](samples/log.txt)
