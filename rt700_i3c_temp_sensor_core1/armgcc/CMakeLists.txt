# CROSS COMPILER SETTING
set(CMAKE_SYSTEM_NAME Generic)
cmake_minimum_required(VERSION 3.10.0)

if (NOT DEFINED SdkRootDirPath)
    SET(SdkRootDirPath ${CMAKE_CURRENT_LIST_DIR})
endif()

include(${CMAKE_CURRENT_LIST_DIR}/config.cmake)
include(${SdkRootDirPath}/cmake/toolchain/toolchain.cmake)

# THE VERSION NUMBER
SET (MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION 2)
SET (MCUXPRESSO_CMAKE_FORMAT_MINOR_VERSION 0)

include(ide_overrides.cmake OPTIONAL)

if(CMAKE_SCRIPT_MODE_FILE)
  message("${MCUXPRESSO_CMAKE_FORMAT_MAJOR_VERSION}")
  return()
endif()


set(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
set(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
set(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(i3c_master_read_sensor_p3t1755_cm33_core1)

enable_language(ASM)

set(MCUX_BUILD_TYPES debug release)

set(MCUX_SDK_PROJECT_NAME i3c_master_read_sensor_p3t1755_cm33_core1.elf)

include(${ProjDirPath}/flags.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${SdkRootDirPath}/arch/arm/CMSIS/Core/Include/cmsis_compiler.h"
"${SdkRootDirPath}/arch/arm/CMSIS/Core/Include/cmsis_gcc.h"
"${SdkRootDirPath}/arch/arm/CMSIS/Core/Include/cmsis_version.h"
"${SdkRootDirPath}/arch/arm/CMSIS/Core/Include/core_cm33.h"
"${SdkRootDirPath}/arch/arm/CMSIS/Core/Include/m-profile/armv8m_mpu.h"
"${SdkRootDirPath}/arch/arm/CMSIS/Core/Include/m-profile/cmsis_gcc_m.h"
"${SdkRootDirPath}/arch/arm/CMSIS/Core/Include/tz_context.h"
"${SdkRootDirPath}/components/assert/fsl_assert.c"
"${SdkRootDirPath}/components/assert/fsl_assert.h"
"${SdkRootDirPath}/components/debug_console_lite/fsl_debug_console.c"
"${SdkRootDirPath}/components/debug_console_lite/fsl_debug_console.h"
"${SdkRootDirPath}/components/misc_utilities/fsl_memcpy.S"
"${SdkRootDirPath}/components/misc_utilities/fsl_sbrk.c"
"${SdkRootDirPath}/components/misc_utilities/fsl_syscall_stub.c"
"${SdkRootDirPath}/components/pmic/pca9422/fsl_pca9422.c"
"${SdkRootDirPath}/components/pmic/pca9422/fsl_pca9422.h"
"${SdkRootDirPath}/components/sensor/p3t1755/fsl_p3t1755.c"
"${SdkRootDirPath}/components/sensor/p3t1755/fsl_p3t1755.h"
"${SdkRootDirPath}/components/str/fsl_str.c"
"${SdkRootDirPath}/components/str/fsl_str.h"
"${SdkRootDirPath}/components/uart/fsl_adapter_lpuart.c"
"${SdkRootDirPath}/components/uart/fsl_adapter_uart.h"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/drivers/fsl_clock.c"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/drivers/fsl_clock.h"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/drivers/fsl_iopctl.h"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/drivers/fsl_power.c"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/drivers/fsl_power.h"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/drivers/fsl_reset.c"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/drivers/fsl_reset.h"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/fsl_device_registers.h"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/gcc/startup_MIMXRT798S_cm33_core1.S"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/MIMXRT798S_cm33_core1_COMMON.h"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/MIMXRT798S_cm33_core1_features.h"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/MIMXRT798S_cm33_core1.h"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/system_MIMXRT798S_cm33_core1.c"
"${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/system_MIMXRT798S_cm33_core1.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_ADC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_AHBSC0.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_AHBSC3.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_AHBSC4.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CACHE64_CTRL.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CACHE64_POLSEL.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CDOG.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CLKCTL0.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CLKCTL1.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CLKCTL2.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CLKCTL3.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CLKCTL4.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CMP.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CRC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_CTIMER.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_DEBUG_MAILBOX.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_DMA.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_ELS.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_FLEXIO.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_FREQME.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_FRO.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_GDET.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_GLIKEY.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_GPIO.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_I2S.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_I3C.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_INPUTMUX0.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_INPUTMUX1.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_IOPCTL0.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_IOPCTL1.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_IOPCTL2.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_ITRC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_JPEGDEC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_JPGDECWRP.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_LCDIF.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_LP_FLEXCOMM.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_LPI2C.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_LPSPI.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_LPUART.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_MIPI_DSI_HOST.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_MMU.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_MRT.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_MU.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_NIC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_OCOTP.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_OSC32KNP.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_OSCCA.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_OSTIMER.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_PDM.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_PINT.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_PKC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_PMC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_PNGDEC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_PUF.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_PVTS.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_ROMCP.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_RSTCTL0.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_RSTCTL1.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_RSTCTL2.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_RSTCTL3.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_RSTCTL4.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_RTC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SCT.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SDADC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SEMA42.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SLEEPCON0.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SLEEPCON1.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SYSCON0.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SYSCON1.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SYSCON2.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SYSCON3.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SYSCON4.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_SYSPM.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_TRNG.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_USB.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_USBHSDCD.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_USBNC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_USBPHY.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_USDHC.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_UTICK.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_WWDT.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_XCACHE.h"
"${SdkRootDirPath}/devices/RT/RT700/periph/PERI_XSPI.h"
"${SdkRootDirPath}/drivers/common/fsl_common_arm.c"
"${SdkRootDirPath}/drivers/common/fsl_common_arm.h"
"${SdkRootDirPath}/drivers/common/fsl_common.c"
"${SdkRootDirPath}/drivers/common/fsl_common.h"
"${SdkRootDirPath}/drivers/glikey/fsl_glikey.c"
"${SdkRootDirPath}/drivers/glikey/fsl_glikey.h"
"${SdkRootDirPath}/drivers/gpio/fsl_gpio.c"
"${SdkRootDirPath}/drivers/gpio/fsl_gpio.h"
"${SdkRootDirPath}/drivers/i3c/fsl_i3c.c"
"${SdkRootDirPath}/drivers/i3c/fsl_i3c.h"
"${SdkRootDirPath}/drivers/lpflexcomm/fsl_lpflexcomm.c"
"${SdkRootDirPath}/drivers/lpflexcomm/fsl_lpflexcomm.h"
"${SdkRootDirPath}/drivers/lpflexcomm/lpi2c/fsl_lpi2c.c"
"${SdkRootDirPath}/drivers/lpflexcomm/lpi2c/fsl_lpi2c.h"
"${SdkRootDirPath}/drivers/lpflexcomm/lpuart/fsl_lpuart.c"
"${SdkRootDirPath}/drivers/lpflexcomm/lpuart/fsl_lpuart.h"
"${SdkRootDirPath}/drivers/sema42/fsl_sema42.c"
"${SdkRootDirPath}/drivers/sema42/fsl_sema42.h"
"${SdkRootDirPath}/drivers/mu1/fsl_mu.c"
"${SdkRootDirPath}/drivers/mu1/fsl_mu.h"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/board.c"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/board.h"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/common/clock/cm33_core1/clock_config.c"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/common/clock/cm33_core1/clock_config.h"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/driver_examples/i3c/master_read_sensor_p3t1755/cm33_core1/app.h"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/driver_examples/i3c/master_read_sensor_p3t1755/cm33_core1/hardware_init.c"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/driver_examples/i3c/master_read_sensor_p3t1755/cm33_core1/pin_mux.c"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/driver_examples/i3c/master_read_sensor_p3t1755/cm33_core1/pin_mux.h"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/dsp_examples/dsp_support.c"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/dsp_examples/dsp_support.h"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/multicore_examples/dsp_config.h"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/pmic_support.c"
"${SdkRootDirPath}/examples/_boards/mimxrt700evk/pmic_support.h"
"${SdkRootDirPath}/examples/driver_examples/i3c/master_read_sensor_p3t1755/i3c_master_read_sensor_p3t1755.c"
"${SdkRootDirPath}/mcux_config.h"
"${SdkRootDirPath}/middleware/multicore/mcmgr/inc/mcmgr_internal_core_api.h"
"${SdkRootDirPath}/middleware/multicore/mcmgr/inc/mcmgr.h"
"${SdkRootDirPath}/middleware/multicore/mcmgr/inc/platform/imxrt700/mcmgr_platform.h"
"${SdkRootDirPath}/middleware/multicore/mcmgr/src/mcmgr_internal_core_api_imxrt700.c"
"${SdkRootDirPath}/middleware/multicore/mcmgr/src/mcmgr_mu_internal.c"
"${SdkRootDirPath}/middleware/multicore/mcmgr/src/mcmgr.c"
"${SdkRootDirPath}/source/inc/power_management.h"
"${SdkRootDirPath}/source/src/power_management.c"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${SdkRootDirPath}
    ${SdkRootDirPath}/arch/arm/CMSIS/Core/Include
    ${SdkRootDirPath}/arch/arm/CMSIS/Core/Include/m-profile
    ${SdkRootDirPath}/components/assert
    ${SdkRootDirPath}/components/debug_console_lite
    ${SdkRootDirPath}/components/pmic/pca9422/
    ${SdkRootDirPath}/components/sensor/p3t1755
    ${SdkRootDirPath}/components/str
    ${SdkRootDirPath}/components/uart
    ${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S
    ${SdkRootDirPath}/devices/RT/RT700/MIMXRT798S/drivers
    ${SdkRootDirPath}/devices/RT/RT700/periph
    ${SdkRootDirPath}/drivers/common
    ${SdkRootDirPath}/drivers/glikey
    ${SdkRootDirPath}/drivers/gpio
    ${SdkRootDirPath}/drivers/i3c
    ${SdkRootDirPath}/drivers/lpflexcomm
    ${SdkRootDirPath}/drivers/lpflexcomm/lpi2c
    ${SdkRootDirPath}/drivers/lpflexcomm/lpuart
    ${SdkRootDirPath}/drivers/mu1
    ${SdkRootDirPath}/drivers/sema42
    ${SdkRootDirPath}/examples/_boards/mimxrt700evk
    ${SdkRootDirPath}/examples/_boards/mimxrt700evk/common/clock/cm33_core1
    ${SdkRootDirPath}/examples/_boards/mimxrt700evk/driver_examples/i3c/master_read_sensor_p3t1755
    ${SdkRootDirPath}/examples/_boards/mimxrt700evk/driver_examples/i3c/master_read_sensor_p3t1755/cm33_core1
    ${SdkRootDirPath}/examples/_boards/mimxrt700evk/dsp_examples/
    ${SdkRootDirPath}/examples/_boards/mimxrt700evk/multicore_examples/
    ${SdkRootDirPath}/middleware/multicore/mcmgr/inc
    ${SdkRootDirPath}/middleware/multicore/mcmgr/inc/platform/imxrt700
    ${SdkRootDirPath}/source/inc/
)


IF(NOT DEFINED TARGET_LINK_SYSTEM_LIBRARIES)  
    SET(TARGET_LINK_SYSTEM_LIBRARIES "-lm -lc -lgcc -lnosys")  
ENDIF()  

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${TARGET_LINK_SYSTEM_LIBRARIES})

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

ADD_CUSTOM_COMMAND(TARGET ${MCUX_SDK_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_OBJCOPY}
-Obinary ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/core1_image.bin)

set_target_properties(${MCUX_SDK_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES "output.map;${EXECUTABLE_OUTPUT_PATH}/i3c_master_read_sensor_p3t1755_cm33_core1.bin")

