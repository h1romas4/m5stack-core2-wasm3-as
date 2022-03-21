# m5stack-core2-wasm3-as

M5Stack Core2 With Wasm3/AssemblyScript Demo.

TODO: Memory allocation needs to be improved so that PSRAM(slow) is not used.

![Main Board](https://raw.githubusercontent.com/h1romas4/m5stack-core2-wasm3-as/main/docs/images/m5stack-core2-01.jpg)

See also: RISC-V version

[m5stamp-c3dev](https://github.com/h1romas4/m5stamp-c3dev)

## Build

### Require

- [Setup ESF-IDF v4.4](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32c3/get-started/index.html#installation-step-by-step)

get_idf

```
alias get_idf='. $HOME/esp/esp-idf/export.sh'
```

```
$ get_idf
Detecting the Python interpreter
... snip ...
Done! You can now compile ESP-IDF projects.
Go to the project directory and run:

  idf.py build

$ echo ${IDF_PATH}
/home/hiromasa/devel/toolchain/esp/esp-idf

$ xtensa-esp32-elf-gcc -v
Using built-in specs.
COLLECT_GCC=xtensa-esp32-elf-gcc
COLLECT_LTO_WRAPPER=/home/hiromasa/.espressif/tools/xtensa-esp32-elf/esp-2021r2-8.4.0/xtensa-esp32-elf/bin/../libexec/gcc/xtensa-esp32-elf/8.4.0/lto-wrapper
Target: xtensa-esp32-elf
... snip ...
Thread model: posix
gcc version 8.4.0 (crosstool-NG esp-2021r2)
```

### Build and Execute

git clone and build

```
git clone --recursive https://github.com/h1romas4/m5stack-core2-wasm3-as
# Flash partition table
idf.py build flash
```

Write TypeType font to SPIFFS

```
parttool.py write_partition --partition-name=font --partition-subtype=spiffs --input resources/spiffs_font.bin
```

Write WebAssembly(.wasm) to SPIFFS ([AssemblyScript Analog Clock](https://h1romas4.github.io/m5stamp-c3dev/asclock/))

```
parttool.py write_partition --partition-name=wasm --partition-subtype=spiffs --input resources/spiffs_wasm.bin
```

Setup WiFi for NTP (Optional)

1. Change WiFi Setting

`nvs_partition.csv`: Set own `[ssid]`, `[password]`

```
key,type,encoding,value
wifi,namespace,,
ssid,data,string,[ssid]
passwd,data,string,[password]
```

2. Create NVS Partation file

```
python ${IDF_PATH}/components/nvs_flash/nvs_partition_generator/nvs_partition_gen.py generate nvs_partition.csv nvs_partition.bin 0x6000
```

3. Write NVS Partation

```
esptool.py write_flash 0x9000 nvs_partition.bin
```

Restart M5Stack Core 2 (NTP synchronization is performed by pressing the virtual button on the screen after the startup logo)

```
idf.py monitor
```

## Build AssemblyScripot

```
cd wasm
npm install
```

**Web Browser Development**

```
npm run asbuild:web
npm run start
# http://localhost:1234/
```

**Build and Flash**

```
npm run asbuild
cd ..
python ${IDF_PATH}/components/spiffs/spiffsgen.py 0x10000 resources/wasm resources/spiffs_wasm.bin
parttool.py write_partition --partition-name=wasm --partition-subtype=spiffs --input resources/spiffs_wasm.bin
```

### Note

- Create SPIFFS parteation file

```
python ${IDF_PATH}/components/spiffs/spiffsgen.py 0x100000 resources/font resources/spiffs_font.bin
python ${IDF_PATH}/components/spiffs/spiffsgen.py 0x10000 resources/wasm resources/spiffs_wasm.bin
```

## Dependencies

Thanks for all the open source.

|Name|Version|License|
|-|-|--|
|[esp-idf](https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32/get-started/index.html)|`v4.4`|BSD License|
|[esp32-arduino](https://github.com/espressif/arduino-esp32)|master (`4da1051`)|LGPL-2.1 License|
|[M5EPD](https://github.com/m5stack/M5EPD)|master(`bf4bd28`)|FreeType Part(The FreeType License)|
|[wasm3](https://github.com/wasm3/wasm3)|master(`dc9fa49`)|MIT License|
|[AsselblyScript](https://github.com/AssemblyScript/assemblyscript)|`0.19.23`|Apache-2.0 License|
|[源真ゴシック](http://jikasei.me/font/genshin/)|-|SIL Open Font License 1.1|

## License

MIT License
