/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "power_management.h"
#include "pmic_support.h"
#include "fsl_mipi_dsi.h"
#include "fsl_pm_device.h"
#include "fsl_pm_core.h"


static void BOARD_DisableCache(CACHE64_CTRL_Type *base)
{
    if ((base->CCR & CACHE64_CTRL_CCR_ENCACHE_MASK) == CACHE64_CTRL_CCR_ENCACHE_MASK)
    {
        /* First, push any modified contents. */
        base->CCR |= CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK | CACHE64_CTRL_CCR_GO_MASK;

        /* Wait until the cache command completes. */
        while ((base->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0x00U)
        {
        }

        /* As a precaution clear the bits to avoid inadvertently re-running this command. */
        base->CCR &= ~(CACHE64_CTRL_CCR_PUSHW0_MASK | CACHE64_CTRL_CCR_PUSHW1_MASK);

        /* Now disable the cache. */
        base->CCR &= ~CACHE64_CTRL_CCR_ENCACHE_MASK;
    }
}

status_t BOARD_DisableXSPI1()
{
    status_t ret = kStatus_Fail;
    BOARD_DisableCache(CACHE64_CTRL1);
    CLOCK_DisableClock(kCLOCK_Cache64ctrl1);
    CLOCK_DisableClock(kCLOCK_Xspi1);
    CLOCK_AttachClk(kNONE_to_XSPI1);
    POWER_EnablePD(kPDRUNCFG_APD_XSPI1);
    POWER_EnablePD(kPDRUNCFG_PPD_XSPI1);
    POWER_ApplyPD();
    ret = kStatus_Success;
    return ret;
}

void BOARD_ConfigPMICModes(pca9422_modecfg_t *cfg, pca9422_power_mode_t mode)
{
    assert(cfg);

    switch (mode)
    {
        case kPCA9422_ActiveModeDVS0:
        case kPCA9422_ActiveModeDVS1:
        case kPCA9422_ActiveModeDVS2:
        case kPCA9422_ActiveModeDVS3:
        case kPCA9422_ActiveModeDVS4:
        case kPCA9422_ActiveModeDVS5:
        case kPCA9422_ActiveModeDVS6:
        case kPCA9422_ActiveModeDVS7:
            cfg[mode].sw1OutVolt  = 1000000U; /* VDD2 */
            cfg[mode].sw2OutVolt  = 1000000U; /* VDDN */
            cfg[mode].sw3OutVolt  = 1000000U; /* VDD1 */
            cfg[mode].sw4OutVolt  = 1800000U;
            cfg[mode].ldo1OutVolt = 1800000U; /* 1V8 AO */
            cfg[mode].ldo2OutVolt = 1800000U; /* VDDA1V8 */
            cfg[mode].ldo3OutVolt = 1200000U; /* VDD1V2_EUSB */
            cfg[mode].ldo4OutVolt = 3300000U; /* VDD3V3_USB, ADC3V3 */
            break;

        /* PMIC_MODE[1:0] = 01b */
        case kPCA9422_SleepMode:
            cfg[mode].sw1OutVolt  = 500000U;
            cfg[mode].sw2OutVolt  = 500000U;
            cfg[mode].sw3OutVolt  = 500000U;
            cfg[mode].sw4OutVolt  = 1800000U;
            cfg[mode].ldo1OutVolt = 1800000U;
            cfg[mode].ldo2OutVolt = 1800000U;
            cfg[mode].ldo3OutVolt = 1200000U;
            cfg[mode].ldo4OutVolt = 3300000U;
            break;

        /* Note: the StandbyMode and DPStandbyMode use same register for voltage configuration. */
        case kPCA9422_StandbyMode: /* PMIC_MODE[1:0] = 10b */
        case kPCA9422_DPStandbyMode: /* PMIC_MODE[1:0] = 11b */
            cfg[mode].sw1OutVolt  = 500000U;
            cfg[mode].sw2OutVolt  = 600000U;
            cfg[mode].sw3OutVolt  = 500000U;
            cfg[mode].sw4OutVolt  = 1800000U;
            cfg[mode].ldo1OutVolt = 1800000U;
            cfg[mode].ldo2OutVolt = 1800000U;
            cfg[mode].ldo3OutVolt = 1200000U;
            cfg[mode].ldo4OutVolt = 3300000U;
            break;

        default:
            break;
    }
}

/* Configure regulator output enable in Run mode. */
void BOARD_ConfigPMICRegEnable(pca9422_handle_t *handle)
{
    pca9422_regulatoren_t cfg;

    /* Configure Regulator Enable */
    PCA9422_GetDefaultRegEnableConfig(&cfg);

    /* All regulators enable in RUN state. */
    cfg.sw2Enable = true;
#if (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
    cfg.sw1Enable = true;
    cfg.sw3Enable = true;
#else /* VDD1, VDD2 are supplied by internal LDO. */
    cfg.sw1Enable    = false;
    cfg.sw3Enable    = false;
#endif
    cfg.sw4Enable  = true;
    cfg.ldo1Enable = true;
    cfg.ldo2Enable = true;
    cfg.ldo3Enable = true;
    cfg.ldo4Enable = true;

    PCA9422_WriteRegEnableConfig(handle, cfg);
}

void BOARD_ConfigPMICEnMode(pca9422_handle_t *handle)
{
    pca9422_enmodecfg_t cfg;
    /* Configure ENMODE */
    PCA9422_GetDefaultEnModeConfig(&cfg);

#if (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
    cfg.sw1OutEnMode = kPCA9422_EnmodeOnActiveSleep;
    cfg.sw3OutEnMode = kPCA9422_EnmodeOnActiveSleep;
#else
    cfg.sw1OutEnMode = kPCA9422_EnmodeOnActive;
    cfg.sw3OutEnMode = kPCA9422_EnmodeOnActive;
#endif
    cfg.sw2OutEnMode  = kPCA9422_EnmodeOnActiveSleep;
    cfg.sw4OutEnMode  = kPCA9422_EnmodeOnActiveSleepStandby;
    cfg.ldo1OutEnMode = kPCA9422_EnmodeOnAll;
    cfg.ldo2OutEnMode = kPCA9422_EnmodeOnActiveSleep;
    cfg.ldo3OutEnMode = kPCA9422_EnmodeOnActiveSleep;
    cfg.ldo4OutEnMode = kPCA9422_EnmodeOnActiveSleep;

    PCA9422_WriteEnModeConfig(handle, cfg);
}

void BOARD_DisableUnusedClocks(void)
{
    // CLOCK_DisableClock(kCLOCK_Xcache1);
    // CLOCK_DisableClock(kCLOCK_Xcache0);
    CLOCK_DisableClock(kCLOCK_Ocotp0);
    // CLOCK_DisableClock(kCLOCK_Sleepcon0);
    // CLOCK_DisableClock(kCLOCK_Syscon0);
    CLOCK_DisableClock(kCLOCK_Glikey0);
    CLOCK_DisableClock(kCLOCK_Glikey3);
    CLOCK_DisableClock(kCLOCK_TpiuTraceClkin);
    CLOCK_DisableClock(kCLOCK_SWOTraceClkin);
    CLOCK_DisableClock(kCLOCK_Tsclk);
    CLOCK_DisableClock(kCLOCK_Dma0);
    CLOCK_DisableClock(kCLOCK_Dma1);
    CLOCK_DisableClock(kCLOCK_PkcRam);
    CLOCK_DisableClock(kCLOCK_Pkc);
    CLOCK_DisableClock(kCLOCK_Romcp);
    // CLOCK_DisableClock(kCLOCK_Xspi0);
    CLOCK_DisableClock(kCLOCK_Xspi1);
    CLOCK_DisableClock(kCLOCK_Cache64ctrl0);
    CLOCK_DisableClock(kCLOCK_Cache64ctrl1);
    CLOCK_DisableClock(kCLOCK_Puf);
    CLOCK_DisableClock(kCLOCK_Mmu0);
    CLOCK_DisableClock(kCLOCK_Mmu1);
    CLOCK_DisableClock(kCLOCK_Gpio0);
    CLOCK_DisableClock(kCLOCK_Gpio1);
    CLOCK_DisableClock(kCLOCK_Gpio2);
    CLOCK_DisableClock(kCLOCK_Gpio3);
    CLOCK_DisableClock(kCLOCK_Gpio4);
    CLOCK_DisableClock(kCLOCK_Gpio5);
    CLOCK_DisableClock(kCLOCK_Gpio6);
    // CLOCK_DisableClock(kCLOCK_Gpio7);
    CLOCK_DisableClock(kCLOCK_Sct0);
    CLOCK_DisableClock(kCLOCK_Cdog0);
    CLOCK_DisableClock(kCLOCK_Cdog1);
    CLOCK_DisableClock(kCLOCK_Cdog2);
    // CLOCK_DisableClock(kCLOCK_LPFlexComm0);
    CLOCK_DisableClock(kCLOCK_LPFlexComm1);
    // CLOCK_DisableClock(kCLOCK_LPUart0);
    CLOCK_DisableClock(kCLOCK_LPUart1);
    // CLOCK_DisableClock(kCLOCK_LPI2c0);
    CLOCK_DisableClock(kCLOCK_LPI2c1);
    // CLOCK_DisableClock(kCLOCK_LPSpi0);
    CLOCK_DisableClock(kCLOCK_LPSpi1);
    CLOCK_DisableClock(kCLOCK_LPFlexComm2);
    CLOCK_DisableClock(kCLOCK_LPFlexComm3);
    CLOCK_DisableClock(kCLOCK_LPFlexComm4);
    CLOCK_DisableClock(kCLOCK_LPFlexComm5);
    CLOCK_DisableClock(kCLOCK_LPFlexComm6);
    CLOCK_DisableClock(kCLOCK_LPFlexComm7);
    CLOCK_DisableClock(kCLOCK_LPFlexComm8);
    CLOCK_DisableClock(kCLOCK_LPFlexComm9);
    CLOCK_DisableClock(kCLOCK_LPFlexComm10);
    CLOCK_DisableClock(kCLOCK_LPFlexComm11);
    CLOCK_DisableClock(kCLOCK_LPFlexComm12);
    CLOCK_DisableClock(kCLOCK_LPFlexComm13);
    CLOCK_DisableClock(kCLOCK_LPUart2);
    CLOCK_DisableClock(kCLOCK_LPUart3);
    CLOCK_DisableClock(kCLOCK_LPUart4);
    CLOCK_DisableClock(kCLOCK_LPUart5);
    CLOCK_DisableClock(kCLOCK_LPUart6);
    CLOCK_DisableClock(kCLOCK_LPUart7);
    CLOCK_DisableClock(kCLOCK_LPUart8);
    CLOCK_DisableClock(kCLOCK_LPUart9);
    CLOCK_DisableClock(kCLOCK_LPUart10);
    CLOCK_DisableClock(kCLOCK_LPUart11);
    CLOCK_DisableClock(kCLOCK_LPUart12);
    CLOCK_DisableClock(kCLOCK_LPUart13);
    CLOCK_DisableClock(kCLOCK_LPI2c2);
    CLOCK_DisableClock(kCLOCK_LPI2c3);
    CLOCK_DisableClock(kCLOCK_LPI2c4);
    CLOCK_DisableClock(kCLOCK_LPI2c5);
    CLOCK_DisableClock(kCLOCK_LPI2c6);
    CLOCK_DisableClock(kCLOCK_LPI2c7);
    CLOCK_DisableClock(kCLOCK_LPI2c8);
    CLOCK_DisableClock(kCLOCK_LPI2c9);
    CLOCK_DisableClock(kCLOCK_LPI2c10);
    CLOCK_DisableClock(kCLOCK_LPI2c11);
    CLOCK_DisableClock(kCLOCK_LPI2c12);
    CLOCK_DisableClock(kCLOCK_LPI2c13);
    CLOCK_DisableClock(kCLOCK_LPSpi2);
    CLOCK_DisableClock(kCLOCK_LPSpi3);
    CLOCK_DisableClock(kCLOCK_LPSpi4);
    CLOCK_DisableClock(kCLOCK_LPSpi5);
    CLOCK_DisableClock(kCLOCK_LPSpi6);
    CLOCK_DisableClock(kCLOCK_LPSpi7);
    CLOCK_DisableClock(kCLOCK_LPSpi8);
    CLOCK_DisableClock(kCLOCK_LPSpi9);
    CLOCK_DisableClock(kCLOCK_LPSpi10);
    CLOCK_DisableClock(kCLOCK_LPSpi11);
    CLOCK_DisableClock(kCLOCK_LPSpi12);
    CLOCK_DisableClock(kCLOCK_LPSpi13);
    CLOCK_DisableClock(kCLOCK_Sai0);
    CLOCK_DisableClock(kCLOCK_Sai1);
    CLOCK_DisableClock(kCLOCK_Sai2);
    CLOCK_DisableClock(kCLOCK_I3c0);
    CLOCK_DisableClock(kCLOCK_I3c1);
    CLOCK_DisableClock(kCLOCK_Crc0);
    CLOCK_DisableClock(kCLOCK_Wwdt0);
    CLOCK_DisableClock(kCLOCK_Wwdt1);
    CLOCK_DisableClock(kCLOCK_Ct32b0);
    CLOCK_DisableClock(kCLOCK_Ct32b1);
    CLOCK_DisableClock(kCLOCK_Ct32b2);
    CLOCK_DisableClock(kCLOCK_Ct32b3);
    CLOCK_DisableClock(kCLOCK_Ct32b4);
    CLOCK_DisableClock(kCLOCK_Mrt0);
    CLOCK_DisableClock(kCLOCK_Utick0);
    CLOCK_DisableClock(kCLOCK_Sema424);
    CLOCK_DisableClock(kCLOCK_Mu4);
    // CLOCK_DisableClock(kCLOCK_Pint0);
    CLOCK_DisableClock(kCLOCK_Pvts0);
    CLOCK_DisableClock(kCLOCK_InputMux0);
    CLOCK_DisableClock(kCLOCK_Freqme0);
    CLOCK_DisableClock(kCLOCK_SafoSgi);
    CLOCK_DisableClock(kCLOCK_Trace);
    CLOCK_DisableClock(kCLOCK_Prince0);
    CLOCK_DisableClock(kCLOCK_Prince1);
    CLOCK_DisableClock(kCLOCK_PrinceExe);
    CLOCK_DisableClock(kCLOCK_Syspm0);
    CLOCK_DisableClock(kCLOCK_Syspm1);
    CLOCK_DisableClock(kCLOCK_Hifi4);
    CLOCK_DisableClock(kCLOCK_Npu0);
    // CLOCK_DisableClock(kCLOCK_CompAccessRamArbiter1);
    CLOCK_DisableClock(kCLOCK_Iopctl0);
    CLOCK_DisableClock(kCLOCK_Hifi4AccessRamArbiter1);
    // CLOCK_DisableClock(kCLOCK_MediaAccessRamArbiter0);
    CLOCK_DisableClock(kCLOCK_Syscon2);
    CLOCK_DisableClock(kCLOCK_Iopctl2);
    CLOCK_DisableClock(kCLOCK_Cpu1);
    CLOCK_DisableClock(kCLOCK_Mu0);
    // CLOCK_DisableClock(kCLOCK_Mu1);
    CLOCK_DisableClock(kCLOCK_Mu2);
    // CLOCK_DisableClock(kCLOCK_OsTimer);
    // CLOCK_DisableClock(kCLOCK_Sema420);
    CLOCK_DisableClock(kCLOCK_Sdadc0);
    CLOCK_DisableClock(kCLOCK_Adc0);
    CLOCK_DisableClock(kCLOCK_Acmp0);
    CLOCK_DisableClock(kCLOCK_Pdm);
    CLOCK_DisableClock(kCLOCK_Glikey4);
    
#if (DEMO_POWER_ENABLE_DEBUG == 0U)
    CLOCK_DisableClock(kCLOCK_Dbg);
#endif
    // CLOCK_DisableClock(kCLOCK_Syscon3);
    CLOCK_DisableClock(kCLOCK_Iopctl1);
    CLOCK_DisableClock(kCLOCK_Glikey1);
    // CLOCK_DisableClock(kCLOCK_LPI2c15);
    CLOCK_DisableClock(kCLOCK_MediaAccessRamArbiter1);
    CLOCK_DisableClock(kCLOCK_Axi0);
    // CLOCK_DisableClock(kCLOCK_Gpu);
    // CLOCK_DisableClock(kCLOCK_MipiDsiCtrl);
    CLOCK_DisableClock(kCLOCK_LPSpi16);
    CLOCK_DisableClock(kCLOCK_LPSpi14);
    CLOCK_DisableClock(kCLOCK_Xspi2);
    CLOCK_DisableClock(kCLOCK_Mmu2);
    CLOCK_DisableClock(kCLOCK_Glikey5);
    CLOCK_DisableClock(kCLOCK_Flexio);
    // CLOCK_DisableClock(kCLOCK_Lcdif);
    CLOCK_DisableClock(kCLOCK_Syscon4);
    CLOCK_DisableClock(kCLOCK_JpgDecoder);
    CLOCK_DisableClock(kCLOCK_PngDecoder);
    CLOCK_DisableClock(kCLOCK_Ezhv);
    CLOCK_DisableClock(kCLOCK_AxbsEzh);
    CLOCK_DisableClock(kCLOCK_Glikey2);
    CLOCK_DisableClock(kCLOCK_Usb0);
    CLOCK_DisableClock(kCLOCK_Usb1);
    CLOCK_DisableClock(kCLOCK_Usdhc0);
    CLOCK_DisableClock(kCLOCK_Usdhc1);
    CLOCK_DisableClock(kCLOCK_Gdet0Ref);
    CLOCK_DisableClock(kCLOCK_TrngRef);
    CLOCK_DisableClock(kCLOCK_Els);
    CLOCK_DisableClock(kCLOCK_ItrcRef);
    CLOCK_DisableClock(kCLOCK_Gdet0);
    CLOCK_DisableClock(kCLOCK_Gdet3Ref);
    // CLOCK_DisableClock(kCLOCK_Rtc);
    CLOCK_DisableClock(kCLOCK_Gdet3);
    CLOCK_DisableClock(kCLOCK_UsbphyRef);
}

void BOARD_DisableUnusedModules(void)
{
    /* Disable unused modules. */
    // POWER_EnablePD(kPDRUNCFG_SHUT_COMPT_MAINCLK);
    // POWER_EnablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK);
    // POWER_EnablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);
    // POWER_EnablePD(kPDRUNCFG_SHUT_RAM0_CLK);
    // POWER_EnablePD(kPDRUNCFG_SHUT_RAM1_CLK);
    // POWER_EnablePD(kPDRUNCFG_SHUT_COMNN_MAINCLK);
    // POWER_EnablePD(kPDRUNCFG_SHUT_MEDIA_MAINCLK);
    // POWER_EnablePD(kPDRUNCFG_PD_SYSXTAL);
    // POWER_EnablePD(kPDRUNCFG_PD_FRO0);
    POWER_EnablePD(kPDRUNCFG_PD_FRO1);
    // POWER_EnablePD(kPDRUNCFG_PD_FRO2);
    // POWER_EnablePD(kPDRUNCFG_PD_LPOSC);
    POWER_EnablePD(kPDRUNCFG_PD_PLLANA);
    POWER_EnablePD(kPDRUNCFG_PD_PLLLDO);
    POWER_EnablePD(kPDRUNCFG_PD_AUDPLLANA);
    POWER_EnablePD(kPDRUNCFG_PD_AUDPLLLDO);
    POWER_EnablePD(kPDRUNCFG_PD_ADC0);
    // POWER_EnablePD(kPDRUNCFG_GATE_FRO0);
    POWER_EnablePD(kPDRUNCFG_GATE_FRO2);
    // POWER_EnablePD(kPDRUNCFG_DSR_VDD2N_MEDIA);
    // POWER_EnablePD(kPDRUNCFG_DSR_VDDN_COM);
    POWER_EnablePD(kPDRUNCFG_PD_VDD2_DSP);
    // POWER_EnablePD(kPDRUNCFG_PD_VDD2_MIPI);
    // POWER_EnablePD(kPDRUNCFG_PD_RBB_VDD1);
    // POWER_EnablePD(kPDRUNCFG_PD_AFBB_VDD1);
    // POWER_EnablePD(kPDRUNCFG_PD_RBB_VDD2);
    // POWER_EnablePD(kPDRUNCFG_PD_AFBB_VDD2);
    // POWER_EnablePD(kPDRUNCFG_PD_RBB_VDDN);
    // POWER_EnablePD(kPDRUNCFG_PD_AFBB_VDDN);
    // POWER_EnablePD(kPDRUNCFG_PD_SRAM_RBB_VDD1);
    // POWER_EnablePD(kPDRUNCFG_PD_SRAM_RBB_VDD2);
    // POWER_EnablePD(kPDRUNCFG_PD_SRAM_AFBB_VDD2);
    POWER_EnablePD(kPDRUNCFG_PD_PMC_TEMPSNS);
    POWER_EnablePD(kPDRUNCFG_LP_PMCREF);
    POWER_EnablePD(kPDRUNCFG_PD_HVD1V8);
    POWER_EnablePD(kPDRUNCFG_LP_POR_VDD1);
    POWER_EnablePD(kPDRUNCFG_LP_LVD_VDD1);
    POWER_EnablePD(kPDRUNCFG_PD_HVD_VDD1);
    POWER_EnablePD(kPDRUNCFG_PD_AGDET_VDD1);
    POWER_EnablePD(kPDRUNCFG_LP_POR_VDD2);
    POWER_EnablePD(kPDRUNCFG_LP_LVD_VDD2);
    POWER_EnablePD(kPDRUNCFG_PD_HVD_VDD2);
    POWER_EnablePD(kPDRUNCFG_PD_AGDET_VDD2);
    POWER_EnablePD(kPDRUNCFG_LP_POR_VDDN);
    POWER_EnablePD(kPDRUNCFG_LP_LVD_VDDN);
    POWER_EnablePD(kPDRUNCFG_PD_HVD_VDDN);
    POWER_EnablePD(kPDRUNCFG_PD_OTP);
    POWER_EnablePD(kPDRUNCFG_PD_ROM);

    POWER_EnablePD(kPDRUNCFG_APD_SRAM1);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM2);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM3);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM4);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM5);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM6);
    POWER_DisablePD(kPDRUNCFG_APD_SRAM7);
    POWER_DisablePD(kPDRUNCFG_APD_SRAM8);
    POWER_DisablePD(kPDRUNCFG_APD_SRAM9);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM10);
    POWER_DisablePD(kPDRUNCFG_APD_SRAM11);
    POWER_DisablePD(kPDRUNCFG_APD_SRAM12);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM13);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM14);
    POWER_DisablePD(kPDRUNCFG_APD_SRAM15);
    POWER_DisablePD(kPDRUNCFG_APD_SRAM16);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM17);

    POWER_EnablePD(kPDRUNCFG_APD_SRAM18);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM19);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM20);
    POWER_DisablePD(kPDRUNCFG_APD_SRAM21);
    POWER_DisablePD(kPDRUNCFG_APD_SRAM22);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM23);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM24);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM25);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM26);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM27);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM28);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM29);

    POWER_EnablePD(kPDRUNCFG_PPD_SRAM1);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM2);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM3);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM4);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM5);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM6);
    POWER_DisablePD(kPDRUNCFG_PPD_SRAM7);
    POWER_DisablePD(kPDRUNCFG_PPD_SRAM8);
    POWER_DisablePD(kPDRUNCFG_PPD_SRAM9);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM10);
    POWER_DisablePD(kPDRUNCFG_PPD_SRAM11);
    POWER_DisablePD(kPDRUNCFG_PPD_SRAM12);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM13);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM14);
    POWER_DisablePD(kPDRUNCFG_PPD_SRAM15);
    POWER_DisablePD(kPDRUNCFG_PPD_SRAM16);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM17);

    POWER_EnablePD(kPDRUNCFG_PPD_SRAM18);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM19);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM20);
    POWER_DisablePD(kPDRUNCFG_PPD_SRAM21);
    POWER_DisablePD(kPDRUNCFG_PPD_SRAM22);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM23);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM24);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM25);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM26);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM27);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM28);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM29);

    POWER_EnablePD(kPDRUNCFG_APD_SDHC0_SRAM);
    POWER_EnablePD(kPDRUNCFG_APD_SDHC1_SRAM);
    POWER_EnablePD(kPDRUNCFG_APD_USB0_SRAM);
    POWER_EnablePD(kPDRUNCFG_APD_USB1_SRAM);
    POWER_EnablePD(kPDRUNCFG_APD_JPEGDEC);
    POWER_EnablePD(kPDRUNCFG_APD_PNGDEC);
    // POWER_EnablePD(kPDRUNCFG_APD_MIPIDSI);
    // POWER_EnablePD(kPDRUNCFG_APD_GPU);
    POWER_EnablePD(kPDRUNCFG_APD_DMA2_3);
    POWER_EnablePD(kPDRUNCFG_APD_DMA0_1_PKC_ETF);
    // POWER_EnablePD(kPDRUNCFG_APD_CPU0_CODE_CACHE);
    // POWER_EnablePD(kPDRUNCFG_APD_CPU0_SYSTEM_CACHE);
    POWER_EnablePD(kPDRUNCFG_APD_DSP_ICACHE);
    POWER_EnablePD(kPDRUNCFG_APD_DSP_DCACHE);
    POWER_EnablePD(kPDRUNCFG_APD_DSP_ITCM);
    POWER_EnablePD(kPDRUNCFG_APD_DSP_DTCM);
    POWER_EnablePD(kPDRUNCFG_APD_EZHV_TCM);
    POWER_EnablePD(kPDRUNCFG_APD_NPU);
    // POWER_EnablePD(kPDRUNCFG_APD_XSPI0);
    POWER_EnablePD(kPDRUNCFG_APD_XSPI1);
    POWER_EnablePD(kPDRUNCFG_APD_XSPI2);
    // POWER_EnablePD(kPDRUNCFG_APD_LCDIF);
    POWER_EnablePD(kPDRUNCFG_APD_OCOTP);
    POWER_EnablePD(kPDRUNCFG_PPD_SDHC0_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_SDHC1_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_USB0_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_USB1_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_JPEGDEC);
    POWER_EnablePD(kPDRUNCFG_PPD_PNGDEC);
    // POWER_EnablePD(kPDRUNCFG_PPD_MIPIDSI);
    // POWER_EnablePD(kPDRUNCFG_PPD_GPU);
    POWER_EnablePD(kPDRUNCFG_PPD_DMA2_3);
    POWER_EnablePD(kPDRUNCFG_PPD_DMA0_1_PKC_ETF);
    // POWER_EnablePD(kPDRUNCFG_PPD_CPU0_CODE_CACHE);
    // POWER_EnablePD(kPDRUNCFG_PPD_CPU0_SYSTEM_CACHE);
    POWER_EnablePD(kPDRUNCFG_PPD_DSP_ICACHE);
    POWER_EnablePD(kPDRUNCFG_PPD_DSP_DCACHE);
    POWER_EnablePD(kPDRUNCFG_PPD_DSP_ITCM);
    POWER_EnablePD(kPDRUNCFG_PPD_DSP_DTCM);
    POWER_EnablePD(kPDRUNCFG_PPD_EZHV_TCM);
    POWER_EnablePD(kPDRUNCFG_PPD_NPU);
    // POWER_EnablePD(kPDRUNCFG_PPD_XSPI0);
    POWER_EnablePD(kPDRUNCFG_PPD_XSPI1);
    POWER_EnablePD(kPDRUNCFG_PPD_XSPI2);
    // POWER_EnablePD(kPDRUNCFG_PPD_LCDIF);
    POWER_EnablePD(kPDRUNCFG_PPD_OCOTP);
}

