add_library(noiseprotobufs STATIC protobufs.c)

target_include_directories(noiseprotobufs PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_compile_options(noiseprotobufs PRIVATE -Wall)

set_target_properties(noiseprotobufs PROPERTIES OUTPUT_NAME noiseprotobufs)
install(TARGETS noiseprotobufs ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
