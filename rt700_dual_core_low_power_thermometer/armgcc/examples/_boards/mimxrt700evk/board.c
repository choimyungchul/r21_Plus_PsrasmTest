/*
 * Copyright 2023-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include "fsl_common.h"
#include "fsl_debug_console.h"
#if defined(MIMXRT798S_cm33_core0_SERIES)
#include "fsl_cache.h"
#endif
#include "fsl_clock.h"
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_lpi2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(MIMXRT798S_cm33_core0_SERIES)
#define HYPERRAM_CMD_LUT_SEQ_IDX_SYNC_READ   0
#define HYPERRAM_CMD_LUT_SEQ_IDX_SYNC_WRITE  1
#define HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ  2
#define HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE 3
#define HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ    4
#define HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE   5
#define HYPERRAM_CMD_LUT_SEQ_IDX_RESET       6

#define CUSTOM_LUT_LENGTH 40
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(MIMXRT798S_cm33_core0_SERIES)
static uint32_t i2c_iomux[2] = {0U};
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;

#if (defined(MIMXRT798S_cm33_core0_SERIES) || defined(MIMXRT798S_hifi4_SERIES) || defined(MIMXRT798S_ezhv_SERIES))
    CLOCK_AttachClk(BOARD_DEBUG_UART_FCCLK_ATTACH);
    CLOCK_SetClkDiv(BOARD_DEBUG_UART_FCCLK_DIV, 1U);

    /* attach FC0 clock to LP_FLEXCOMM (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
#else
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_SetClkDiv(BOARD_DEBUG_UART_CLK_DIV, 4U);
#endif

    uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}
#if defined(MIMXRT798S_cm33_core1_SERIES)
void BOARD_ClockPreConfig(void)
{
    CLOCK_AttachClk(kFRO1_DIV3_to_SENSE_BASE);
    CLOCK_SetClkDiv(kCLOCK_DivSenseMainClk, 1);
    CLOCK_AttachClk(kSENSE_BASE_to_SENSE_MAIN);
}

/*
 * NOTE, the actual output of the LDO may not exactly same with the setting due to accuracy and internal circuite voltage drop.
 * To make sure the minumum supply voltage meeting the rquirement of corresponding operation frequency, external supply is suggested.
 */
void BOARD_ClockHSRunPreConfig(void)
{
    BOARD_ClockPreConfig();

    /* Change power supply for LDO, if using external PMIC supply for VDD1/VDD2, need configure PMIC to change voltage supply. */
    power_regulator_voltage_t ldo = {
        .LDO.vsel0 = 700000U,  /* 700mv, 0.45 V + 12.5 mV * x */
        .LDO.vsel1 = 800000U,  /* 800mv*/
        .LDO.vsel2 = 900000U,  /* 900mv */
        .LDO.vsel3 = 1100000U, /* 1100mv */
    };

    power_lvd_voltage_t lvd = {
        .VDD12.lvl0 = 600000U,  /* 600mv */
        .VDD12.lvl1 = 700000U,  /* 700mv */
        .VDD12.lvl2 = 800000U,  /* 800mv */
        .VDD12.lvl3 = 1000000U, /* 1000mv */
    };

    POWER_ConfigRegulatorSetpoints(kRegulator_Vdd1LDO, &ldo, &lvd);
    POWER_SetRunRegulatorMode(kRegulator_Vdd1LDO, kPower_LDOMode_Bypass); /* Change to bypass mode. */

    POWER_ApplyPD();
}

void BOARD_ClockPostConfig(void)
{
}

void BOARD_ClockHSRunPostConfig(void)
{
}
#endif

#if defined(MIMXRT798S_cm33_core0_SERIES)
void BOARD_ClockPreConfig(void)
{
    POWER_DisablePD(kPDRUNCFG_PD_FRO1); /* Make sure FRO1 is enabled. */

    /* Switch to FRO1 for safe configure. */
    CLOCK_AttachClk(kFRO1_DIV1_to_COMPUTE_BASE);
    CLOCK_AttachClk(kCOMPUTE_BASE_to_COMPUTE_MAIN);
    CLOCK_SetClkDiv(kCLOCK_DivCmptMainClk, 1U);
    CLOCK_AttachClk(kFRO1_DIV1_to_RAM);
    CLOCK_SetClkDiv(kCLOCK_DivComputeRamClk, 1U);
    CLOCK_AttachClk(kFRO1_DIV1_to_COMMON_BASE);
    CLOCK_AttachClk(kCOMMON_BASE_to_COMMON_VDDN);
    CLOCK_SetClkDiv(kCLOCK_DivCommonVddnClk, 1U);

    BOARD_XspiClockSafeConfig(); /*Change to common_base clock(Sourced by FRO1). */
}

/*
 * NOTE, the actual output of the LDO may not exactly same with the setting due to accuracy and internal circuite voltage drop.
 * To make sure the minumum supply voltage meeting the rquirement of corresponding operation frequency, external supply is suggested.
 */
void BOARD_ClockHSRunPreConfig(void)
{
    BOARD_ClockPreConfig();

    /* Change power supply for LDO, if using external PMIC supply for VDD1/VDD2, need configure PMIC to change voltage supply. */
    power_regulator_voltage_t ldo = {
        .LDO.vsel0 = 700000U,  /* 700mv, 0.45 V + 12.5 mV * x */
        .LDO.vsel1 = 800000U,  /* 800mv*/
        .LDO.vsel2 = 900000U,  /* 900mv */
        .LDO.vsel3 = 1100000U, /* 1100mv */
    };

    power_lvd_voltage_t lvd = {
        .VDD12.lvl0 = 600000U,  /* 600mv */
        .VDD12.lvl1 = 700000U,  /* 700mv */
        .VDD12.lvl2 = 800000U,  /* 800mv */
        .VDD12.lvl3 = 1000000U, /* 1000mv */
    };

    POWER_ConfigRegulatorSetpoints(kRegulator_Vdd2LDO, &ldo, &lvd);
    POWER_SetRunRegulatorMode(kRegulator_Vdd2LDO, kPower_LDOMode_Bypass); /* Change to bypass mode. */

    POWER_ApplyPD();
}

void BOARD_ClockPostConfig(void)
{
    /* Call function BOARD_SetXspiClock() to set user configured clock source/divider for XSPI. */
    BOARD_SetXspiClock(XSPI0, 2U, 1U); /* FRO0 DIV1. */
}

void BOARD_ClockHSRunPostConfig(void)
{
    BOARD_ClockPostConfig();
}

