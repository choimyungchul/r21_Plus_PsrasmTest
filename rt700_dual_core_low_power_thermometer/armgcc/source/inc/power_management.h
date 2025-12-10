/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef POWER_MANAGEMENT_H_
#define POWER_MANAGEMENT_H_

#include "fsl_common.h"

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

#define DEMO_POWER_ENABLE_DEBUG 1U /* Enable debug or not, disable the debug function/clock can save power. */
/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

void BOARD_InitPowerConfig(void);
void BOARD_PowerConfigAfterCPU1Booted(void);
status_t BOARD_StartVGliteClock(void);
status_t BOARD_StopVGliteClock(void);
void BOARD_StartMIPIClock(void);
void BOARD_StopMIPIClock(void);


#if defined(__cplusplus)
}
#endif

#endif /* POWER_MANAGEMENT_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
