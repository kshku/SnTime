set(SRCS
    test.c
)

add_executable(sn_time_test)
target_sources(sn_time_test PRIVATE ${SRCS})
target_link_libraries(sn_time_test PRIVATE sntime)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND SN_TIME_BUILD_SHARED)
    add_custom_target(copy_dlls ALL
        COMMENT "Copy the dlls"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLLS:sn_time_test>" "$<TARGET_FILE_DIR:sn_time_test>"
        COMMAND_EXPAND_LISTS
    )

    add_dependencies(copy_dlls sn_time_test)
endif()

