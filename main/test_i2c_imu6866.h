typedef struct
{
    float_t pitch;
    float_t roll;
    float_t yaw;
} imu6886_t;

void init_i2c_imu6886(void);
void tick_i2c_imu6886(void);
void get_i2c_imu6886(imu6886_t *imu6886);
