/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
#include "display_support.h"
#include "lvgl_support.h"
#include "mcmgr.h"
#include "fsl_cache.h"
#include "FreeRTOS.h"
/*${header:end}*/

/*${function:start}*/
#if (DEMO_PANEL_RM67162 == DEMO_PANEL) || (DEMO_PANEL_CO5300 == DEMO_PANEL)

void  BOARD_MIPI_TE_GPIO_IRQ_Handler(void)
{
    uint32_t intStat;

    intStat = GPIO_PinGetInterruptFlag(BOARD_MIPI_TE_GPIO, BOARD_MIPI_TE_PIN);

    if (intStat != 0U)
    {
        GPIO_GpioClearInterruptFlags(BOARD_MIPI_TE_GPIO, 1U << BOARD_MIPI_TE_PIN);
        BOARD_DisplayTEPinHandler();
    }
}
void  BOARD_MIPI_TOUCH_INT_GPIO_IRQ_Handler(void)
{
    uint32_t intStat;
    intStat = GPIO_PinGetInterruptFlag(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, BOARD_MIPI_PANEL_TOUCH_INT_PIN);

    if (intStat != 0U)
    {
        GPIO_GpioClearInterruptFlags(BOARD_MIPI_PANEL_TOUCH_INT_GPIO, 1U << BOARD_MIPI_PANEL_TOUCH_INT_PIN);
        BOARD_TouchIntHandler();
    }
    else if (GPIO_PinGetInterruptFlag(BOARD_SW7_GPIO, BOARD_SW7_GPIO_PIN))
    {
        BOARD_SW7_IRQ_HANDLER();
    }
}
#endif


void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitAHBSC();
    BOARD_InitBootPins();
    BOARD_InitPMICPins();
    BOARD_InitUserSWPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

#if (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL)
#if (SSD1963_DRIVEN_BY == SSD1963_DRIVEN_BY_FLEXIO)
    BOARD_InitFlexIOPanelPins();
#else
    BOARD_InitLcdDBIPanelPins();
#endif
    CLOCK_EnableClock(kCLOCK_Gpio2);
    RESET_PeripheralReset(kGPIO2_RST_SHIFT_RSTn);
    BOARD_InitI2c8();
#else
#if (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL)
    BOARD_InitI2c8();
#endif
    BOARD_InitMipiPanelPins();
    CLOCK_EnableClock(kCLOCK_Gpio1);
    CLOCK_EnableClock(kCLOCK_Gpio3);
    RESET_PeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO3_RST_SHIFT_RSTn);

    BOARD_InitMipiTouchPanelPins();

#endif

#if ((DEMO_PANEL_RK055AHD091 == DEMO_PANEL) || (DEMO_PANEL_RK055MHD091 == DEMO_PANEL) || \
     (DEMO_PANEL_RK055IQH091 == DEMO_PANEL) || (DEMO_PANEL_RM67162 == DEMO_PANEL) || \
     (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL) || (DEMO_PANEL_RASPI_7INCH == DEMO_PANEL) || \
     (DEMO_PANEL_CO5300 == DEMO_PANEL))
    BOARD_MIPIPanelTouch_I2C_Init();
#else
#endif

    /* Powerup all the SRAM partitions. */
    PMC0->PDRUNCFG2 &= ~0x3FFC0000;
    PMC0->PDRUNCFG3 &= ~0x3FFC0000;

    POWER_DisablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK);
    POWER_ApplyPD();
}

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void)
{
    uint32_t image_size;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    image_size = (uint32_t)&Image$$CORE1_REGION$$Length;
#elif defined(__ICCARM__)
    image_size = (uint32_t)__section_end("__core1_image") - (uint32_t)__section_begin("__core1_image");
#elif defined(__GNUC__)
    image_size = (uint32_t)core1_image_size;
#endif
    return image_size;
}
#endif /* CORE1_IMAGE_COPY_TO_RAM */

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
void invalidate_cache_for_core1_image_memory(uint32_t address, uint32_t size_byte)
{
    XCACHE_CleanInvalidateCacheByRange(address, size_byte);
}
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}

#if configUSE_TICKLESS_IDLE == 2

/*!
 * @brief Fuction of OS timer.
 *
 * This function to return OS timer base address
 */

OSTIMER_Type *vPortGetOstimerBase(void)
{
    return TICKLESS_OSTIMER_BASE_PTR;
}

/*!
 * @brief Fuction of OS timer.
 *
 * This function to return OS timer interrupt number
 */

IRQn_Type vPortGetOstimerIrqn(void)
{
    return TICKLESS_OSTIMER_IRQn;
}
#endif
/*${function:end}*/