AT_QUICKACCESS_SECTION_CODE(void BOARD_EnableXspiCache(CACHE64_CTRL_Type *cache))
{
    /* First, invalidate the entire cache. */
    cache->CCR |= CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_GO_MASK;
    while ((cache->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
    {
    }
    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    cache->CCR &= ~(CACHE64_CTRL_CCR_INVW0_MASK | CACHE64_CTRL_CCR_INVW1_MASK);
    /* Now enable the cache. */
    cache->CCR |= CACHE64_CTRL_CCR_ENCACHE_MASK;
}

AT_QUICKACCESS_SECTION_CODE(void BOARD_DisableXspiCache(CACHE64_CTRL_Type *cache))
{
    /* First, clean XSPI cache. */
    cache->CCR |= CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK | CACHE64_CTRL_CCR_GO_MASK;
    while ((cache->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
    {
    }
    /* As a precaution clear the bits to avoid inadvertently re-running this command. */
    cache->CCR &= ~(CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK);

    /* Now disable XSPI cache. */
    cache->CCR &= ~CACHE64_CTRL_CCR_ENCACHE_MASK;
}

void BOARD_ConfigMPU(void)
{
    uint8_t attr;
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t nonCacheSize  = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                                 0 :
                                 ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
    extern uint32_t __base_NCACHE_REGION;
    extern uint32_t __top_NCACHE_REGION;
    uint32_t nonCacheStart = (uint32_t)(&__base_NCACHE_REGION);
    uint32_t nonCacheSize  = (uint32_t)(&__top_NCACHE_REGION) - nonCacheStart;
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    extern uint32_t __NCACHE_BUFFERS_REGION_START[];
    extern uint32_t __NCACHE_BUFFERS_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t nonCacheSize  = (uint32_t)__NCACHE_REGION_SIZE;
    uint32_t nonCacheBuffersStart = (uint32_t)__NCACHE_BUFFERS_REGION_START;
    uint32_t nonCacheBuffersSize  = (uint32_t)__NCACHE_BUFFERS_REGION_SIZE;
#else
#error "Unsupported compiler"
#endif

    XCACHE_DisableCache(XCACHE0);
    XCACHE_DisableCache(XCACHE1);

    /* Disable MPU */
    ARM_MPU_Disable();

    /* Attr0: device memory. */
    ARM_MPU_SetMemAttr(0U, ARM_MPU_ATTR(ARM_MPU_ATTR_DEVICE, ARM_MPU_ATTR_DEVICE_nGnRnE));
    /* Attr1: non cacheable. */
    ARM_MPU_SetMemAttr(1U, ARM_MPU_ATTR(ARM_MPU_ATTR_NON_CACHEABLE, ARM_MPU_ATTR_NON_CACHEABLE));
    /* Attr2: non transient, write through, read allocate. */
    attr = ARM_MPU_ATTR_MEMORY_(0U, 0U, 1U, 0U);
    ARM_MPU_SetMemAttr(2U, ARM_MPU_ATTR(attr, attr));
    /* Attr3: non transient, write back, read/write allocate. */
    attr = ARM_MPU_ATTR_MEMORY_(0U, 1U, 1U, 1U);
    ARM_MPU_SetMemAttr(3U, ARM_MPU_ATTR(attr, attr));

    /* Region 0: [0x0, 0x1FFFFFFF], non-shareable, read/write, any privileged, executable. Attr 2 (write through). */
    ARM_MPU_SetRegion(0U, ARM_MPU_RBAR(0U, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x1FFFFFFFU, 2U));
    /* Region 2 (Peripherals): [0x40000000, 0x5FFFFFFF], non-shareable, read/write, non-privileged, executable. Attr 0
     * (device). */
    ARM_MPU_SetRegion(2U, ARM_MPU_RBAR(0x40000000U, ARM_MPU_SH_NON, 0U, 1U, 0U), ARM_MPU_RLAR(0x5FFFFFFF, 0U));

    if (nonCacheSize != 0)
    {
        /* The MPU region size should a granularity of 32 bytes. */
        assert((nonCacheSize & 0x1FU) == 0x0U);

        /* Region 1 setting : outter-shareable, read-write,  non-privileged, executable. Attr 1. (non-cacheable) */
        ARM_MPU_SetRegion(1U, ARM_MPU_RBAR(nonCacheStart, ARM_MPU_SH_OUTER, 0U, 1U, 0U),
                          ARM_MPU_RLAR(nonCacheStart + nonCacheSize - 1, 1U));
    }

    if (nonCacheBuffersSize != 0)
    {
        /* The MPU region size should a granularity of 32 bytes. */
        assert((nonCacheBuffersSize & 0x1FU) == 0x0U);
        ARM_MPU_SetRegion(3U, ARM_MPU_RBAR(nonCacheBuffersStart, ARM_MPU_SH_OUTER, 0U, 1U, 0U),
                          ARM_MPU_RLAR(nonCacheBuffersStart + nonCacheBuffersSize - 1, 1U));
    }

    /*
     * Enable MPU and HFNMIENA feature
     * HFNMIENA ensures the core uses MPU configuration when in hard fault, NMI, and FAULTMASK handlers,
     * otherwise all memory regions are accessed without MPU protection.
     */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);

    /* Enable code & system cache */
    XCACHE_EnableCache(XCACHE0);
    XCACHE_EnableCache(XCACHE1);

    /* flush pipeline */
    __DSB();
    __ISB();
}

/*!
 * @brief For A0 Silicon, the XSPI AHB Buffer need to be reconfigured to allow the PKC, eDMA1, GPU to access external
 * memory.
 * @param base XSPI peripheral base pointer
 */
AT_QUICKACCESS_SECTION_CODE(void BOARD_ReConfigXspiAhbBuffer(XSPI_Type *base))
{
    while (((base->SR & XSPI_SR_AHB_ACC_MASK) != 0U) && ((base->SR & XSPI_SR_AHB_ACC_MASK) != 0U))
    {
    }
    /* Configure Master ID to reserved master to not use buffer0-2. */
    base->BUFCR[0] = 0x13U;
    base->BUFCR[1] = 0x13U;
    base->BUFCR[2] = 0x13U;
}

void BOARD_DeinitXspi(XSPI_Type *base, CACHE64_CTRL_Type *cache)
{
    if (base == XSPI0)
    {
        /* Enable clock again. */
        CLKCTL0->PSCCTL1_SET = CLKCTL0_PSCCTL1_CLR_XSPI0_MASK;
    }
    else if (base == XSPI1)
    {
        /* Enable clock again. */
        CLKCTL0->PSCCTL1_SET = CLKCTL0_PSCCTL1_CLR_XSPI1_MASK;
    }
    else
    {
    }

    base->MCR &= ~XSPI_MCR_MDIS_MASK;
    if ((cache->CCR & CACHE64_CTRL_CCR_ENCACHE_MASK) != 0x00U)
    {
        BOARD_DisableXspiCache(cache);
    }
    /* Wait until XSPI is not busy */
    while ((base->SR & XSPI_SR_BUSY_MASK) != 0U)
    {
    }
    /* Disable module. */
    base->MCR |= XSPI_MCR_MDIS_MASK;
}

void BOARD_InitXspi(XSPI_Type *base, CACHE64_CTRL_Type *cache)
{
    /* Enable XSPI module */
    base->MCR |= XSPI_MCR_MDIS_MASK;

    base->MCR |= XSPI_MCR_SWRSTSD_MASK | XSPI_MCR_SWRSTHD_MASK;
    for (uint32_t i = 0U; i < 6U; i++)
    {
        __NOP();
    }
    base->MCR &= ~(XSPI_MCR_SWRSTSD_MASK | XSPI_MCR_SWRSTHD_MASK);
    base->MCR |= XSPI_MCR_IPS_TG_RST_MASK | XSPI_MCR_MDIS_MASK;
    base->MCR &= ~XSPI_MCR_ISD3FA_MASK;
    base->MCR &= ~XSPI_MCR_MDIS_MASK;
    base->MCR |= XSPI_MCR_MDIS_MASK;
    base->MCR |= XSPI_MCR_ISD3FA_MASK;
    base->MCR &= ~XSPI_MCR_MDIS_MASK;

    base->MCR |= XSPI_MCR_MDIS_MASK;
    base->SMPR = (((base->SMPR) & (~XSPI_SMPR_DLLFSMPFA_MASK)) |
                  XSPI_SMPR_DLLFSMPFA(FSL_FEATURE_XSPI_DLL_REF_VALUE_DDR_DELAY_TAP_NUM));
    base->MCR &= ~XSPI_MCR_MDIS_MASK;

    base->DLLCR[0] &= ~(XSPI_DLLCR_SLV_DLL_BYPASS_MASK | XSPI_DLLCR_DLL_CDL8_MASK | XSPI_DLLCR_SLV_DLY_OFFSET_MASK |
                        XSPI_DLLCR_SLV_FINE_OFFSET_MASK | XSPI_DLLCR_DLLRES_MASK | XSPI_DLLCR_DLL_REFCNTR_MASK |
                        XSPI_DLLCR_FREQEN_MASK);
    base->DLLCR[0] &= ~(XSPI_DLLCR_SLV_EN_MASK | XSPI_DLLCR_SLAVE_AUTO_UPDT_MASK | XSPI_DLLCR_DLLEN_MASK);
    /* Enable subordinate as auto update mode. */
    base->DLLCR[0] |= XSPI_DLLCR_SLV_EN_MASK | XSPI_DLLCR_SLAVE_AUTO_UPDT_MASK;
    /* program DLL to desired delay. */
    base->DLLCR[0] |= XSPI_DLLCR_DLLRES(FSL_FEATURE_XSPI_DLL_REF_VALUE_AUTOUPDATE_RES) |
                      XSPI_DLLCR_DLL_REFCNTR(2U) | XSPI_DLLCR_DLL_CDL8(1U) |
                      XSPI_DLLCR_SLV_FINE_OFFSET(0) | XSPI_DLLCR_SLV_DLY_OFFSET(0) | XSPI_DLLCR_FREQEN(1U);
    /* Load above settings into delay chain. */
    base->DLLCR[0] |= XSPI_DLLCR_SLV_UPD_MASK;
    base->DLLCR[0] |= XSPI_DLLCR_DLLEN_MASK;
    base->DLLCR[0] &= ~XSPI_DLLCR_SLV_UPD_MASK;

    while ((base->DLLSR & XSPI_DLLSR_SLVA_LOCK_MASK) == 0UL)
    {
    }

    if ((cache->CCR & CACHE64_CTRL_CCR_ENCACHE_MASK) == 0x00U)
    {
        BOARD_EnableXspiCache(cache);
        /* flush pipeline */
        __DSB();
        __ISB();
    }
}

/* BOARD_SetXspiClock run in RAM used to configure XSPI clock source and divider when XIP. */
void BOARD_SetXspiClock(XSPI_Type *base, uint32_t src, uint32_t divider)
{
    if (base == XSPI0)
    {
        if ((CLKCTL0->XSPI0FCLKSEL != CLKCTL0_XSPI0FCLKSEL_SEL(src)) ||
            ((CLKCTL0->XSPI0FCLKDIV & CLKCTL0_XSPI0FCLKDIV_DIV_MASK) != (divider - 1)))
        {
            /* Always deinit XSPI and init XSPI for the flash to make sure the flash works correctly after the
             XSPI root clock changed as the default XSPI configuration may does not work for the new root clock
             frequency. */
            BOARD_DeinitXspi(base, CACHE64_CTRL0);

            /* Disable clock before changing clock source */
            CLKCTL0->PSCCTL1_CLR = CLKCTL0_PSCCTL1_CLR_XSPI0_MASK;
            /* Update XSPI clock. */
            CLKCTL0->XSPI0FCLKSEL = CLKCTL0_XSPI0FCLKSEL_SEL(src) | CLKCTL0_XSPI0FCLKSEL_SEL_EN_MASK;
            CLKCTL0->XSPI0FCLKDIV = CLKCTL0_XSPI0FCLKDIV_DIV(divider - 1);
            while ((CLKCTL0->XSPI0FCLKDIV) & CLKCTL0_XSPI0FCLKDIV_REQFLAG_MASK)
            {
            }
            /* Enable XSPI clock again */
            CLKCTL0->PSCCTL1_SET = CLKCTL0_PSCCTL1_SET_XSPI0_MASK;

            BOARD_InitXspi(base, CACHE64_CTRL0);
        }
    }
    else if (base == XSPI1)
    {
        if ((CLKCTL0->XSPI1FCLKSEL != CLKCTL0_XSPI1FCLKSEL_SEL(src)) ||
            ((CLKCTL0->XSPI1FCLKDIV & CLKCTL0_XSPI1FCLKDIV_DIV_MASK) != (divider - 1)))
        {
            /* Always deinit XSPI and init XSPI for the flash to make sure the flash works correctly after the
             XSPI root clock changed as the default XSPI configuration may does not work for the new root clock
             frequency. */
            BOARD_DeinitXspi(base, CACHE64_CTRL1);

            /* Disable clock before changing clock source */
            CLKCTL0->PSCCTL1_CLR = CLKCTL0_PSCCTL1_CLR_XSPI1_MASK;
            /* Update XSPI clock. */
            CLKCTL0->XSPI1FCLKSEL = CLKCTL0_XSPI1FCLKSEL_SEL(src) | CLKCTL0_XSPI1FCLKSEL_SEL_EN_MASK;
            CLKCTL0->XSPI1FCLKDIV = CLKCTL0_XSPI1FCLKDIV_DIV(divider - 1);
            while ((CLKCTL0->XSPI1FCLKDIV) & CLKCTL0_XSPI1FCLKDIV_REQFLAG_MASK)
            {
            }
            /* Enable XSPI clock again */
            CLKCTL0->PSCCTL1_SET = CLKCTL0_PSCCTL1_SET_XSPI1_MASK;

            BOARD_InitXspi(base, CACHE64_CTRL1);
        }
    }
    else
    {
        return;
    }
}

void BOARD_XspiClockSafeConfig(void)
{
    BOARD_SetXspiClock(XSPI0, 0U, 1U);
    BOARD_SetXspiClock(XSPI1, 0U, 1U);

    /* For A0 Silicon, the XSPI AHB Buffer need to be reconfigured to allow the PKC, eDMA1, GPU to access external
     * memory.*/
    if (SYSCON3->SILICONREV_ID == 0xA0000UL)
    {
        BOARD_ReConfigXspiAhbBuffer(XSPI0);
        BOARD_ReConfigXspiAhbBuffer(XSPI1);
    }
}

static void xspi_hyper_ram_get_mcr(XSPI_Type *base, uint32_t regAddr, uint8_t *mrVal)
{
    xspi_transfer_t flashXfer = {0U};

    /* Read data */
    if (base == XSPI1)
    {
        flashXfer.deviceAddress = XSPI1_AMBA_BASE + regAddr;
    }
    else if (base == XSPI2)
    {
        flashXfer.deviceAddress = XSPI2_AMBA_BASE + regAddr;
    }
    flashXfer.cmdType         = kXSPI_Read;
    flashXfer.seqIndex        = HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = (uint32_t *)mrVal;
    flashXfer.dataSize        = 4;
    flashXfer.lockArbitration = false;

    (void)XSPI_TransferBlocking(base, &flashXfer);
}

static void xspi_hyper_ram_write_mcr(XSPI_Type *base, uint32_t regAddr, uint8_t *mrVal)
{
    xspi_transfer_t flashXfer = {0U};

    /* Write data */
    if (base == XSPI1)
    {
        flashXfer.deviceAddress = XSPI1_AMBA_BASE + regAddr;
    }
    else if (base == XSPI2)
    {
        flashXfer.deviceAddress = XSPI2_AMBA_BASE + regAddr;
    }
    flashXfer.cmdType         = kXSPI_Write;
    flashXfer.seqIndex        = HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE;
    flashXfer.targetGroup     = kXSPI_TargetGroup0;
    flashXfer.data            = (uint32_t *)mrVal;
    flashXfer.dataSize        = 4;
    flashXfer.lockArbitration = false;

    (void)XSPI_TransferBlocking(base, &flashXfer);
}

void BOARD_Init16bitsPsRam(XSPI_Type *base)
{
    xspi_config_t config;
    xspi_ahb_access_config_t psRamAhbAccessConfig;
    xspi_ip_access_config_t psRamIpAccessConfig;

    config.ptrAhbAccessConfig = &psRamAhbAccessConfig;
    config.ptrIpAccessConfig  = &psRamIpAccessConfig;

    /* clang-format off */
    /* 
     * Errata ERR052528: Limitation on LUT-Data Size < 8byte in xspi.
     * Description: Read command including RDSR command can't work if LUT data size in read status is less than 8.
     * Workaround: Use LUT data size of minimum 8 byte for read commands including RDSR.
    */
    const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
#if (defined(BOARD_PSRAM_ENABLE_VARIABLE_LATENCY) && BOARD_PSRAM_ENABLE_VARIABLE_LATENCY)
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xA0,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                    kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 6), /* Dummy cycle: 2 * 6 + 2 */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        /* Memory Write */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 0] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x20,
                                                                    kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                    kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 6), /* Dummy cycle: 2 * 6 + 2 */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x04,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0X0),

        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xE0,
                                                            kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 0x6), /* Dummy cycle: 2 * 6 + 2 */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08,
                                                                kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        /* Register write */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x60,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x02),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x0,
                                                                kXSPI_Command_STOP, kXSPI_1PAD, 0x0),
