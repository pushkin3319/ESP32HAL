/**
This file is part of ESP32HAL esp-idf component (https://github.com/CalinRadoni/ESP32HAL)
Copyright (C) 2020 by Calin Radoni

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "esp32_hal_i2c.h"

// -----------------------------------------------------------------------------

esp32_hal_i2c::esp32_hal_i2c(void)
{
    initialized = false;
    i2c_port = i2c_port_t::I2C_NUM_MAX;
}

esp32_hal_i2c::~esp32_hal_i2c()
{
    CleanUp();
}

// -----------------------------------------------------------------------------

bool esp32_hal_i2c::Initialize(uint8_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio, uint32_t clock_speed, bool enable_pull_up)
{
    i2c_config_t config;

    if (initialized) CleanUp();

    switch (port) {
        case 0:
            i2c_port = i2c_port_t::I2C_NUM_0;
            break;
        case 1:
            i2c_port = i2c_port_t::I2C_NUM_1;
            break;
        default:
            i2c_port = i2c_port_t::I2C_NUM_MAX;
            return false;
    }

    config.mode = i2c_mode_t::I2C_MODE_MASTER;
    config.sda_io_num = sda_gpio;
    config.scl_io_num = scl_gpio;
    config.sda_pullup_en = enable_pull_up ? gpio_pullup_t::GPIO_PULLUP_ENABLE : gpio_pullup_t::GPIO_PULLUP_DISABLE;
    config.scl_pullup_en = enable_pull_up ? gpio_pullup_t::GPIO_PULLUP_ENABLE : gpio_pullup_t::GPIO_PULLUP_DISABLE;
    config.master.clk_speed = clock_speed;

    esp_err_t err = i2c_param_config(i2c_port, &config);
    if (err != ESP_OK) return false;

    err = i2c_driver_install(i2c_port, config.mode, 0, 0, 0);
    if (err != ESP_OK) return false;

    initialized = true;
    return true;
}

void esp32_hal_i2c::CleanUp(void)
{
    if (initialized) {
        i2c_driver_delete(i2c_port);
        initialized = false;
    }
}

// -----------------------------------------------------------------------------
