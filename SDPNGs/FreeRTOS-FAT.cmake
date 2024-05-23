
if (NOT DEFINED FreeRTOS_FAT_CLI_PATH)
    set(FreeRTOS_FAT_CLI_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-FAT-CLI-for-RPi-Pico")
endif()

if (NOT DEFINED Lab_FreeRTOS_FAT_PATH)
    set(Lab_FreeRTOS_FAT_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/Lab-Project-FreeRTOS-FAT")
endif()


if (NOT DEFINED FreeRTOS_FAT_PORT)
    set(FreeRTOS_FAT_PORT "${CMAKE_CURRENT_LIST_DIR}/port/FreeRTOS-FAT")
endif()


set (FAT_CLI "${FreeRTOS_FAT_CLI_PATH}/src/FreeRTOS+FAT+CLI")

add_library(FreeRTOS_FAT INTERFACE)
target_sources(FreeRTOS_FAT INTERFACE
        ${Lab_FreeRTOS_FAT_PATH}/ff_crc.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_dir.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_error.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_fat.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_file.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_format.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_ioman.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_locking.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_memory.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_stdio.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_string.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_sys.c
        ${Lab_FreeRTOS_FAT_PATH}/ff_time.c 
        ${FAT_CLI}/portable/RP2040/dma_interrupts.c
        ${FAT_CLI}/portable/RP2040/ff_sddisk.c
        ${FAT_CLI}/portable/RP2040/sd_card.c
        ${FAT_CLI}/portable/RP2040/SPI/crc.c
        ${FAT_CLI}/portable/RP2040/SPI/sd_card_spi.c
        ${FAT_CLI}/portable/RP2040/SPI/sd_spi.c
        ${FAT_CLI}/portable/RP2040/SPI/my_spi.c
        ${FAT_CLI}/portable/RP2040/SDIO/sd_card_sdio.c
        ${FAT_CLI}/portable/RP2040/SDIO/rp2040_sdio.c
        ${FAT_CLI}/src/ff_utils.c
#        ${FAT_CLI}/src/freertos_callbacks.c
        ${FAT_CLI}/src/FreeRTOS_strerror.c
        ${FAT_CLI}/src/FreeRTOS_time.c
        ${FAT_CLI}/src/my_debug.c
        ${FAT_CLI}/src/util.c
)
target_link_libraries(FreeRTOS_FAT INTERFACE
        cmsis_core
        FreeRTOS-Kernel
        hardware_adc
        hardware_clocks
        hardware_dma
        hardware_pio
        hardware_rtc
        hardware_spi
        hardware_timer
        pico_multicore
        pico_stdlib
        pico_sync
)
target_include_directories(FreeRTOS_FAT INTERFACE 
        ${FAT_CLI}/include/ 
        ${Lab_FreeRTOS_FAT_PATH}/include/
        ${FAT_CLI}/portable/RP2040/
        ${FreeRTOS_FAT_PORT}/
)