#else
        /* Memory Read */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xA0,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 13),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ + 2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        /* Memory Write */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x20,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 13), /* Dummy cycle: 13 + 1 */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x04,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0X0),

        /* Register Read */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xE0,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                    kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, 13),  /* Dummy cycle: 13 + 1 */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_READ + 2] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        /* Register write */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x60,
                                                                kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 1] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                                                                    kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x02),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 2] = XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x0,
                                                                    kXSPI_Command_STOP, kXSPI_1PAD, 0x0),
#endif
        /* reset */
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_RESET + 0]     = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xFF, kXSPI_Command_DDR, kXSPI_8PAD, 0xFF),
        [5 * HYPERRAM_CMD_LUT_SEQ_IDX_RESET + 1]     = XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_8PAD, 0x0, kXSPI_Command_STOP, kXSPI_8PAD, 0x0),
    };
    /* clang-format on */

    xspi_device_ddr_config_t psRamDdrConfig = {
        .ddrDataAlignedClk         = kXSPI_DDRDataAlignedWith2xInternalRefClk,
        .enableByteSwapInOctalMode = false,
        .enableDdr                 = true,
    };

    xspi_device_config_t psRamDeviceConfig = {
        .xspiRootClk                                = 500000000,      /*!< 500MHz */
        .enableCknPad                               = true,           /*!< CKN signal is connected in board. */
        .deviceInterface                            = kXSPI_HyperBus, /*!< HyperBus protocol. */
        .interfaceSettings.hyperBusSettings.x16Mode = kXSPI_x16ModeEnabledOnlyData, /*!< Only Data use x16 mode. */
#if (defined(BOARD_PSRAM_ENABLE_VARIABLE_LATENCY) && BOARD_PSRAM_ENABLE_VARIABLE_LATENCY)
        .interfaceSettings.hyperBusSettings.enableVariableLatency =
            true, /*!< Enable additional latency to increase performance. */
#else
         .interfaceSettings.hyperBusSettings.enableVariableLatency = false,
#endif
        .interfaceSettings.hyperBusSettings.forceBit10To1 = false,
        .interfaceSettings.hyperBusSettings.pageSize      = 1024,
        .CSHoldTime                                       = 2,
        .CSSetupTime                                      = 2,
        .sampleClkConfig.sampleClkSource                  = kXSPI_SampleClkFromExternalDQS,
        .sampleClkConfig.enableDQSLatency                 = false,
        .sampleClkConfig.dllConfig.dllMode                = kXSPI_AutoUpdateMode,
        .sampleClkConfig.dllConfig.useRefValue            = true,
        .sampleClkConfig.dllConfig.enableCdl8             = true,
        .addrMode                                         = kXSPI_Device2ByteAddressable,
        .columnAddrWidth                                  = 3U,
        .enableCASInterleaving                            = false,
        .deviceSize[0]                                    = 0x8000U,
        .deviceSize[1]      = 0x8000U, /*!< Single die device, so deviceSize1 should equal to deviceSize0. */
        .ptrDeviceRegInfo   = NULL,
        .ptrDeviceDdrConfig = &psRamDdrConfig,
    };
    /* Get XSPI default settings and configure the xspi. */
    XSPI_GetDefaultConfig(&config);
    if (base == XSPI2)
    {
        POWER_DisablePD(kPDRUNCFG_APD_XSPI2);
        POWER_DisablePD(kPDRUNCFG_PPD_XSPI2);
        POWER_ApplyPD();

        CLOCK_AttachClk(kMAIN_PLL_PFD3_to_XSPI2);
        CLOCK_SetClkDiv(kCLOCK_DivXspi2Clk, 1U);
#if (defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
        CLOCK_EnableClock(kCLOCK_Xspi2);
#endif
    }
    else
    {
        psRamDeviceConfig.xspiRootClk = 400000000;
        POWER_DisablePD(kPDRUNCFG_APD_XSPI1);
        POWER_DisablePD(kPDRUNCFG_PPD_XSPI1);
        POWER_ApplyPD();

        CLOCK_AttachClk(kAUDIO_PLL_PFD1_to_XSPI1);
        CLOCK_SetClkDiv(kCLOCK_DivXspi1Clk, 1U);
#if (defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
        CLOCK_EnableClock(kCLOCK_Xspi1);
#endif
    }

    xspi_ahb_write_config_t psRamAhbWriteConfig = {
        .AWRSeqIndex          = HYPERRAM_CMD_LUT_SEQ_IDX_BURST_WRITE,
        .blockRead            = false,
        .blockSequenceWrite   = false,
        .pageWaitTimeoutValue = 0UL,
        .policy               = kXSPI_SoftwareClearPPWFlag,
    };
    config.ptrAhbAccessConfig->ARDSeqIndex                 = HYPERRAM_CMD_LUT_SEQ_IDX_BURST_READ;
    config.ptrAhbAccessConfig->ahbErrorPayload.highPayload = 0x5A5A5A5A;
    config.ptrAhbAccessConfig->ahbErrorPayload.lowPayload  = 0x5A5A5A5A;
    config.ptrAhbAccessConfig->enableAHBBufferWriteFlush   = true;
    config.ptrAhbAccessConfig->enableAHBPrefetch           = true;
    config.ptrAhbAccessConfig->ptrAhbWriteConfig           = &psRamAhbWriteConfig;

    config.ptrIpAccessConfig->ptrSfpFradConfig = NULL;
    config.ptrIpAccessConfig->ptrSfpMdadConfig = NULL;

    XSPI_Init(base, &config);
    XSPI_SetDeviceConfig(base, &psRamDeviceConfig);

    /* Update LUT table. */
    XSPI_UpdateLUT(base, 0, customLUT, CUSTOM_LUT_LENGTH);

    uint16_t cr1Register[2] = {0x0, 0x0};

    /* Enable CLKn. */
    (void)xspi_hyper_ram_get_mcr(base, ((1U << 11) | (1U << 0)), (uint8_t *)cr1Register);
    cr1Register[1] &= ~(0x01 << 6); /* enable ckn */
    (void)xspi_hyper_ram_write_mcr(base, ((1U << 12) | (1U << 0)), (uint8_t *)cr1Register);

