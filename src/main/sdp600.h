#ifndef SDP600_H_
#define SDP600_H_

#include <stdint.h>

#include "i2c.h"

esp_err_t sdp600_i2c_init();
esp_err_t sdp600_read(int16_t *measurement);
esp_err_t sdp600_soft_reset();

#endif