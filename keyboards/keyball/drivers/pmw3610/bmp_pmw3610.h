/*
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdint.h>
#include "apidef.h"
#include "gpio.h"

//////////////////////////////////////////////////////////////////////////////
// Configurations

#ifndef PMW3610_NCS_PIN
#    define PMW3610_NCS_PIN 13
#endif

#ifndef PMW3610_SDIO
#    define PMW3610_SDIO 14
#endif

#ifndef PMW3610_MOTION
#    define PMW3610_MOTION 15
#endif

#ifndef PMW3610_SCLK
#    define PMW3610_SCLK 16
#endif

#define PMW3610_SPI_MODE 3
#define PMW3610_SPI_DIVISOR (F_CPU / PMW3610_CLOCKS)
#define PMW3610_CLOCKS 2000000

/// DEBUG_PMW3360_SCAN_RATE enables scan performance counter.
/// It records scan count in a last second and enables pmw3360_scan_rate_get().
/// Additionally, it will be logged automatically when defined CONSOLE_ENABLE
/// and `debug_enable = true`.
//#define DEBUG_PMW3360_SCAN_RATE

//////////////////////////////////////////////////////////////////////////////
// Types
typedef struct {
    uint8_t stat;
    int16_t x;
    int16_t y;
} pmw3610_motion_t;

typedef enum {

    pmw3610_Product_ID  = 0x00,
    pmw3610_Revision_ID = 0x01,
    pmw3610_Motion = 0x02,
    pmw3610_Delta_X_L = 0x03,
    pmw3610_Delta_Y_L = 0x04,
    pmw3610_Delta_XY_H = 0x05,
    pmw3610_Performance = 0x11,
    pmw3610_Motion_Burst = 0x12,
    pmw3610_Run_Downshift = 0x1b,
    pmw3610_Rest1_Rate = 0x1c,
    pmw3610_Rest1_Downshift = 0x1d,
    pmw3610_Observation = 0x2d,
    pmw3610_Power_Up_Reset = 0x3a,
    pmw3610_Shutdown = 0x3b,
    pmw3610_SPI_CLK_ON = 0x41,
    pmw3610_PAGE0 = 0x7f,
    pmw3610_WRITE_FLAG = 0x80,
    pmw3610_RES_STEP = 0x85,
} pmw3610_reg_t;

enum {
    REG_VALUE_SCLK_ENABLE = 0xba,
    REG_VALUE_SCLK_DISABLE = 0xb5,
    REG_VALUE_PAGE1_SWITCH = 0xff,
    REG_VALUE_PAGE0_SWITCH = 0x00,
    pmw3610_MAXCPI = 0x10, // = 3200 CPI
};

//////////////////////////////////////////////////////////////////////////////
// Exported values (touch carefully)

//////////////////////////////////////////////////////////////////////////////
// Top level API

/// pmw3360_init initializes PMW3360DM-T2QU module.
/// It will return true when succeeded, otherwise false.
bool pmw3610_init(void);


/// pmw3360_motion_burst gets a motion data by Motion_Burst command.
/// This requires to write a dummy data to pmw3360_Motion_Burst register
/// just before.
bool pmw3610_motion_burst(pmw3610_motion_t *d);

/// pmw3360_scan_rate_get gets count of scan in a last second.
/// This works only when DEBUG_PMW3360_SCAN_RATE is defined.
uint32_t pmw3610_scan_rate_get(void);

// TODO: document
uint8_t pmw3610_cpi_get(void);

// TODO: document
void pmw3610_cpi_set(uint8_t cpi);

//////////////////////////////////////////////////////////////////////////////
// Register operations

/// pmw3360_reg_write writes a value to a register.
void pmw3610_reg_write(uint8_t addr, uint8_t data);

/// pmw3360_reg_read reads a value from a register.
uint8_t pmw3610_reg_read(uint8_t addr);

//////////////////////////////////////////////////////////////////////////////
// SPI operations

static inline bool spim_init(void) {
  bmp_api_spim_config_t config = {
    .freq = SPI_FREQ_2M,
    .miso = PMW3610_SDIO,
    .mosi = PMW3610_SDIO,
    .sck  = PMW3610_SCLK,
    .mode = PMW3610_SPI_MODE
  };
  return BMPAPI->spim.init(&config);
}

static inline int spim_start(uint8_t * p_tx_buffer, size_t tx_length, uint8_t * p_rx_buffer, size_t rx_length, uint8_t cs_pin) {
  return BMPAPI->spim.start(p_tx_buffer, tx_length, p_rx_buffer, rx_length, cs_pin);
}

static inline void spi_start(void) {
  setPinOutput(PMW3610_NCS_PIN);
  writePinLow(PMW3610_NCS_PIN);
  wait_us(1);
}

static inline void spi_stop(void) {
  wait_us(1);
  setPinOutput(PMW3610_NCS_PIN);
  writePinHigh(PMW3610_NCS_PIN);
}