void BOARD_InitPowerConfig(void)
{
    pca9422_modecfg_t pca9422ModeCfg[12];
    uint32_t i;
    clock_osc32k_config_t config = {
        .bypass = false, .monitorEnable = false, .lowPowerMode = true, .cap = kCLOCK_Osc32kCapPf16};
    /* Configure OSC32K. */
    CLOCK_EnableOsc32K(&config);

    CLKCTL2->MAINPLL0PFDDOMAINEN  = 0;
    CLKCTL2->AUDIOPLL0PFDDOMAINEN = 0;
    /* Disable PLL. */
    CLOCK_DeinitMainPll();
    CLOCK_DeinitAudioPll();

    /* BE CAUTIOUS TO SET CORRECT VOLTAGE RANGE ACCORDING TO YOUR BOARD/APPLICATION. PAD SUPPLY BEYOND THE RANGE DO
       HARM TO THE SILICON. */
    POWER_SetPio2VoltRange(kPadVol_300_360);
    
    BOARD_InitPmic();
    for (i = 0; i < ARRAY_SIZE(pca9422ModeCfg); i++)
    {
        PCA9422_GetDefaultPowerModeConfig(&pca9422ModeCfg[i]);
    }

    for (i = 0; i < ARRAY_SIZE(pca9422ModeCfg); i++)
    {
        BOARD_ConfigPMICModes(pca9422ModeCfg, (pca9422_power_mode_t)i);
        PCA9422_WritePowerModeConfigs(&pca9422Handle, (pca9422_power_mode_t)i, pca9422ModeCfg[i]);
    }
    BOARD_ConfigPMICRegEnable(&pca9422Handle);
    BOARD_ConfigPMICEnMode(&pca9422Handle);
#if defined(DEMO_POWER_SUPPLY_OPTION) && (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
    /* Switch to a new DVS mode before re-configuring the VDD1/VDD2 per CPU frequency. */
    BOARD_SetPmicDVSPinStatus(0x1);
    /* PMIC is used. When using On-Chip regulator, need to be changed to kVddSrc_PMC. */
    POWER_SetVddnSupplySrc(kVddSrc_PMIC);
    POWER_SetVdd1SupplySrc(kVddSrc_PMIC);
    POWER_SetVdd2SupplySrc(kVddSrc_PMIC);
    POWER_DisableRegulators(kPower_SCPC);

    POWER_SelectRunSetpoint(kRegulator_Vdd1LDO, 0U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd1LDO, 0U);
#endif

    CLOCK_EnableClock(kCLOCK_Sleepcon0); /* make sure the sleepcon clock is enabled. */
    POWER_DisablePD(kPDRUNCFG_PD_FRO2);
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK); /* Keep Sense shared parts clock on. */

    POWER_DisablePD(kPDRUNCFG_GATE_FRO0);           /* Just use PD bit to control FRO. */
    POWER_DisablePD(kPDRUNCFG_GATE_FRO2);
    POWER_DisablePD(kPDRUNCFG_SHUT_RAM1_CLK);

    POWER_ApplyPD();
    POWER_DisableLPRequestMask(kPower_MaskAll); /* Let's compute control all the shared resources. */

}

