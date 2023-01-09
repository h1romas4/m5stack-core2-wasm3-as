#include "M5Core2.h"
#include "esp_log.h"
#include "BluetoothSerial.h"

#include "test_bluetooth_serial.h"

#define DEVICE_NAME "IMU6886"

// https://github.com/espressif/arduino-esp32/blob/master/libraries/BluetoothSerial/examples/SerialToSerialBT/SerialToSerialBT.ino
// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #if !defined(CONFIG_BT_SPP_ENABLED)
// It is only available for the ESP32 chip.
BluetoothSerial SerialBT;

static const char *TAG = "test_bluetooth_serial.cpp";

void init_bluetooth_serial(void)
{
    // Bluetooth SPP (Serial Port Protocol)
    // $ sudo rfcomm -r -M -L 0 bind 0 XX:XX:XX:XX:XX:XX
    // $ sudo rfcomm -a
    // rfcomm0: XX:XX:XX:XX:XX:XX channel 1 clean
    // $ sudo screen /dev/rfcomm0 115200 # stop ctrl + a, k
    // $ sudo rfcomm -a
    // rfcomm0: XX:XX:XX:XX:XX:XX channel 1 connected [tty-attached]
    // $ sudo rfcomm release 0
    SerialBT.begin(DEVICE_NAME);
}

void printf_bluetooth_serial(const char *str)
{
    SerialBT.printf(str);
}
