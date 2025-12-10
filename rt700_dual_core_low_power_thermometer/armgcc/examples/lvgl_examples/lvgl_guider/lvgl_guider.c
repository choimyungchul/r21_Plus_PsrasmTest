/*
 * Copyright 2020, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "lvgl_support.h"
#include "board.h"
#include "app.h"
#include "lvgl.h"
#include "widgets_init.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "lvgl_demo_utils.h"
#include "core1_support.h"
#include "power_management.h"
#include "mcmgr.h"
#include "fsl_irtc.h"
#include "semphr.h"
#include <time.h>
#include "fsl_pm_core.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

 #define TEMP_ARRAY_SIZE    10
/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool s_lvgl_initialized = false;
lv_ui guider_ui;

SemaphoreHandle_t xMUBinarySemaphore = NULL;
SemaphoreHandle_t xRTCBinarySemaphore = NULL;

static uint32_t result;
static int32_t temperatureArray[TEMP_ARRAY_SIZE] = {0};
 
static int clock_hour_value = 11;
static int clock_min_value = 25;
static int clock_sec_value = 50;
static char clock_meridiem[] = "AM";

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void print_cb(lv_log_level_t level, const char * buf);

#if LV_USE_LOG
void print_cb(lv_log_level_t level, const char * buf)
{
    LV_UNUSED(level);

    PRINTF("\r%s\n", buf);
}
#endif

void MU_Rx0FullFlagISR(MU_Type *base, mcmgr_core_t coreNum)
{
    if (base == APP_MU)
    {
        result = MU_ReceiveMsg(APP_MU, CHN_MU_REG_NUM);
        if (xMUBinarySemaphore != NULL)
        {           
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(xMUBinarySemaphore, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

static void increaseRTC(time_unit_t valueToIncrease)
{
    IRTC_SetWriteProtection(APP_RTC, false);
    IRTC_DisableInterrupts(APP_RTC, IRTC_GetEnabledInterrupts(APP_RTC));
    
    irtc_datetime_t datetime;
    struct tm datetime_tm;
    time_t epoch;
    /* Read the RTC seconds register to get current time in seconds */
    IRTC_GetDatetime(APP_RTC, &datetime);

    datetime_tm.tm_year  = datetime.year - 1900;
    datetime_tm.tm_mon   = datetime.month - 1;
    datetime_tm.tm_mday  = datetime.day;
    datetime_tm.tm_hour  = datetime.hour;
    datetime_tm.tm_min   = datetime.minute;
    datetime_tm.tm_sec   = datetime.second;

    epoch = mktime(&datetime_tm);
    switch(valueToIncrease)
    {
        case TIME_UNIT_MINUTE:
            epoch += 60;
        break;
        case TIME_UNIT_HOUR:
            epoch += 3600;
        break;
        default:
        break;
    }
    gmtime_r(&epoch, &datetime_tm);

    datetime.year   = datetime_tm.tm_year + 1900;
    datetime.month  = datetime_tm.tm_mon + 1;
    datetime.day    = datetime_tm.tm_mday;
    datetime.hour   = datetime_tm.tm_hour;
    datetime.minute = datetime_tm.tm_min ;
    datetime.second = datetime_tm.tm_sec ;
    IRTC_SetDatetime(APP_RTC, &datetime);
}

void BOARD_SW5_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(BOARD_SW5_GPIO, 1U << BOARD_SW5_GPIO_PIN);
    increaseRTC(TIME_UNIT_HOUR);
    xSemaphoreGive(xRTCBinarySemaphore);
}

void BOARD_SW7_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(BOARD_SW7_GPIO, 1U << BOARD_SW7_GPIO_PIN);
    increaseRTC(TIME_UNIT_MINUTE);
    xSemaphoreGive(xRTCBinarySemaphore);
}

