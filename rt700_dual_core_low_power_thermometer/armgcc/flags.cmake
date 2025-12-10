IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-sp-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE_UART=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_DEBUG " \
    ${CMAKE_ASM_FLAGS_DEBUG} \
    -DCPU_MIMXRT798SGFOB_cm33_core0 \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -include \
    mcux_config.h \
    -mcpu=cortex-m33 \
    -g \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_DEBUG " \
    ${CMAKE_C_FLAGS_DEBUG} \
    -DCPU_MIMXRT798SGFOB_cm33_core0 \
    -DDEBUG \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DBOOT_HEADER_ENABLE=1 \
    -DMULTICORE_APP=1 \
    -DCORE1_IMAGE_COPY_TO_RAM \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DSSD1963_DATA_WITDH=8 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DMCUX_DBI_LEGACY=0 \
    -DSDK_OS_FREE_RTOS \
    -DLV_CONF_INCLUDE_SIMPLE=1 \
    -DGCID_REV_CID=gc555/0x423_ECO \
    -DCUSTOM_VGLITE_MEMORY_CONFIG=0 \
    -DDBI_USE_MIPI_PANEL=1 \
    -include \
    mcux_config.h \
    -mcpu=cortex-m33 \
    -g \
    -O0 \
    --specs=nano.specs \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -std=gnu99 \
    -fms-extensions \
    -Wno-error=implicit-function-declaration \
    -Wno-error=int-conversion \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG " \
    ${CMAKE_EXE_LINKER_FLAGS_DEBUG} \
    -mcpu=cortex-m33 \
    -g \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Wl,--gc-sections \
    -Wl,-static \
    -Wl,--print-memory-usage \
    -Xlinker \
    -Map=output.map \
    -Wl,--no-warn-rwx-segments \
    ${FPU} \
    ${SPECS} \
    -T\"${SdkRootDirPath}/examples/_boards/mimxrt700evk/lvgl_examples/linker/MIMXRT798Sxxxx_cm33_core0_ram.ld\" -static \
")

SET(CMAKE_ASM_FLAGS_RELEASE " \
    ${CMAKE_ASM_FLAGS_RELEASE} \
    -DCPU_MIMXRT798SGFOB_cm33_core0 \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -D__STARTUP_INITIALIZE_NONCACHEDATA \
    -include \
    mcux_config.h \
    -mcpu=cortex-m33 \
    -g \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_RELEASE " \
    ${CMAKE_C_FLAGS_RELEASE} \
    -DCPU_MIMXRT798SGFOB_cm33_core0 \
    -DNDEBUG \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DBOOT_HEADER_ENABLE=1 \
    -DMULTICORE_APP=1 \
    -DCORE1_IMAGE_COPY_TO_RAM \
    -DBOARD_ENABLE_PSRAM_CACHE=0 \
    -DSSD1963_DATA_WITDH=8 \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DFLEXIO_MCULCD_DATA_BUS_WIDTH=8 \
    -DMCUX_DBI_LEGACY=0 \
    -DSDK_OS_FREE_RTOS \
    -DLV_CONF_INCLUDE_SIMPLE=1 \
    -DGCID_REV_CID=gc555/0x423_ECO \
    -DCUSTOM_VGLITE_MEMORY_CONFIG=0 \
    -DDBI_USE_MIPI_PANEL=1 \
    -include \
    mcux_config.h \
    -mcpu=cortex-m33 \
    -g \
    -O3 \
    --specs=nano.specs \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -std=gnu99 \
    -fms-extensions \
    -Wno-error=implicit-function-declaration \
    -Wno-error=int-conversion \
    ${FPU} \
    ${DEBUG_CONSOLE_CONFIG} \
")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE " \
    ${CMAKE_EXE_LINKER_FLAGS_RELEASE} \
    -mcpu=cortex-m33 \
    -g \
    -Wall \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -fno-builtin \
    -mthumb \
    -mapcs \
    -Wl,--gc-sections \
    -Wl,-static \
    -Wl,--print-memory-usage \
    -Xlinker \
    -Map=output.map \
    -Wl,--no-warn-rwx-segments \
    ${FPU} \
    ${SPECS} \
    -T\"${SdkRootDirPath}/examples/_boards/mimxrt700evk/lvgl_examples/linker/MIMXRT798Sxxxx_cm33_core0_ram.ld\" -static \
")