#if (defined(BOARD_PSRAM_ENABLE_VARIABLE_LATENCY) && BOARD_PSRAM_ENABLE_VARIABLE_LATENCY)
    /* Following code to enable variable latency for hyperRAM */
    uint16_t cr0Register[2] = {0x0U, 0x0U};

    (void)xspi_hyper_ram_get_mcr(base, (1U << 11), (uint8_t *)cr0Register);

    cr0Register[1] &= ~(1U << 3U);
    cr0Register[0] &= ~(0x07 << 4); /* Clear drive strength */
    cr0Register[0] |= (0x03 << 4);  /* 46Ohms */

    (void)xspi_hyper_ram_write_mcr(base, (1U << 11), (uint8_t *)cr0Register);
#endif
    /* Updated address mode for AHB access. */
    psRamDeviceConfig.addrMode = kXSPI_Device4ByteAddressable;
    XSPI_SetDeviceConfig(base, &psRamDeviceConfig);
}


#if 1
#define XSPI_PSRAM_CMD_LUT_SEQ_IDX_BURST_READ   0U
#define XSPI_PSRAM_CMD_LUT_SEQ_IDX_BURST_WRITE  1U
#define XSPI_PSRAM_CMD_LUT_SEQ_IDX_REG_READ     2U
#define XSPI_PSRAM_CMD_LUT_SEQ_IDX_REG_WRITE    3U
#define XSPI_PSRAM_CMD_LUT_SEQ_IDX_RESET        4U
#define XSPI_PSRAM_CUSTOM_LUT_LENGTH            (5U * 5U)
#define XSPI_PSRAM_FALLBACK_PAGE_SIZE_BYTES     2048UL
#define XSPI_PSRAM_FALLBACK_DEVICE_SIZE_KB      0x8000UL
#define XSPI_PSRAM_READ_DUMMY_CYCLES            4U /* Default LC=5 cycles. LUT operand is cycles - 1. */
#define XSPI_PSRAM_WRITE_DUMMY_CYCLES           4U /* Default WLC=5 cycles. LUT operand is cycles - 1. */
#define XSPI_PSRAM_PATTERN_CHUNK_WORDS          64U
#define XSPI_PSRAM_TEST_WORDS                   64U
#define XSPI_PSRAM_TEST_OFFSET                  0x00010000U
#define XSPI_PSRAM_STRESS_BYTES                 (64U * 1024U)
#define XSPI_PSRAM_TEST_SKIPPED                 ((status_t)0x5A5A0001U)

