if(SN_TIME_BUILD_SHARED)
    add_library(sntime SHARED)
    target_compile_definitions(sntime PRIVATE SN_EXPORT)
else()
    add_library(sntime STATIC)
    target_compile_definitions(sntime PRIVATE SN_TIME_STATIC)
endif()

target_include_directories(sntime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(sntime PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(sntime PRIVATE sn_time_configs)

add_subdirectory(src)
