/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_mu.h"
#include "fsl_power.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_MU             MU1_MUA
#define CHN_MU_REG_NUM     kMU_MsgReg0

#define APP_RTC             RTC0
#define APP_RTC_IRQN        RTC0_ALARM_IRQn
#define APP_RTC_IRQ_HANDLER RTC0_ALARM_IRQHandler

#define BOARD_SW5_IRQ         GPIO00_IRQn
#define BOARD_SW5_LPREQ       kPower_GPIO0_LPREQ
#define BOARD_SW7_IRQ         GPIO10_IRQn
#define BOARD_SW7_LPREQ       kPower_GPIO1_LPREQ
#define BOARD_SW5_IRQ_HANDLER GPIO00_IRQHandler

/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS 0x205A0000
#define APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY

/* @brief FreeRTOS tickless timer configuration. */
#define TICKLESS_OSTIMER_BASE_PTR OSTIMER_CPU0      /*!< Tickless timer base address. */
#define TICKLESS_OSTIMER_IRQn     OS_EVENT_IRQn /*!< Tickless timer IRQ number. */

#define APP_SLEEP_CONSTRAINTS                                                                                       \
    27, \
    PM_RESC_COMP_MAINCLK_ON, \
    PM_RESC_SENSEP_MAINCLK_ON, \
    PM_RESC_SENSES_MAINCLK_ON, \
    PM_RESC_RAM0CLK_ON, \
    PM_RESC_COMN_MAINCLK_ON, \
    PM_RESC_MEDIA_MAINCLK_ON, \
    PM_RESC_XTAL_ON, \
    PM_RESC_FRO0_ON, \
    PM_RESC_LPOSC_ON, \
    PM_RESC_FRO0_EN, \
    PM_RESC_V2NMED_ON, \
    PM_RESC_VNCOM_ON, \
    PM_RESC_V2MIPI_ON, \
    PM_RESC_SRAM7_128KB_ACTIVE, \
    PM_RESC_SRAM8_256KB_ACTIVE, \
    PM_RESC_SRAM9_256KB_ACTIVE, \
    PM_RESC_SRAM11_512KB_ACTIVE, \
    PM_RESC_SRAM12_1MB_ACTIVE, \
    PM_RESC_SRAM15_512KB_ACTIVE, \
    PM_RESC_SRAM16_256KB_ACTIVE, \
    PM_RESC_SRAM_MIPI_ACTIVE, \
    PM_RESC_SRAM_GPU_ACTIVE, \
    PM_RESC_SRAM_CPU0_ICACHE_ACTIVE, \
    PM_RESC_SRAM_CPU0_DCACHE_ACTIVE, \
    PM_RESC_SRAM_XSPI0_ACTIVE, \
    PM_RESC_SRAM_LCDIF_ACTIVE, \
    PM_RESC_SRAM_OCOTP_ACTIVE


#define APP_DEEP_SLEEP_CONSTRAINTS  \
    11,                             \
    PM_RESC_VNCOM_ON,               \
    PM_RESC_V2NMED_ON,              \
    PM_RESC_SRAM7_128KB_RETENTION,  \
    PM_RESC_SRAM8_256KB_RETENTION,  \
    PM_RESC_SRAM9_256KB_RETENTION,  \
    PM_RESC_SRAM11_512KB_RETENTION, \
    PM_RESC_SRAM12_1MB_RETENTION,   \
    PM_RESC_SRAM15_512KB_RETENTION, \
    PM_RESC_SRAM16_256KB_RETENTION, \
    PM_RESC_SRAM_CPU0_ICACHE_RETENTION, \
    PM_RESC_SRAM_CPU0_DCACHE_RETENTION

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
#define CORE1_IMAGE_START __section_begin("__core1_image")
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern uint32_t core1_image_size;
#define CORE1_IMAGE_START (core1_image_start)
#define CORE1_IMAGE_SIZE  (core1_image_size)
#endif
/*${macro:end}*/

typedef enum {
  TIME_UNIT_SECOND = 0,
  TIME_UNIT_MINUTE,
  TIME_UNIT_HOUR
} time_unit_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*${prototype:start}*/
void BOARD_InitHardware(void);
extern OSTIMER_Type *vPortGetOstimerBase(void);
extern IRQn_Type vPortGetOstimerIrqn(void);

void BOARD_SW7_IRQ_HANDLER();

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif /* CORE1_IMAGE_COPY_TO_RAM */

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
void invalidate_cache_for_core1_image_memory(uint32_t address, uint32_t size_byte);
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */
/*${prototype:end}*/

#endif /* _APP_H_ */