void APP_RTC_IRQ_HANDLER(void)
{
    uint32_t flags = IRTC_GetStatusFlags(APP_RTC);
    if ((flags & kIRTC_AlarmFlag) != 0U)
    {
        /* Unlock to allow register write operation */
        IRTC_SetWriteProtection(APP_RTC, false);
        /*Clear all irtc flag */
        IRTC_ClearStatusFlags(APP_RTC, flags);
        if (xRTCBinarySemaphore != NULL)
        {           
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(xRTCBinarySemaphore, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            IRTC_DisableInterrupts(APP_RTC, kIRTC_AlarmInterruptEnable);
            DisableDeepSleepIRQ(APP_RTC_IRQN);
        }
    }
    IRTC_ClearStatusFlags(APP_RTC, flags);
}

static void APP_InitTimeButtons(void)
{
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    RESET_ClearPeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    RESET_ClearPeripheralReset(kGPIO1_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio1);

    GPIO_SetPinInterruptConfig(BOARD_SW5_GPIO, BOARD_SW5_GPIO_PIN, kGPIO_InterruptFallingEdge);
    GPIO_SetPinInterruptConfig(BOARD_SW7_GPIO, BOARD_SW7_GPIO_PIN, kGPIO_InterruptFallingEdge);
    EnableIRQ(BOARD_SW5_IRQ);
    EnableIRQ(BOARD_SW7_IRQ);
    GPIO_PinInit(BOARD_SW5_GPIO, BOARD_SW5_GPIO_PIN, &sw_config);
    GPIO_PinInit(BOARD_SW7_GPIO, BOARD_SW7_GPIO_PIN, &sw_config);
}

static void APP_InitWakeupTimer(void)
{
    irtc_config_t irtcConfig;

    IRTC_GetDefaultConfig(&irtcConfig);
    if (IRTC_Init(APP_RTC, &irtcConfig) != kStatus_Success)
    {
        PRINTF("RTC Init Failed.\r\n");
    }
}

static void APP_SetWakeupConfig(void)
{
    irtc_datetime_t datetime;
    struct tm datetime_tm;
    time_t epoch;
    /* Read the RTC seconds register to get current time in seconds */
    IRTC_GetDatetime(APP_RTC, &datetime);

    datetime_tm.tm_year  = datetime.year - 1900;
    datetime_tm.tm_mon   = datetime.month - 1;
    datetime_tm.tm_mday  = datetime.day;
    datetime_tm.tm_hour  = datetime.hour;
    datetime_tm.tm_min   = datetime.minute;
    datetime_tm.tm_sec   = datetime.second;

    epoch = mktime(&datetime_tm);
    /* Add alarm seconds to current time */
    epoch++;
    gmtime_r(&epoch, &datetime_tm);

    datetime.year   = datetime_tm.tm_year + 1900;
    datetime.month  = datetime_tm.tm_mon + 1;
    datetime.day    = datetime_tm.tm_mday;
    datetime.hour   = datetime_tm.tm_hour;
    datetime.minute = datetime_tm.tm_min ;
    datetime.second = datetime_tm.tm_sec ;
    
    /* Unlock to allow register write operation */
    IRTC_SetWriteProtection(APP_RTC, false);
    /* Set alarm time in seconds */
    IRTC_SetAlarm(APP_RTC, &datetime);
    /* Enable RTC alarm interrupt */
    IRTC_EnableInterrupts(APP_RTC, kIRTC_AlarmInterruptEnable);
    NVIC_SetPriority(APP_RTC_IRQN, 4);
    EnableDeepSleepIRQ(APP_RTC_IRQN);
}

void UpdateDisplayedTemperature(int32_t temperature)
{
    static bool initialized = false;
    static int8_t prevIntegerTemp = 0, prevDecimalTemp = 0;
    int8_t integerTemp, decimalTemp;
    int32_t sum = 0, meanTemperature;

    if (!initialized)
    {
        for (uint8_t i =  0 ; i < TEMP_ARRAY_SIZE ; i++) {
            temperatureArray[i] = temperature;
        }
        initialized = true;
    }

    for (uint8_t i = TEMP_ARRAY_SIZE - 1 ; i > 0; i--) {
        temperatureArray[i] = temperatureArray[i - 1];
        sum += temperatureArray[i];
    }

    temperatureArray[0] = temperature;
    sum += temperatureArray[0];
    meanTemperature = sum / TEMP_ARRAY_SIZE;
    
    integerTemp = meanTemperature/10000;
    decimalTemp = meanTemperature/1000 - integerTemp*10;
    if (prevIntegerTemp != integerTemp || prevDecimalTemp != decimalTemp)
    {
        prevIntegerTemp = integerTemp;
        prevDecimalTemp = decimalTemp;
        lv_label_set_text_fmt((&guider_ui)->screen_TempDigits_Back, "%d.%d", integerTemp, decimalTemp);
        lv_label_set_text_fmt((&guider_ui)->screen_TempDigits_Front, "%d.%d", integerTemp, decimalTemp);
    }
}

void clock_labels_update()
{
    static bool odd = false;
    irtc_datetime_t currentTime;
    /* Read the RTC to get current time and apply to lvgl labels */
    IRTC_GetDatetime(APP_RTC, &currentTime);

    if (clock_sec_value != currentTime.second)
    {
        clock_sec_value = currentTime.second;
        lv_label_set_text_fmt(guider_ui.screen_TempUnit_Back, "%s", (odd ? "°C" : " C"));
        lv_label_set_text_fmt(guider_ui.screen_TempUnit_Front, "%s", (odd ? "°C" : " C"));
        odd = !odd;
    }

    if (clock_min_value != currentTime.minute ||
        (currentTime.hour > 12 ? (currentTime.hour % 12 != clock_hour_value) :
        (currentTime.hour == 12 ? (currentTime.hour != clock_hour_value) :
        (currentTime.hour != clock_hour_value % 12)))
        )
    {
        clock_hour_value = currentTime.hour;
        clock_min_value = currentTime.minute;

        if (currentTime.hour == 0) clock_hour_value = 12;
        if (currentTime.hour >= 12)
        {
            lv_strcpy(clock_meridiem, "PM");
            if (currentTime.hour != 12) clock_hour_value = currentTime.hour - 12;
        }
        else
        {
            lv_strcpy(clock_meridiem, "AM");
        }

        if (lv_obj_is_valid(guider_ui.screen_Clock_Back) && lv_obj_is_valid(guider_ui.screen_Clock_Front))
        {
            lv_label_set_text_fmt(guider_ui.screen_Clock_Back, "%d:%02d %s", clock_hour_value, clock_min_value, clock_meridiem);
            lv_label_set_text_fmt(guider_ui.screen_Clock_Front, "%d:%02d %s", clock_hour_value, clock_min_value, clock_meridiem);
        }
    }
}

static void AppTask(void *param)
{
    MU_EnableInterrupts(APP_MU, (kMU_Rx0FullInterruptEnable));

    xMUBinarySemaphore = xSemaphoreCreateBinary();
    xRTCBinarySemaphore = xSemaphoreCreateBinary();

    lv_init();
    lv_port_disp_init();
    /* Disable Touch as it is not needed */
    // lv_port_indev_init();

#if LV_USE_LOG
    lv_log_register_print_cb(print_cb);
#endif

    LV_LOG("lvgl guider demo started\r\n");

    s_lvgl_initialized = true;

    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);

    /* Delete original display refresh timer */
    lv_display_delete_refr_timer(lv_display_get_default());
    PM_EnablePowerManager(true);

    for (;;)
    {
        clock_labels_update();
        MU_SendMsgNonBlocking(APP_MU, CHN_MU_REG_NUM, 0);
        lv_display_refr_timer(NULL);
        xSemaphoreTake(xMUBinarySemaphore, portMAX_DELAY);
        UpdateDisplayedTemperature(*(int32_t*)&result);
        APP_SetWakeupConfig();
        xSemaphoreTake(xRTCBinarySemaphore, portMAX_DELAY);
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    BaseType_t stat;

    /* Init board hardware. */
    BOARD_InitHardware();
    DEMO_InitUsTimer();

    BOARD_InitPowerConfig();
    CORE1_InitCore();

    /* MUA init */
    MU_Init(APP_MU);
    while (MU_GetFlags(APP_MU) != 0x1);
    BOARD_PowerConfigAfterCPU1Booted();

    APP_InitWakeupTimer();

    APP_InitTimeButtons();

    stat = xTaskCreate(AppTask, "lvgl", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);

    if (pdPASS != stat)
    {
        PRINTF("Failed to create lvgl task");
        while (1)
            ;
    }

    vTaskStartScheduler();

    for (;;)
    {
    } /* should never get here */
}

/*!
 * @brief Malloc failed hook.
 */
void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc failed. Increase the heap size.");

    for (;;)
        ;
}

/*!
 * @brief FreeRTOS tick hook.
 */
void vApplicationTickHook(void)
{
    if (s_lvgl_initialized)
    {
        lv_tick_inc(1);
    }
}

/*!
 * @brief Stack overflow hook.
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)pcTaskName;
    (void)xTask;

    for (;;)
        ;
}
