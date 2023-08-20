# m5stack-core2-wasm3-as

![](https://github.com/h1romas4/m5stack-core2-wasm3-as/workflows/Build/badge.svg)

M5Stack Core2(Xtensa) With Wasm3/AssemblyScript Demo.

![Wasm1](https://raw.githubusercontent.com/h1romas4/m5stack-core2-wasm3-as/main/docs/images/m5stack-core2-01.jpg)

See also: RISC-V version

> [m5stamp-c3dev](https://github.com/h1romas4/m5stamp-c3dev)
>
> This is a development board for the M5Stamp C3 (RISC-V/FreeRTOS).

## Twitter Video

- [📼 Demo Clock](https://twitter.com/h1romas4/status/1609484777186553861)
- [📼 Demo 3D Cube1](https://twitter.com/h1romas4/status/1609882456781623296)
- [📼 Demo 3D Cube2](https://twitter.com/h1romas4/status/1610228824607985664)
- [📼 Demo 3D Cube with IMU6886](https://twitter.com/h1romas4/status/1612348626248044544)

## Build

### Require

- [Setup ESF-IDF v4.4.4](https://docs.espressif.com/projects/esp-idf/en/v4.4.4/esp32/get-started/index.html#installation-step-by-step)

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
COLLECT_LTO_WRAPPER=/home/hiromasa/.espressif/tools/xtensa-esp32-elf/esp-2021r2-patch3-8.4.0/xtensa-esp32-elf/bin/../libexec/gcc/xtensa-esp32-elf/8.4.0/lto-wrapper
lto-wrapper
Target: xtensa-esp32-elf
... snip ...
Thread model: posix
gcc version 8.4.0 (crosstool-NG esp-2021r2-patch5)
```

### Build and Execute

1. git clone and build

```
git clone --recursive https://github.com/h1romas4/m5stack-core2-wasm3-as
cd m5stack-core2-wasm3-as
idf.py build
```

2. Write Partition table

```
idf.py partition-table-flash
```

3. Write TypeType font to SPIFFS

```
parttool.py write_partition --partition-name=font --partition-subtype=spiffs --input resources/spiffs_font.bin
```

4. Write WebAssembly(.wasm) to SPIFFS ([WebAssembly Apps](https://github.com/h1romas4/m5stack-core2-wasm3-as/tree/main/wasm))

```
parttool.py write_partition --partition-name=wasm --partition-subtype=spiffs --input resources/spiffs_wasm.bin
```

5. Restart M5Stack Core 2

```
idf.py flash monitor
```

### Switch WebAssembly Apps

```
idf.py menuconfig
```

`WebAssembly Apps - Select WebAssembly Apps`

![menuconfig](https://raw.githubusercontent.com/h1romas4/m5stack-core2-wasm3-as/main/docs/images/m5stack-core2-03.png)

```
idf.py build flash monitor
```

![Wasm2](https://raw.githubusercontent.com/h1romas4/m5stack-core2-wasm3-as/main/docs/images/m5stack-core2-05.jpg)

### Setup WiFi (Optional)

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

4. Run

NTP synchronization is performed by pressing the "Center button" after the startup logo.

```
idf.py monitor
```

## Build AssemblyScript

```
cd wasm/clockenv # or wasm/imu6886 or wasm/gpsgsv
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
cd ../..
python ${IDF_PATH}/components/spiffs/spiffsgen.py 0x10000 resources/wasm resources/spiffs_wasm.bin
parttool.py write_partition --partition-name=wasm --partition-subtype=spiffs --input resources/spiffs_wasm.bin
```

## Note

- Create SPIFFS parteation file

```
# for TrueType font
python ${IDF_PATH}/components/spiffs/spiffsgen.py 0x100000 resources/font resources/spiffs_font.bin
# for .wasm binary
python ${IDF_PATH}/components/spiffs/spiffsgen.py 0x10000 resources/wasm resources/spiffs_wasm.bin
```

## Dependencies

Thanks for all the open source.

|Name|Version|License|
|-|-|--|
|[esp-idf](https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32/get-started/index.html)|`v4.4.5`|BSD License|
|[arduino-esp32](https://github.com/espressif/arduino-esp32)|`2.0.11`|LGPL-2.1 License|
|[M5Core2](https://github.com/m5stack/M5Core2)|`0.1.5`|MIT License|
|[M5EPD](https://github.com/m5stack/M5EPD)|`0.1.4`|FreeType Part(The FreeType License)|
|[lwgps](https://github.com/MaJerle/lwgps)|`v2.1.0`|MIT License|
|[Wasm3](https://github.com/wasm3/wasm3)|master(`045040a9`)|MIT License|
|[AsselblyScript](https://github.com/AssemblyScript/assemblyscript)|`0.27.9`|Apache-2.0 License|
|[源真ゴシック](http://jikasei.me/font/genshin/)|-|SIL Open Font License 1.1|

## License

MIT License
