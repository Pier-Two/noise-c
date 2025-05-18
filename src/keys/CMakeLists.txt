add_library(noisekeys STATIC
    certificate.c
    loader.c
)

target_include_directories(noisekeys PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/noise/keys>
)

target_compile_options(noisekeys PRIVATE -Wall)

set_target_properties(noisekeys PROPERTIES OUTPUT_NAME noisekeys)
install(TARGETS noisekeys ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
