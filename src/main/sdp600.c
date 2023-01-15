#include "freertos/FreeRTOS.h"

#include "sdp600.h"

#define SDP600_ADDR 0x40
#define SDP600_CMD_TRIGGER_MEASUREMENT 0xF1
#define SDP600_CMD_SOFT_RESET 0xFE
#define SDP600_CMD_READ_ADVANCED_USER_REGISTER 0xE5
#define SDP600_CMD_WRITE_ADVANCED_USER_REGISTER 0xE4

int i2c_master_port = 0;

i2c_config_t conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = 21,
    .scl_io_num = 22,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 400000,
    .clk_flags = 0,
};

esp_err_t sdp600_i2c_init()
{
    i2c_param_config(i2c_master_port, &conf);

    /* SDP600 is using painfully long clock stretching which is disabled by default*/
    i2c_set_timeout(i2c_master_port, 400000);

    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

esp_err_t sdp600_soft_reset()
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    /* Send device address with write bit */
    i2c_master_write_byte(cmd, (SDP600_ADDR << 1) | I2C_MASTER_WRITE, I2C_ACK_VAL);

    /* Send command  */
    i2c_master_write_byte(cmd, SDP600_CMD_SOFT_RESET, I2C_ACK_VAL);

    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

esp_err_t sdp600_read(int16_t *measurement)
{
    uint8_t sdp_read[3] = {0};

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);

    /* Send device address with write bit */
    i2c_master_write_byte(cmd, (SDP600_ADDR << 1) | I2C_MASTER_WRITE, I2C_ACK_VAL);

    /* Send command */
    i2c_master_write_byte(cmd, SDP600_CMD_TRIGGER_MEASUREMENT, I2C_ACK_VAL);

    /* Send repeated start */
    i2c_master_start(cmd);

    /* Now send device address with read bit */
    i2c_master_write_byte(cmd, (SDP600_ADDR << 1) | I2C_MASTER_READ, I2C_ACK_VAL);

    /* Read Data */
    i2c_master_read(cmd, sdp_read, 3, I2C_ACK_VAL);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(i2c_master_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    *measurement = ((int16_t)sdp_read[0] << 8) | sdp_read[1];

    return ret;
}