cmake_minimum_required(VERSION 3.0.0)
project(gencore VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_EXE_LINKER_FLAGS "-static")

file(GLOB JSONCPP_SOURCES "3rdparty/jsoncpp/src/lib_json/*.cpp")
file(GLOB JSONCPP_HEADERS "3rdparty/jsoncpp/include/json/*.h")

set(ALL_SOURCES
    main.cpp
    core/dumper.cpp
    core/elfheader.cpp
    core/programheader.cpp
    core/note.cpp
    core/prstatus.cpp
    core/noteprogramheader.cpp
    core/vmaprogramheader.cpp
    core/vma.cpp
    core/coregenerator.cpp
    core/aligningdumper.cpp
    core/log.cpp
    3rdparty/cpp-base64/base64.cpp
    ${JSONCPP_SOURCES}
)

set(ALL_HEADERS
    core/include/dumper.h
    core/include/elfheader.h
    core/include/programheader.h
    core/include/note.h
    core/include/prstatus.h
    core/include/elfcore.h
    core/include/noteprogramheader.h
    core/include/vmaprogramheader.h
    core/include/vma.h
    core/include/coregenerator.h
    core/include/aligningdumper.h
    core/include/log.h
    3rdparty/cpp-base64/base64.h
    3rdparty/cmdline/cmdline.h
    ${JSONCPP_HEADERS}
)

include_directories(core/include)
include_directories(3rdparty/cpp-base64)
include_directories(3rdparty/cmdline)
include_directories(3rdparty/jsoncpp/include)

add_executable(gencore ${ALL_SOURCES} ${ALL_HEADERS})
target_link_libraries(gencore z)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
