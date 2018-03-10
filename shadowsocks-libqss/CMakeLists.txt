set(TASK "shadowsocks-libqss")

set(SOURCE
    client.cpp
    main.cpp
    utils.cpp
    )

add_definitions(-DUNICODE)
include(service/CMakeLists.txt)

add_executable(${TASK} ${SOURCE})

target_link_libraries(${TASK} Qt5::Core Qt5::Network QtShadowsocks)
target_include_directories(${TASK}
    PUBLIC ${PROJECT_SOURCE_DIR}/lib ${CMAKE_CURRENT_SOURCE_DIR}/service)

install(TARGETS ${TASK} RUNTIME DESTINATION bin)
