/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef CORE1_SUPPORT_H_
#define CORE1_SUPPORT_H_

#include "app.h"

/*******************************************************************************
 * DEFINITION
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

void CORE1_InitCore(void);

/*!
 * @brief Copy CPU1 image to the SRAM by specified address.
 * This API will copy the image from flash to CPU1 running SRAM.
 * NOTE, the memory address need to be writable by CPU0.
 *
 * @param addr The destination for image copy.
 */
void BOARD_CopyCore1Image(uint32_t addr);

/*!
 * @brief Power up all SRAM and Core1 in sense domain.
 */
void BOARD_ReleaseCore1Power(void);

/*!
 * @brief Boot CPU1.
 * This API will set CPU1 vector address, then enable the clock and release wait for CPU1.
 *
 * @param nsVector Non-Secure vector address.
 * @param sVector Secure vector address.
 */
void BOARD_BootCore1(uint32_t nsVector, uint32_t sVector);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* CORE1_SUPPORT_H_ */