void BOARD_PowerConfigAfterCPU1Booted(void)
{
    BOARD_DisableUnusedClocks();
    BOARD_DisableUnusedModules();
    BOARD_DisableXSPI1();
    
    SYSCON0->COMP_AUTOGATE_EN = 0x7U; /* MBUS_EN bit disabled to allow other master accessing RAM0. */
    POWER_ApplyPD();
    
    POWER_EnableRunAFBB(kPower_BodyBiasVdd2 | kPower_BodyBiasVddn); /* Configure VDD2 AFBB mode during active.*/
    POWER_EnableRunNBB(kPower_BodyBiasVdd2Sram);
    POWER_EnableRunRBB(kPower_BodyBiasVdd1 | kPower_BodyBiasVdd1Sram);
    POWER_EnableSleepRBB(kPower_BodyBiasVddn | kPower_BodyBiasVdd2Sram | kPower_BodyBiasVdd2 | kPower_BodyBiasVdd1 |
        kPower_BodyBiasVdd1Sram);
        
    POWER_ApplyPD();
        
    /* Set the four LDO setpoints per predefined CPU frequency, must in ascending order. */
    uint32_t freqs[4] = {0};
    freqs[0] = 0U; /* For DeepSleep. */
    freqs[1] = 64000000U;
    freqs[2] = SystemCoreClock; /* Only setpoint 2 and 0 are used. */
    freqs[3] = 325000000U;
    
    uint32_t miniVolts[4] = {0U};
    miniVolts[0] = 630000U; /* For DeepSleep. */
    
    POWER_ConfigRegulatorSetpointsForFreq(kRegulator_Vdd2LDO, freqs, miniVolts, 0U, 4U);
    
    uint32_t g_runVolt = POWER_CalcVoltLevel(kRegulator_Vdd2LDO, SystemCoreClock, 0U); /* Calculate the voltage per frequency. */
    
    POWER_SelectRunSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_ApplyPD();
    CLOCK_EnableClock(kCLOCK_Sema420);
    CLOCK_EnableClock(kCLOCK_LPI2c15);
    BOARD_SetPmicVdd2Voltage(g_runVolt); /* 0.9V. */
    CLOCK_DisableClock(kCLOCK_Sema420);
    CLOCK_DisableClock(kCLOCK_LPI2c15);
}