static status_t XSPI2_PSRAM_SendIpCommand(uint8_t seqIndex,
                                         xspi_command_type_t cmdType,
                                         uint32_t deviceAddress,
                                         uint32_t *data,
                                         size_t dataSize)
{
    xspi_transfer_t transfer = {0};

    transfer.deviceAddress  = deviceAddress;
    transfer.cmdType        = cmdType;
    transfer.seqIndex       = seqIndex;
    transfer.targetGroup    = kXSPI_TargetGroup0;
    transfer.data           = data;
    transfer.dataSize       = dataSize;
    transfer.lockArbitration = false;

    return XSPI_TransferBlocking(XSPI2, &transfer);
}

status_t XSPI2_PSRAM_Init(void)
{
    xspi_config_t xspiConfig;
    xspi_ahb_access_config_t ahbAccessConfig;
    xspi_ip_access_config_t ipAccessConfig;
    xspi_ahb_write_config_t ahbWriteConfig;
    xspi_device_ddr_config_t ddrConfig;
    xspi_device_config_t deviceConfig;
    status_t status;
    uint32_t xspiRootClkHz;
    const uint32_t customLUT[XSPI_PSRAM_CUSTOM_LUT_LENGTH] = {
        /* APS256XXN-OB9 Xccela linear burst read: 0x20. */
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_BURST_READ + 0U] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x20,
                                                                     kXSPI_Command_DDR, kXSPI_8PAD, 0x00),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_BURST_READ + 1U] = XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20,
                                                                     kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, XSPI_PSRAM_READ_DUMMY_CYCLES),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_BURST_READ + 2U] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x04,
                                                                     kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        /* APS256XXN-OB9 Xccela linear burst write: 0xA0. */
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 0U] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xA0,
                                                                     kXSPI_Command_DDR, kXSPI_8PAD, 0x00),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 1U] = XSPI_LUT_SEQ(kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x20,
                                                                     kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, XSPI_PSRAM_WRITE_DUMMY_CYCLES),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_BURST_WRITE + 2U] = XSPI_LUT_SEQ(kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x04,
                                                                     kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_REG_READ + 0U] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x40,
                         	 	 	 	 	 	 	 	   	   	   	    kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_REG_READ + 1U] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                          kXSPI_Command_DUMMY_SDR, kXSPI_8PAD, XSPI_PSRAM_READ_DUMMY_CYCLES),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_REG_READ + 2U] = XSPI_LUT_SEQ(kXSPI_Command_READ_DDR, kXSPI_8PAD, 0x08,
                         	 	 	 	 	 	 	 	 	 	 	 	kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 0U] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xC0,
                         	 	 	 	 	 	 	 	 	 	 	 	 kXSPI_Command_RADDR_DDR, kXSPI_8PAD, 0x18),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 1U] = XSPI_LUT_SEQ(kXSPI_Command_CADDR_DDR, kXSPI_8PAD, 0x10,
                         	 	 	 	 	 	 	 	 	 	 	 	 kXSPI_Command_WRITE_DDR, kXSPI_8PAD, 0x02),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_REG_WRITE + 2U] = XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x0,
                         	 	 	 	 	 	 	 	 	 	 	 	 kXSPI_Command_STOP, kXSPI_1PAD, 0x0),

        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_RESET + 0U] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0xFF,
                                                                     kXSPI_Command_DDR, kXSPI_8PAD, 0x00),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_RESET + 1U] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x00,
                                                                     kXSPI_Command_DDR, kXSPI_8PAD, 0x00),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_RESET + 2U] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x00,
                                                                     kXSPI_Command_DDR, kXSPI_8PAD, 0x00),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_RESET + 3U] = XSPI_LUT_SEQ(kXSPI_Command_DDR, kXSPI_8PAD, 0x00,
                                                                     kXSPI_Command_DDR, kXSPI_8PAD, 0x00),
        [5U * XSPI_PSRAM_CMD_LUT_SEQ_IDX_RESET + 4U] = XSPI_LUT_SEQ(kXSPI_Command_STOP, kXSPI_1PAD, 0x0,
                                                                     kXSPI_Command_STOP, kXSPI_1PAD, 0x0),
    };

    POWER_DisablePD(kPDRUNCFG_APD_XSPI2);
    POWER_DisablePD(kPDRUNCFG_PPD_XSPI2);
    POWER_ApplyPD();

    /* BOARD_InitPowerConfig() deinitializes MAIN_PLL, so XSPI2 must use a
     * clock source that remains active during the application. */
    CLOCK_AttachClk(kFRO0_DIV1_to_XSPI2);
    CLOCK_SetClkDiv(kCLOCK_DivXspi2Clk, 2U); /* 192 MHz / 2 = 96 MHz. */

    ahbWriteConfig.AWRSeqIndex          = XSPI_PSRAM_CMD_LUT_SEQ_IDX_BURST_WRITE;
    ahbWriteConfig.blockRead            = false;
    ahbWriteConfig.blockSequenceWrite   = false;
    ahbWriteConfig.pageWaitTimeoutValue = 0UL;
    ahbWriteConfig.policy               = kXSPI_SoftwareClearPPWFlag;

    xspiConfig.ptrAhbAccessConfig = &ahbAccessConfig;
    xspiConfig.ptrIpAccessConfig  = &ipAccessConfig;
    XSPI_GetDefaultConfig(&xspiConfig);

    xspiConfig.ptrAhbAccessConfig->ARDSeqIndex                 = XSPI_PSRAM_CMD_LUT_SEQ_IDX_BURST_READ;
    xspiConfig.ptrAhbAccessConfig->ahbErrorPayload.highPayload = 0x5A5A5A5AU;
    xspiConfig.ptrAhbAccessConfig->ahbErrorPayload.lowPayload  = 0x5A5A5A5AU;
    xspiConfig.ptrAhbAccessConfig->enableAHBBufferWriteFlush   = true;
    xspiConfig.ptrAhbAccessConfig->enableAHBPrefetch           = false;
    xspiConfig.ptrAhbAccessConfig->ahbSplitSize                = kXSPI_AhbSplitSize64b;
    xspiConfig.ptrAhbAccessConfig->ptrAhbWriteConfig           = &ahbWriteConfig;

    ddrConfig.ddrDataAlignedClk         = kXSPI_DDRDataAlignedWith2xInternalRefClk;
    ddrConfig.enableByteSwapInOctalMode = false;
    ddrConfig.enableDdr                 = true;

    memset(&deviceConfig, 0, sizeof(deviceConfig));
    xspiRootClkHz = CLOCK_GetFreq(kCLOCK_Xspi2Clk);
    if (xspiRootClkHz == 0U)
    {
        return kStatus_Fail;
    }
    deviceConfig.xspiRootClk                                   = xspiRootClkHz;
    deviceConfig.enableCknPad                                  = false;
    deviceConfig.deviceInterface                               = kXSPI_StrandardExtendedSPI;
    deviceConfig.interfaceSettings.strandardExtendedSPISettings.pageSize =
        XSPI_PSRAM_FALLBACK_PAGE_SIZE_BYTES;
    deviceConfig.CSHoldTime                      = 3U;
    deviceConfig.CSSetupTime                     = 3U;
    deviceConfig.sampleClkConfig.sampleClkSource = kXSPI_SampleClkFromExternalDQS;
    /* APS256XXN asserts DQS before the first valid read data. Enable the
     * controller latency compensation so the first byte is not discarded. */
    deviceConfig.sampleClkConfig.enableDQSLatency = false;
    deviceConfig.sampleClkConfig.dllConfig.dllMode = kXSPI_AutoUpdateMode;
    deviceConfig.sampleClkConfig.dllConfig.useRefValue = true;
    deviceConfig.sampleClkConfig.dllConfig.enableCdl8 = true;
    deviceConfig.addrMode                     = kXSPI_DeviceByteAddressable;
    /* The Xccela LUT sends the complete A3..A0 address through one 32-bit
     * RADDR instruction.  Keep CAS at zero so XSPI does not strip low address
     * bits for a separate CADDR phase. */
    deviceConfig.columnAddrWidth              = 0U;
    deviceConfig.enableCASInterleaving        = false;
    deviceConfig.deviceSize[0]                = XSPI_PSRAM_FALLBACK_DEVICE_SIZE_KB;
    deviceConfig.deviceSize[1]                = XSPI_PSRAM_FALLBACK_DEVICE_SIZE_KB;
    deviceConfig.ptrDeviceRegInfo             = NULL;
    deviceConfig.ptrDeviceDdrConfig           = &ddrConfig;

    XSPI_Init(XSPI2, &xspiConfig);
    status = XSPI_SetDeviceConfig(XSPI2, &deviceConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* APS256XXN uses the DQS pin as active-high DM during writes.  The SDK
     * clears DQS_OUT_EN for StandardExtendedSPI devices, which prevents AHB
     * byte/halfword strobes from reaching the PSRAM. */
    XSPI_EnableModule(XSPI2, false);
    XSPI2->MCR |= XSPI_MCR_DQS_OUT_EN_MASK;
    XSPI_EnableModule(XSPI2, true);

    XSPI_UpdateLUT(XSPI2, 0U, customLUT, XSPI_PSRAM_CUSTOM_LUT_LENGTH);

    /* The device requires at least 150 us for power-up self-initialization
     * before Global Reset, then at least 2 us before the next command. */
    SDK_DelayAtLeastUs(150U, SystemCoreClock);
    status = XSPI2_PSRAM_SendIpCommand(XSPI_PSRAM_CMD_LUT_SEQ_IDX_RESET, kXSPI_Command, XSPI2_AMBA_BASE, NULL, 0U);
    if (status != kStatus_Success)
    {
        return status;
    }

    SDK_DelayAtLeastUs(10U, SystemCoreClock);
    XSPI_ClearAhbBuffer(XSPI2);
    XSPI_ClearTxBuffer(XSPI2);
    XSPI_ClearRxBuffer(XSPI2);

    /* Frame-buffer consumers such as LCDIF/GPU access PSRAM as non-CPU AHB
     * masters. Route buffer 0-2 away from CPU-specific master IDs so those
     * masters can use the all-master AHB buffer. */
    BOARD_ReConfigXspiAhbBuffer(XSPI2);

    return kStatus_Success;
}

