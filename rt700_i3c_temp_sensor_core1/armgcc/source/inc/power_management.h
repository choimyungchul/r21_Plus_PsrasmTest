/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef POWER_MANAGEMENT_H_
#define POWER_MANAGEMENT_H_

#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Define the power supply mode on the board.
 *  - DEMO_POWER_SUPPLY_MIXED, the VDDN is supplied by external PMIC. VDD1 and VDD2 are supplied by internal LDOs.
 *  - DEMO_POWER_SUPPLY_PMIC, the VDDN, VDD1 and VDD2 are supplied by PMIC.
 */
#define DEMO_POWER_SUPPLY_PMIC  2U
#define DEMO_POWER_SUPPLY_MIXED 3U
#define DEMO_POWER_SUPPLY_OPTION DEMO_POWER_SUPPLY_PMIC

#define DEMO_POWER_CPU1_PRINT_ENABLE 0U
#if DEMO_POWER_CPU1_PRINT_ENABLE
#define DEMO_LOG PRINTF
#else
#define DEMO_LOG(...)
#endif

#define DEMO_POWER_ENABLE_DEBUG 1U /* Enable debug or not, disable the debug function/clock can save power. */

/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#define APP_DEEPSLEEP_SLEEPCFG    (SLEEPCON1_SLEEPCFG_FRO2_PD_MASK | SLEEPCON1_SLEEPCFG_FRO2_GATE_MASK) /* SLEEPCON->SLEEPCFG */
#define APP_DEEPSLEEP_PDSLEEPCFG0 (0U)                           /* PMC->PDSLEEPCFG0 */
#define APP_DEEPSLEEP_PDSLEEPCFG1 (0U)                           /* PMC->PDSLEEPCFG1 */
#define APP_DSR_PDSLEEPCFG0       (0U)                           /* PMC->PDSLEEPCFG0 */
#define APP_DEEPSLEEP_RAM_APD     0x00600000U /* PMC->PDSLEEPCFG2, all keep powered, the unused PT can be power off. */
#define APP_DEEPSLEEP_RAM_PPD     (0U)     /* PMC->PDSLEEPCFG3 */
#define APP_DEEPSLEEP_PDSLEEPCFG4 \
    (PMC_PDSLEEPCFG4_CPU0_CCACHE_MASK | PMC_PDSLEEPCFG4_CPU0_SCACHE_MASK | PMC_PDSLEEPCFG4_OCOTP_MASK)
#define APP_DSR_PDSLEEPCFG4       (PMC_PDSLEEPCFG4_OCOTP_MASK) /* Cache is not retented in DSR. */
#define APP_DEEPSLEEP_PDSLEEPCFG5 (0U)
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DEEPSLEEP_PDSLEEPCFG0, APP_DEEPSLEEP_PDSLEEPCFG1, \
                         APP_DEEPSLEEP_RAM_APD, APP_DEEPSLEEP_RAM_PPD, APP_DEEPSLEEP_PDSLEEPCFG4,      \
                         APP_DEEPSLEEP_PDSLEEPCFG5}))

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

void BOARD_InitPowerConfig(void);
void BOARD_EnterDeepSleep(const uint32_t exclude_from_pd[7]);

#if defined(__cplusplus)
}
#endif

#endif /* POWER_MANAGEMENT_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
