/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "power_management.h"
#include "pmic_support.h"

#define DEMO_LOW_POWER_RUN_VOLT 700000U /* The VDD1 voltage during low power run. Used for CPU1 DS while CPU0 active.*/

uint32_t g_runVolt = 1000000U; /* The VDD1 voltage during normal run, 1000mV, will be update per CPU clock. */

void BOARD_DisableUnusedClocks(void)
{

    // CLOCK_DisableClock(kCLOCK_Sleepcon1);
    CLOCK_DisableClock(kCLOCK_Syscon1);
    CLOCK_DisableClock(kCLOCK_SenseAccessRamArbiter0);
    CLOCK_DisableClock(kCLOCK_Hifi1);
    CLOCK_DisableClock(kCLOCK_Dma2);
    CLOCK_DisableClock(kCLOCK_Dma3);
    CLOCK_DisableClock(kCLOCK_LPFlexComm17);
    CLOCK_DisableClock(kCLOCK_LPFlexComm18);
    CLOCK_DisableClock(kCLOCK_LPFlexComm20);
    CLOCK_DisableClock(kCLOCK_LPI2c17);
    CLOCK_DisableClock(kCLOCK_LPI2c18);
    CLOCK_DisableClock(kCLOCK_LPI2c20);
    CLOCK_DisableClock(kCLOCK_LPSpi17);
    CLOCK_DisableClock(kCLOCK_LPSpi18);
    CLOCK_DisableClock(kCLOCK_LPSpi20);
    CLOCK_DisableClock(kCLOCK_LPUart17);
    CLOCK_DisableClock(kCLOCK_LPUart18);
    CLOCK_DisableClock(kCLOCK_LPUart20);
    CLOCK_DisableClock(kCLOCK_Sai3);
    CLOCK_DisableClock(kCLOCK_I3c2);
    // CLOCK_DisableClock(kCLOCK_I3c3);
    CLOCK_DisableClock(kCLOCK_Gpio8);
    CLOCK_DisableClock(kCLOCK_Gpio9);
    CLOCK_DisableClock(kCLOCK_Gpio10);
    CLOCK_DisableClock(kCLOCK_Pint1);
    CLOCK_DisableClock(kCLOCK_Ct32b5);
    CLOCK_DisableClock(kCLOCK_Ct32b6);
    CLOCK_DisableClock(kCLOCK_Ct32b7);
    CLOCK_DisableClock(kCLOCK_Mrt1);
    CLOCK_DisableClock(kCLOCK_Utick1);
    CLOCK_DisableClock(kCLOCK_Cdog3);
    CLOCK_DisableClock(kCLOCK_Cdog4);
    CLOCK_DisableClock(kCLOCK_Mu3);
    CLOCK_DisableClock(kCLOCK_Sema423);
    CLOCK_DisableClock(kCLOCK_Wwdt2);
    CLOCK_DisableClock(kCLOCK_Wwdt3);
    CLOCK_DisableClock(kCLOCK_Pvts1);
    CLOCK_DisableClock(kCLOCK_InputMux1);
    CLOCK_DisableClock(kCLOCK_Syscon2);
    CLOCK_DisableClock(kCLOCK_Iopctl2);
    // CLOCK_DisableClock(kCLOCK_Cpu1);
    CLOCK_DisableClock(kCLOCK_Mu0);
    // CLOCK_DisableClock(kCLOCK_Mu1);
    CLOCK_DisableClock(kCLOCK_Mu2);
    CLOCK_DisableClock(kCLOCK_OsTimer);
    CLOCK_DisableClock(kCLOCK_Sema420);
    CLOCK_DisableClock(kCLOCK_Sdadc0);
    CLOCK_DisableClock(kCLOCK_Adc0);
    CLOCK_DisableClock(kCLOCK_Acmp0);
    CLOCK_DisableClock(kCLOCK_Pdm);
    CLOCK_DisableClock(kCLOCK_Glikey4);
#if (DEMO_POWER_ENABLE_DEBUG == 0U)
    CLOCK_DisableClock(kCLOCK_Dbg);
#endif
#if (DEMO_POWER_CPU1_PRINT_ENABLE == 0U)
    CLOCK_DisableClock(kCLOCK_LPUart19);
    CLOCK_DisableClock(kCLOCK_LPFlexComm19);
    CLOCK_DisableClock(kCLOCK_LPI2c19);
    CLOCK_DisableClock(kCLOCK_LPSpi19);
    CLOCK_AttachClk(kNONE_to_FLEXCOMM19);
#endif
    CLOCK_DisableClock(kCLOCK_Syscon3);
    CLOCK_DisableClock(kCLOCK_Iopctl1);
    CLOCK_DisableClock(kCLOCK_Glikey1);
    // CLOCK_DisableClock(kCLOCK_LPI2c15);
    CLOCK_DisableClock(kCLOCK_MediaAccessRamArbiter1);
    CLOCK_DisableClock(kCLOCK_Axi0);
    CLOCK_DisableClock(kCLOCK_Gpu);
    CLOCK_DisableClock(kCLOCK_MipiDsiCtrl);
    CLOCK_DisableClock(kCLOCK_LPSpi16);
    CLOCK_DisableClock(kCLOCK_LPSpi14);
    CLOCK_DisableClock(kCLOCK_Xspi2);
    CLOCK_DisableClock(kCLOCK_Mmu2);
    CLOCK_DisableClock(kCLOCK_Glikey5);
    CLOCK_DisableClock(kCLOCK_Flexio);
    CLOCK_DisableClock(kCLOCK_Lcdif);
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
    CLOCK_DisableClock(kCLOCK_Gdet3Ref);
    CLOCK_DisableClock(kCLOCK_Rtc);
    CLOCK_DisableClock(kCLOCK_Gdet3);
    CLOCK_DisableClock(kCLOCK_UsbphyRef);

    CLOCK_AttachClk(kNONE_to_SYSTICK);
    CLOCK_AttachClk(kNONE_to_MICFIL0);
    CLOCK_AttachClk(kNONE_to_SENSE_DSP);
    CLOCK_AttachClk(kNONE_to_SAI3);
    CLOCK_AttachClk(kNONE_to_UTICK);
    CLOCK_AttachClk(kNONE_to_WWDT2);
    CLOCK_AttachClk(kNONE_to_WWDT3);
    CLOCK_AttachClk(kNONE_to_CTIMER5);
    CLOCK_AttachClk(kNONE_to_CTIMER6);
    CLOCK_AttachClk(kNONE_to_CTIMER7);
    CLOCK_AttachClk(kNONE_to_FLEXCOMM17);
    CLOCK_AttachClk(kNONE_to_FLEXCOMM18);
    CLOCK_AttachClk(kNONE_to_FLEXCOMM20);
}

