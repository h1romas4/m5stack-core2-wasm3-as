#include "M5Core2.h"
#include "esp_log.h"
#include "BluetoothSerial.h"

#include "test_i2c_imu6866.h"

#define SAMPLE_RATE 25

uint32_t microsPerReading;
uint32_t microsPrevious;

float_t roll;
float_t pitch;
float_t heading;

BluetoothSerial SerialBT;

static const char *TAG = "test_i2c_imu6866.cpp";

float_t convertRawAcceleration(float_t aRaw)
{
    // since we are using 2 g range
    // -2 g maps to a raw value of -32768
    // +2 g maps to a raw value of 32767
    float_t a = (aRaw * 2.0) / 32768.0;
    return a;
}

float_t convertRawGyro(float_t gRaw)
{
    // since we are using 250 degrees/seconds range
    // -250 maps to a raw value of -32768
    // +250 maps to a raw value of 32767
    float_t g = (gRaw * 250.0) / 32768.0;
    return g;
}

void init_i2c_imu6886(void)
{
    M5.IMU.Init();
    // Set the accelerometer range to 2 g
    M5.IMU.SetAccelFsr(M5.IMU.Ascale::AFS_2G);
    // Set the gyroscope range to 250 degrees/second
    M5.IMU.SetGyroFsr(M5.IMU.Gscale::GFS_250DPS);
    // initialize variables to pace updates to correct rate
    microsPerReading = 1000000 / SAMPLE_RATE;
    microsPrevious = micros();
    // Bluetooth Serial
    // $ sudo rfcomm -r -M -L 0 bind 0 XX:XX:XX:XX:XX:XX
    // $ sudo rfcomm -a
    // rfcomm0: XX:XX:XX:XX:XX:XX channel 1 clean
    // $ sudo tail -f /dev/rfcomm0
    // $ sudo rfcomm -a
    // rfcomm0: XX:XX:XX:XX:XX:XX channel 1 connected [tty-attached]
    // $ sudo rfcomm release 0
    SerialBT.begin("IMU6886");
}

void tick_i2c_imu6886(void)
{
    uint32_t microsNow;

    // check if it's time to read data and update the filter
    microsNow = micros();
    if (microsNow - microsPrevious < microsPerReading) {
        return;
    }

    // float_t aix, aiy, aiz;
    // float_t gix, giy, giz;
    // float_t ax, ay, az;
    // float_t gx, gy, gz;
    // read raw data from IMU
    // M5.IMU.getAccelData(&aix, &aiy, &aiz);
    // M5.IMU.getGyroData(&gix, &giy, &giz);
    // convert from raw data to gravity and degrees/second units
    // ax = convertRawAcceleration(aix);
    // ay = convertRawAcceleration(aiy);
    // az = convertRawAcceleration(aiz);
    // gx = convertRawGyro(gix);
    // gy = convertRawGyro(giy);
    // gz = convertRawGyro(giz);

    M5.IMU.getAhrsData(&pitch, &roll, &heading);

    // M5.Lcd.setCursor(0, 75);
    // role (0↑90↓180)  pitch (-90←0→+90)
    // M5.Lcd.printf("%6.2f %6.2f %6.2f", roll, pitch, heading);

    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;
}

void get_i2c_imu6886(imu6886_t *imu6886)
{
    imu6886->pitch = pitch;
    imu6886->roll = roll;
    imu6886->yaw = heading;
}
