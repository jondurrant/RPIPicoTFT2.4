# Add library cpp files

if (NOT DEFINED SPNG_DIR)
    set(SPNG_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/libspng")
endif()

if (NOT DEFINED MINIZ_DIR)
    set(MINIZ_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/miniz")
endif()

if (NOT DEFINED MINIZ_PORT)
    set(MINIZ_PORT "${CMAKE_CURRENT_LIST_DIR}/port/miniz")
endif()


add_library(SPNG STATIC)
target_sources(SPNG PUBLIC
    ${SPNG_DIR}/spng/spng.c
    ${MINIZ_DIR}/miniz.c
    ${MINIZ_DIR}/miniz_tinfl.c
)

# Add include directory
target_include_directories(SPNG PUBLIC 
   ${SPNG_DIR}/spng/
   ${MINIZ_DIR}/
   ${MINIZ_PORT}/
)

# Add the standard library to the build
target_link_libraries(SPNG PUBLIC 
    pico_stdlib
)

target_compile_definitions(SPNG PUBLIC 
	SPNG_USE_MINIZ=1)