void BOARD_DisableUnusedModules(void)
{
    /* Disable unused modules. */
    POWER_EnablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK);
    POWER_EnablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);
    POWER_EnablePD(kPDRUNCFG_SHUT_RAM0_CLK);
    POWER_EnablePD(kPDRUNCFG_SHUT_RAM1_CLK);
    POWER_EnablePD(kPDRUNCFG_SHUT_COMNN_MAINCLK);
    POWER_EnablePD(kPDRUNCFG_SHUT_MEDIA_MAINCLK);
    // POWER_EnablePD(kPDRUNCFG_PD_SYSXTAL);
    // POWER_EnablePD(kPDRUNCFG_PD_FRO2);
    POWER_EnablePD(kPDRUNCFG_PD_LPOSC);
    POWER_EnablePD(kPDRUNCFG_PD_PLLANA);
    POWER_EnablePD(kPDRUNCFG_PD_PLLLDO);
    POWER_EnablePD(kPDRUNCFG_PD_AUDPLLANA);
    POWER_EnablePD(kPDRUNCFG_PD_AUDPLLLDO);
    POWER_EnablePD(kPDRUNCFG_PD_ADC0);
    // POWER_EnablePD(kPDRUNCFG_GATE_FRO2);
    POWER_EnablePD(kPDRUNCFG_DSR_VDD2N_MEDIA);
    POWER_EnablePD(kPDRUNCFG_DSR_VDDN_COM);
    POWER_EnablePD(kPDRUNCFG_PD_VDD2_DSP);
    POWER_EnablePD(kPDRUNCFG_PD_VDD2_MIPI);
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
    POWER_EnablePD(kPDRUNCFG_APD_SRAM7);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM8);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM9);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM10);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM11);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM12);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM13);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM14);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM15);
    POWER_EnablePD(kPDRUNCFG_APD_SRAM16);
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
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM7);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM8);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM9);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM10);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM11);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM12);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM13);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM14);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM15);
    POWER_EnablePD(kPDRUNCFG_PPD_SRAM16);
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
    POWER_EnablePD(kPDRUNCFG_APD_MIPIDSI);
    POWER_EnablePD(kPDRUNCFG_APD_GPU);
    POWER_EnablePD(kPDRUNCFG_APD_DMA2_3);
    POWER_EnablePD(kPDRUNCFG_APD_DMA0_1_PKC_ETF);
    POWER_EnablePD(kPDRUNCFG_APD_CPU0_CODE_CACHE);
    POWER_EnablePD(kPDRUNCFG_APD_CPU0_SYSTEM_CACHE);
    POWER_EnablePD(kPDRUNCFG_APD_DSP_ICACHE);
    POWER_EnablePD(kPDRUNCFG_APD_DSP_DCACHE);
    POWER_EnablePD(kPDRUNCFG_APD_DSP_ITCM);
    POWER_EnablePD(kPDRUNCFG_APD_DSP_DTCM);
    POWER_EnablePD(kPDRUNCFG_APD_EZHV_TCM);
    POWER_EnablePD(kPDRUNCFG_APD_NPU);
    POWER_EnablePD(kPDRUNCFG_APD_XSPI0);
    POWER_EnablePD(kPDRUNCFG_APD_XSPI1);
    POWER_EnablePD(kPDRUNCFG_APD_XSPI2);
    POWER_EnablePD(kPDRUNCFG_APD_LCDIF);
    POWER_EnablePD(kPDRUNCFG_APD_OCOTP);
    POWER_EnablePD(kPDRUNCFG_PPD_SDHC0_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_SDHC1_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_USB0_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_USB1_SRAM);
    POWER_EnablePD(kPDRUNCFG_PPD_JPEGDEC);
    POWER_EnablePD(kPDRUNCFG_PPD_PNGDEC);
    POWER_EnablePD(kPDRUNCFG_PPD_MIPIDSI);
    POWER_EnablePD(kPDRUNCFG_PPD_GPU);
    POWER_EnablePD(kPDRUNCFG_PPD_DMA2_3);
    POWER_EnablePD(kPDRUNCFG_PPD_DMA0_1_PKC_ETF);
    POWER_EnablePD(kPDRUNCFG_PPD_CPU0_CODE_CACHE);
    POWER_EnablePD(kPDRUNCFG_PPD_CPU0_SYSTEM_CACHE);
    POWER_EnablePD(kPDRUNCFG_PPD_DSP_ICACHE);
    POWER_EnablePD(kPDRUNCFG_PPD_DSP_DCACHE);
    POWER_EnablePD(kPDRUNCFG_PPD_DSP_ITCM);
    POWER_EnablePD(kPDRUNCFG_PPD_DSP_DTCM);
    POWER_EnablePD(kPDRUNCFG_PPD_EZHV_TCM);
    POWER_EnablePD(kPDRUNCFG_PPD_NPU);
    POWER_EnablePD(kPDRUNCFG_PPD_XSPI0);
    POWER_EnablePD(kPDRUNCFG_PPD_XSPI1);
    POWER_EnablePD(kPDRUNCFG_PPD_XSPI2);
    POWER_EnablePD(kPDRUNCFG_PPD_LCDIF);
    POWER_EnablePD(kPDRUNCFG_PPD_OCOTP);
}

