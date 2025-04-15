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

#include "quantum.h"
#include "bmp_pmw3610.h"

uint8_t pmw3610_reg_read(uint8_t addr) {
    spi_start();
    uint8_t snd = addr & 0x7f;
    uint8_t rcv = 0x00;
    spim_start(&snd, 1, NULL, 0, 0xFF);
    //wait_us(160);
    spim_start(NULL, 0, &rcv, 1, 0xFF);
    //wait_us(1);
    spi_stop();
    //wait_us(19);
    // Reset motion_bursting mode if read from a register other than motion
    // burst register
    return rcv;
}

void pmw3610_reg_write(uint8_t addr, uint8_t data) {
    spi_start();
    uint8_t snd[2];
    snd[0] = pmw3610_SPI_CLK_ON | pmw3610_WRITE_FLAG;
    snd[1] = REG_VALUE_SCLK_ENABLE;
    spim_start(snd, 2, NULL, 0, 0xFF);
   // wait_us(35);

    snd[0] = addr | pmw3610_WRITE_FLAG;
    snd[1] = data;
    spim_start(snd, 2, NULL, 0, 0xFF);
   // wait_us(35);

    snd[0] = pmw3610_SPI_CLK_ON | pmw3610_WRITE_FLAG;
    snd[1] = REG_VALUE_SCLK_DISABLE;
    spim_start(snd, 2, NULL, 0, 0xFF);

    spi_stop();
   // wait_us(180);
}

static uint16_t cpi;

uint8_t pmw3610_cpi_get(void) {
    return cpi;
}


void pmw3610_cpi_set(uint8_t cpi) {
    if (cpi > pmw3610_MAXCPI) {
        cpi = pmw3610_MAXCPI;
    }
    pmw3610_reg_write(pmw3610_PAGE0, REG_VALUE_PAGE1_SWITCH);
    pmw3610_reg_write(pmw3610_RES_STEP,cpi);
    pmw3610_reg_write(pmw3610_PAGE0, REG_VALUE_PAGE0_SWITCH);
}

static uint32_t pmw3610_timer      = 0;
static uint32_t pmw3610_scan_count = 0;
static uint32_t pmw3610_last_count = 0;

void pmw3610_scan_perf_task(void) {
    pmw3610_scan_count++;
    uint32_t now = timer_read32();
    if (TIMER_DIFF_32(now, pmw3610_timer) > 1000) {
#if defined(CONSOLE_ENABLE)
        dprintf("pmw3610 scan frequency: %lu\n", pmw3610_scan_count);
#endif
        pmw3610_last_count = pmw3610_scan_count;
        pmw3610_scan_count = 0;
        pmw3610_timer      = now;
    }
}

uint32_t pmw3610_scan_rate_get(void) {
    return pmw3610_last_count;
}

bool pmw3610_motion_burst(pmw3610_motion_t *d) {
#ifdef DEBUG_PMW3610_SCAN_RATE
    pmw3610_scan_perf_task();
#endif
    // Start motion burst if motion burst mode is not started.

    spi_start();
    uint8_t snd = pmw3610_Motion_Burst;
    spim_start(&snd, 1, NULL, 0, 0xFF);
    wait_us(35);
    uint8_t data[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF,0xFF};
    spim_start(NULL, 0, data, sizeof(data), 0xFF);

    spi_stop();
    // Required NCS in 500ns after motion burst.
    //wait_us(1);
    d->stat = data[0];
    d->x = (int16_t)((data[1]<< 4) | ((data[3] & 0xf0) << 8)) >> 4;
    d->y = (int16_t)((data[2]<< 4) | ((data[3] & 0x0f) << 12)) >> 4;

    return true;
}

bool pmw3610_init(void) {
    int res = 0;
    setPinOutput(PMW3610_NCS_PIN);
    spim_init();
    pmw3610_reg_write(pmw3610_Power_Up_Reset, 0x5A);

    uint8_t retry =10;
    do{
        spi_start();
        wait_us(150);
        spi_stop();
        wait_us(150);

         pmw3610_reg_write(pmw3610_Power_Up_Reset, 0x96);

         pmw3610_reg_write(pmw3610_Observation, 0x00);
         wait_us(10*1000);

         uint8_t obs = pmw3610_reg_read(pmw3610_Observation);
         if((obs & 0x0f) == 0x0f){
            res = 1;
         }



    }while(res == 1 && --retry > 0);


    // read five registers of motion and discard those values
    pmw3610_reg_read(pmw3610_Motion);
    pmw3610_reg_read(pmw3610_Delta_X_L);
    pmw3610_reg_read(pmw3610_Delta_Y_L);
    pmw3610_reg_read(pmw3610_Delta_XY_H);


    // check product ID and revision ID
   //res = pmw3610_reg_read(pmw3610_Product_ID) == 0x3e ? 0 : 1;


   // uint8_t rev = pmw3610_reg_read(pmw3610_Revision_ID);
    pmw3610_reg_write(pmw3610_Performance, 0x0d);
    pmw3610_reg_write(pmw3610_Run_Downshift, 0x04);
     pmw3610_reg_write(pmw3610_Rest1_Rate, 0x04);
      pmw3610_reg_write(pmw3610_Rest1_Downshift, 0x0f);
 //   spi_stop();
    return res;
}