status_t XSPI2_PSRAM_MemoryTest(void)
{
    volatile uint32_t *testAddr = (volatile uint32_t *)(XSPI2_AMBA_BASE + XSPI_PSRAM_TEST_OFFSET);
    uint32_t backup[XSPI_PSRAM_TEST_WORDS];
    uint32_t expected;
    uint32_t actual;

    for (uint32_t i = 0U; i < XSPI_PSRAM_TEST_WORDS; i++)
    {
        backup[i] = testAddr[i];
    }

    for (uint32_t i = 0U; i < XSPI_PSRAM_TEST_WORDS; i++)
    {
        testAddr[i] = 0xA5A50000UL ^ (i * 0x01010101UL);
    }
    __DSB();
    XSPI_ClearAhbBuffer(XSPI2);

    for (uint32_t i = 0U; i < XSPI_PSRAM_TEST_WORDS; i++)
    {
        expected = 0xA5A50000UL ^ (i * 0x01010101UL);
        actual   = testAddr[i];
        if (actual != expected)
        {
            PRINTF("XSPI2 PSRAM test FAIL phase=pattern index=%u addr=0x%08X expected=0x%08X actual=0x%08X backup=0x%08X\r\n",
                   (unsigned int)i,
                   (unsigned int)&testAddr[i],
                   (unsigned int)expected,
                   (unsigned int)actual,
                   (unsigned int)backup[i]);
            for (uint32_t j = 0U; j < XSPI_PSRAM_TEST_WORDS; j++)
            {
                testAddr[j] = backup[j];
            }
            __DSB();
            XSPI_ClearAhbBuffer(XSPI2);
            return kStatus_Fail;
        }
    }

    for (uint32_t i = 0U; i < XSPI_PSRAM_TEST_WORDS; i++)
    {
        testAddr[i] = ~backup[i];
    }
    __DSB();
    XSPI_ClearAhbBuffer(XSPI2);

    for (uint32_t i = 0U; i < XSPI_PSRAM_TEST_WORDS; i++)
    {
        if (testAddr[i] != ~backup[i])
        {
            PRINTF("XSPI2 PSRAM test FAIL phase=invert index=%u addr=0x%08X expected=0x%08X actual=0x%08X backup=0x%08X\r\n",
                   (unsigned int)i,
                   (unsigned int)&testAddr[i],
                   (unsigned int)(~backup[i]),
                   (unsigned int)testAddr[i],
                   (unsigned int)backup[i]);
            for (uint32_t j = 0U; j < XSPI_PSRAM_TEST_WORDS; j++)
            {
                testAddr[j] = backup[j];
            }
            __DSB();
            XSPI_ClearAhbBuffer(XSPI2);
            return kStatus_Fail;
        }
    }

    for (uint32_t i = 0U; i < XSPI_PSRAM_TEST_WORDS; i++)
    {
        testAddr[i] = backup[i];
    }
    __DSB();
    XSPI_ClearAhbBuffer(XSPI2);

    return kStatus_Success;
}
#endif