void BOARD_InitPowerConfig(void)
{
    BOARD_DisableUnusedClocks();
    BOARD_DisableUnusedModules();

    /* Enable the used modules in sense side. */
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_SHUT_SENSES_MAINCLK);
    POWER_DisablePD(kPDRUNCFG_GATE_FRO2);
    POWER_DisablePD(kPDRUNCFG_PD_FRO2);
    POWER_DisablePD(kPDRUNCFG_PD_LPOSC); /* Used by RTC. */

    SYSCON3->SENSE_AUTOGATE_EN = 0x3U;
    CLOCK_EnableClock(kCLOCK_Cpu1); /*Let CPU1 control it's clock. */

    POWER_ApplyPD();

    /* Request the domains out of sense into RBB mode. */
    POWER_EnableRunAFBB(kPower_BodyBiasVdd1);
    POWER_EnableRunNBB(kPower_BodyBiasVdd1Sram);
    POWER_EnableRunRBB(kPower_BodyBiasVdd2 | kPower_BodyBiasVddn | kPower_BodyBiasVdd2Sram);
    POWER_EnableSleepRBB(kPower_BodyBiasVdd2 | kPower_BodyBiasVddn | kPower_BodyBiasVdd2Sram | kPower_BodyBiasVdd1 |
                         kPower_BodyBiasVdd1Sram);
    POWER_ApplyPD();

    /* Set the four LDO setpoints per predefined CPU frequency, must in ascending order*/
    uint32_t freqs[4] = {0};
    freqs[0] = 0U; /* For DeepSleep. */
    freqs[1] = 64000000U; /* 64MHz */
    freqs[2] = SystemCoreClock; /* Only setpoint 2 and 0 are used. */
    freqs[3] = 250000000U;

    uint32_t miniVolts[4] = {0U}; 
    miniVolts[0] = 630000U; /* For DeepSleep, 0.63V. */

    POWER_ConfigRegulatorSetpointsForFreq(kRegulator_Vdd1LDO, freqs, miniVolts, 0U, 4U);

    g_runVolt = POWER_CalcVoltLevel(kRegulator_Vdd1LDO, SystemCoreClock, 0U); /* Calculate the voltage per frequency. */

