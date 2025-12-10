/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "board.h"
#include "core1_support.h"
#include "fsl_cache.h"
#include "mcmgr.h"
#include "fsl_debug_console.h"
#include "app.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void CORE1_InitCore()
{
    /* Initialize MCMGR, install generic event handlers */
    (void)MCMGR_Init();

#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* This section ensures the secondary core image is copied from flash location to the target RAM memory.
       It consists of several steps: image size calculation, image copying and cache invalidation (optional for some
       platforms/cases). These steps are not required on MCUXpresso IDE which copies the secondary core image to the
       target memory during startup automatically. */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    (void)PRINTF("Copy Secondary core image to address: 0x%x, size: %d\r\n", (void *)(char *)CORE1_BOOT_ADDRESS,
                 core1_image_size);

    /* Copy Secondary core application from FLASH to the target memory. */
    (void)memcpy((void *)(char *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
    /* Invalidate cache for memory range the secondary core image has been copied to. */
    invalidate_cache_for_core1_image_memory(CORE1_BOOT_ADDRESS, core1_image_size);
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */
#endif /* CORE1_IMAGE_COPY_TO_RAM */

    /* Boot Secondary core application */
    (void)PRINTF("Starting Secondary core.\r\n");
    (void)MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, 2, kMCMGR_Start_Synchronous);
}

void BOARD_CopyCore1Image(uint32_t addr)
{
    XCACHE_CleanInvalidateCacheByRange(addr, CORE1_IMAGE_SIZE);
    memcpy((void *)addr, (void *)CORE1_IMAGE_START, CORE1_IMAGE_SIZE);
    XCACHE_CleanInvalidateCacheByRange(addr, CORE1_IMAGE_SIZE);
}

void BOARD_ReleaseCore1Power()
{
    /* Powerup all the SRAM partitions. */
    PMC0->PDRUNCFG2 &= ~0x3FFC0000;
    PMC0->PDRUNCFG3 &= ~0x3FFC0000;

    POWER_DisablePD(kPDRUNCFG_SHUT_SENSEP_MAINCLK);
    POWER_ApplyPD();
}

void BOARD_BootCore1(uint32_t nsVector, uint32_t sVector)
{
    /*Glikey write enable, GLIKEY4*/
    GlikeyWriteEnable(GLIKEY4, 1U);

    /* Boot source for Core 1 from RAM. */
    SYSCON3->CPU1_NSVTOR = (nsVector >> 7U);
    SYSCON3->CPU1_SVTOR  = (sVector >> 7U);
    
    GlikeyClearConfig(GLIKEY4);

    /* Enable cpu1 clock. */
    CLOCK_EnableClock(kCLOCK_Cpu1);

    /* Clear reset*/
    RESET_ClearPeripheralReset(kCPU1_RST_SHIFT_RSTn);

    /* Release cpu wait*/
    SYSCON3->CPU_STATUS &= ~SYSCON3_CPU_STATUS_CPU_WAIT_MASK;
}