inline static void i2c_release_bus_delay(void)
{
    SDK_DelayAtLeastUs(10U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

void BOARD_InitI2c2PinAsGpio(void)
{
    /* Reset IOPCTL0 module */
    RESET_ClearPeripheralReset(kIOPCTL0_RST_SHIFT_RSTn);

    /* PORT1 PIN11 is configured as PIO1_11 */
    i2c_iomux[0]        = IOPCTL0->PIO[1][11];
    IOPCTL0->PIO[1][11] = 0x440u; /* GPIO with inputbuffer and pseudo uutput drain enabled. */
    /* PORT1 PIN12 is configured as PIO1_12 */
    i2c_iomux[1]        = IOPCTL0->PIO[1][12];
    IOPCTL0->PIO[1][12] = 0x400U; /* GPIO with pseudo uutput drain enabled. */
}

void BOARD_RestoreI2c2PinMux(void)
{
    IOPCTL0->PIO[1][11] = i2c_iomux[0];
    IOPCTL0->PIO[1][12] = i2c_iomux[1];
}

void BOARD_I2c2RecoverBus(void)
{
    gpio_pin_config_t pin_config = {
        kGPIO_DigitalOutput,
        1U,
    };

    GPIO_PinInit(BOARD_CODEC_I2C_SCL_GPIO, BOARD_CODEC_I2C_SCL_PIN, &pin_config);
    i2c_release_bus_delay();
    
    /* Configure SDA pin as input. */
    pin_config.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(BOARD_CODEC_I2C_SDA_GPIO, BOARD_CODEC_I2C_SDA_PIN, &pin_config);

    /* Send pulses on SCL until SDA is released and then send stop. */
    while(true)
    {
        /* SCL pulse - low */
        GPIO_PinWrite(BOARD_CODEC_I2C_SCL_GPIO, BOARD_CODEC_I2C_SCL_PIN, 0U);
        i2c_release_bus_delay();

        /* Check whether SDA line is released */
        if (1U == GPIO_PinRead(BOARD_CODEC_I2C_SDA_GPIO, BOARD_CODEC_I2C_SDA_PIN))
        {
            /* SDA is released, hold it in low */
            pin_config.pinDirection = kGPIO_DigitalOutput;
            pin_config.outputLogic = 0U;
            GPIO_PinInit(BOARD_CODEC_I2C_SDA_GPIO, BOARD_CODEC_I2C_SDA_PIN, &pin_config);

            /* SCL pulse - high */
            GPIO_PinWrite(BOARD_CODEC_I2C_SCL_GPIO, BOARD_CODEC_I2C_SCL_PIN, 1U);
            i2c_release_bus_delay();

            /* Set SDA to high from low - send stop */
            GPIO_PinWrite(BOARD_CODEC_I2C_SDA_GPIO, BOARD_CODEC_I2C_SDA_PIN, 1U);
            i2c_release_bus_delay();

            break;
        }
        else
        {
            /* SCL pulse - high */
            GPIO_PinWrite(BOARD_CODEC_I2C_SCL_GPIO, BOARD_CODEC_I2C_SCL_PIN, 1U);
            i2c_release_bus_delay();
        }
    }
}

#endif /* MIMXRT798S_cm33_core0_SERIES */

#if defined(MIMXRT798S_cm33_core0_SERIES) || defined(MIMXRT798S_cm33_core1_SERIES)
void GlikeyWriteEnable(GLIKEY_Type *base, uint8_t idx)
{
    (void)GLIKEY_SyncReset(base);

    (void)GLIKEY_StartEnable(base, idx);
    (void)GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP1);
    (void)GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP2);
    (void)GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP3);
    (void)GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP_EN);
}