status_t BOARD_StartVGliteClock(void)
{
    PM_SetConstraints(PM_LP_STATE_SLEEP, 0);
	CLOCK_AttachClk(kFRO0_DIV1_to_VGPU);
	CLOCK_SetClkDiv(kCLOCK_DivVgpuClk, 2);
    return kStatus_Success;
}

status_t BOARD_StopVGliteClock(void)
{
	CLOCK_AttachClk(kNONE_to_VGPU);
    PM_ReleaseConstraints(PM_LP_STATE_SLEEP, 0);
    return kStatus_Success;
}

void BOARD_StartMIPIClock()
{
    PM_SetConstraints(PM_LP_STATE_SLEEP, 0);
	CLOCK_AttachClk(kFRO0_DIV1_to_MIPI_DSI_HOST_PHY);
	CLOCK_AttachClk(kFRO0_DIV1_to_MIPI_DPHYESC_CLK);
	CLOCK_EnableClock(kCLOCK_Lcdif);
	CLOCK_EnableClock(kCLOCK_MipiDsiCtrl);
	/* Power Up D-PHY. */
	MIPI_DSI_HOST->PD_DPHY = 0U;
}

void BOARD_StopMIPIClock()
{
	/* Power Down D-PHY. */
	MIPI_DSI_HOST->PD_DPHY = 1U;
	DSI_DeinitDphy(MIPI_DSI_HOST);
	CLOCK_DisableClock(kCLOCK_MipiDsiCtrl);
	CLOCK_DisableClock(kCLOCK_Lcdif);
	CLOCK_AttachClk(kNONE_to_MIPI_DSI_HOST_PHY);
	CLOCK_AttachClk(kNONE_to_MIPI_DPHYESC_CLK);
    PM_ReleaseConstraints(PM_LP_STATE_SLEEP, 0);
}