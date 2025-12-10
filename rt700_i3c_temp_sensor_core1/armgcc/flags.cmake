IF(NOT DEFINED FPU)  
    SET(FPU "-mfloat-abi=hard -mfpu=fpv5-sp-d16")  
ENDIF()  

IF(NOT DEFINED SPECS)  
    SET(SPECS "--specs=nosys.specs")  
ENDIF()  

IF(NOT DEFINED DEBUG_CONSOLE_CONFIG)  
    SET(DEBUG_CONSOLE_CONFIG "-DSDK_DEBUGCONSOLE=1")  
ENDIF()  

SET(CMAKE_ASM_FLAGS_DEBUG " \
    ${CMAKE_ASM_FLAGS_DEBUG} \
    -DCPU_MIMXRT798SGFOB_cm33_core1 \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -include \
    mcux_config.h \
    -mcpu=cortex-m33 \
    -g \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_DEBUG " \
    ${CMAKE_C_FLAGS_DEBUG} \
    -DCPU_MIMXRT798SGFOB_cm33_core1 \
    -DDEBUG \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
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
    -T\"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core1_ram.ld\" -static \
")
  

SET(CMAKE_ASM_FLAGS_RELEASE " \
    ${CMAKE_ASM_FLAGS_RELEASE} \
    -DCPU_MIMXRT798SGFOB_cm33_core1 \
    -D__STARTUP_CLEAR_BSS \
    -DMCUXPRESSO_SDK \
    -include \
    mcux_config.h \
    -mcpu=cortex-m33 \
    -g \
    -mthumb \
    ${FPU} \
")
SET(CMAKE_C_FLAGS_RELEASE " \
    ${CMAKE_C_FLAGS_RELEASE} \
    -DCPU_MIMXRT798SGFOB_cm33_core1 \
    -DNDEBUG \
    -DMCUX_META_BUILD \
    -DMCUXPRESSO_SDK \
    -DSDK_I2C_BASED_COMPONENT_USED=1 \
    -DPRINTF_ADVANCED_ENABLE=1 \
    -DPRINTF_FLOAT_ENABLE=1 \
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
    -T\"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core1_ram.ld\" -static \
")