void GlikeyClearConfig(GLIKEY_Type *base)
{
    (void)GLIKEY_SyncReset(base);
}

/* Disable the secure check for AHBSC and enable periperhals/sram access for masters */
void BOARD_InitAHBSC(void)
{
#if defined(MIMXRT798S_cm33_core0_SERIES)
    GlikeyWriteEnable(GLIKEY0, 1U);
    AHBSC0->MISC_CTRL_DP_REG = 0x000086aa;
    AHBSC0->MISC_CTRL_REG    = 0x000086aa; /* AHBSC0 MISC_CTRL_REG, disable Privilege & Secure checking. */

    GlikeyWriteEnable(GLIKEY0, 7U);
    /* Enable arbiter0 accessing SRAM */
    AHBSC0->COMPUTE_ARB0RAM_ACCESS_ENABLE = 0x3FFFFFFF;
    AHBSC0->SENSE_ARB0RAM_ACCESS_ENABLE   = 0x3FFFFFFF;
    AHBSC0->MEDIA_ARB0RAM_ACCESS_ENABLE   = 0x3FFFFFFF;
    AHBSC0->NPU_ARB0RAM_ACCESS_ENABLE     = 0x3FFFFFFF;
    AHBSC0->HIFI4_ARB0RAM_ACCESS_ENABLE   = 0x3FFFFFFF;
#endif

    GlikeyWriteEnable(GLIKEY1, 1U);
    AHBSC3->MISC_CTRL_DP_REG = 0x000086aa;
    AHBSC3->MISC_CTRL_REG    = 0x000086aa; /* AHBSC3 MISC_CTRL_REG, disable Privilege & Secure checking.*/

    GlikeyWriteEnable(GLIKEY1, 9U);
    /* Enable arbiter1 accessing SRAM */
    AHBSC3->COMPUTE_ARB1RAM_ACCESS_ENABLE = 0x3FFFFFFF;
    AHBSC3->SENSE_ARB1RAM_ACCESS_ENABLE   = 0x3FFFFFFF;
    AHBSC3->MEDIA_ARB1RAM_ACCESS_ENABLE   = 0x3FFFFFFF;
    AHBSC3->NPU_ARB1RAM_ACCESS_ENABLE     = 0x3FFFFFFF;
    AHBSC3->HIFI4_ARB1RAM_ACCESS_ENABLE   = 0x3FFFFFFF;
    AHBSC3->HIFI1_ARB1RAM_ACCESS_ENABLE   = 0x3FFFFFFF;

    GlikeyWriteEnable(GLIKEY1, 8U);
    /* Access enable for COMPUTE domain masters to common APB peripherals.*/
    AHBSC3->COMPUTE_APB_PERIPHERAL_ACCESS_ENABLE = 0xffffffff;
    AHBSC3->SENSE_APB_PERIPHERAL_ACCESS_ENABLE   = 0xffffffff;
    GlikeyWriteEnable(GLIKEY1, 7U);
    AHBSC3->COMPUTE_AIPS_PERIPHERAL_ACCESS_ENABLE = 0xffffffff;
    AHBSC3->SENSE_AIPS_PERIPHERAL_ACCESS_ENABLE   = 0xffffffff;

    GlikeyWriteEnable(GLIKEY2, 1U);
    /*Disable secure and secure privilege checking. */
    AHBSC4->MISC_CTRL_DP_REG = 0x000086aa;
    AHBSC4->MISC_CTRL_REG    = 0x000086aa;

#if defined(MIMXRT798S_cm33_core0_SERIES)
    GlikeyClearConfig(GLIKEY0);
#endif
    GlikeyClearConfig(GLIKEY1);
    GlikeyClearConfig(GLIKEY2);
}
#endif /* MIMXRT798S_cm33_core0_SERIES || MIMXRT798S_cm33_core1_SERIES */

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_I2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz)
{
    lpi2c_master_config_t i2cConfig = {0};

    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    i2cConfig.debugEnable = true;
    LPI2C_MasterInit(base, &i2cConfig, clkSrc_Hz);
}

status_t BOARD_I2C_Send(LPI2C_Type *base,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subaddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize)
{
    lpi2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kLPI2C_Write;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data           = txBuff;
    masterXfer.dataSize       = txBuffSize;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(base, &masterXfer);
}

status_t BOARD_I2C_Receive(LPI2C_Type *base,
                           uint8_t deviceAddress,
                           uint32_t subAddress,
                           uint8_t subaddressSize,
                           uint8_t *rxBuff,
                           uint8_t rxBuffSize)
{
    lpi2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data           = rxBuff;
    masterXfer.dataSize       = rxBuffSize;
    masterXfer.direction      = kLPI2C_Read;
    masterXfer.flags          = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(base, &masterXfer);
}

void BOARD_PMIC_I2C_Init(void)
{
    BOARD_I2C_Init(BOARD_PMIC_I2C_BASEADDR, BOARD_PMIC_I2C_CLOCK_FREQ);
    LPI2C_MasterSetBaudRate(BOARD_PMIC_I2C_BASEADDR, BOARD_PMIC_I2C_CLOCK_FREQ, 2000000);
}

void BOARD_PMIC_I2C_Deinit(void)
{
    LPI2C_MasterDeinit(BOARD_PMIC_I2C_BASEADDR);
}

status_t BOARD_PMIC_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_I2C_Send(BOARD_PMIC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, (uint8_t *)txBuff,
                          txBuffSize);
}

status_t BOARD_PMIC_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_I2C_Receive(BOARD_PMIC_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff, rxBuffSize);
}

#if defined(MIMXRT798S_cm33_core0_SERIES)
void BOARD_MIPIPanelTouch_I2C_Init(void)
{
    CLOCK_AttachClk(kOSC_CLK_to_FCCLK0);
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM8);
    CLOCK_EnableClock(kCLOCK_LPFlexComm8);
    BOARD_I2C_Init(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, BOARD_MIPI_PANEL_TOUCH_I2C_CLOCK_FREQ);
}

status_t BOARD_MIPIPanelTouch_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize)
{
    return BOARD_I2C_Send(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize,
                          (uint8_t *)txBuff, txBuffSize);
}

status_t BOARD_MIPIPanelTouch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_I2C_Receive(BOARD_MIPI_PANEL_TOUCH_I2C_BASEADDR, deviceAddress, subAddress, subAddressSize, rxBuff,
                             rxBuffSize);
}
#endif
#endif