#if defined(DEMO_POWER_SUPPLY_OPTION) && (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_MIXED)
    POWER_SelectRunSetpoint(kRegulator_Vdd1LDO, 2U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd1LDO, 0U);
    POWER_SelectRunSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_ApplyPD();
#elif defined(DEMO_POWER_SUPPLY_OPTION) && (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
    POWER_DisableLPRequestMask(kPower_MaskLpi2c15);
    BOARD_InitPmic();
    /* Select the lowest LVD setpoint. */
    POWER_SelectRunSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd2LDO, 0U);
    POWER_SelectRunSetpoint(kRegulator_Vdd1LDO, 0U);
    POWER_SelectSleepSetpoint(kRegulator_Vdd1LDO, 0U);
    POWER_ApplyPD();

    BOARD_SetPmicVdd1Voltage(g_runVolt);
#endif

}

/*! @brief Decrease the CPU frequency and supply voltage for lower power consumption.
 */
static inline void BOARD_PrepareForDS(void)
{
    /* Change to a lower frequency to safely decrease the VDD1 voltage when CPU1 enter low power mode but CPU0 is active
     * and still requires sense shared main clock. */
    CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv3OutEn | kCLOCK_FroDiv6OutEn); /* Need Keep DIV6. */
#if defined(DEMO_POWER_SUPPLY_OPTION) && (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
    BOARD_SetPmicVdd1Voltage(DEMO_LOW_POWER_RUN_VOLT);
#endif
#if (defined(BOARD_PMIC_CONFIG_USE_SEMA4) && (BOARD_PMIC_CONFIG_USE_SEMA4 != 0U))
    CLOCK_DisableClock(kCLOCK_Sema420);
#endif
    CLOCK_DisableClock(kCLOCK_LPI2c15);
}

/*! @brief Decrease the CPU frequency and supply voltage for lower power consumption.
 */
static inline void BOARD_RestoreAfterDS(void)
{
#if (defined(BOARD_PMIC_CONFIG_USE_SEMA4) && (BOARD_PMIC_CONFIG_USE_SEMA4 != 0U))
    CLOCK_EnableClock(kCLOCK_Sema420);
#endif
    CLOCK_EnableClock(kCLOCK_LPI2c15);
#if defined(DEMO_POWER_SUPPLY_OPTION) && (DEMO_POWER_SUPPLY_OPTION == DEMO_POWER_SUPPLY_PMIC)
    BOARD_SetPmicVdd1Voltage(g_runVolt);
#endif
    CLOCK_EnableFroClkOutput(FRO2, kCLOCK_FroDiv3OutEn);
}

void BOARD_EnterDeepSleep(const uint32_t exclude_from_pd[7])
{
    BOARD_PrepareForDS();
    POWER_EnterDeepSleep(exclude_from_pd);
    BOARD_RestoreAfterDS();
 }