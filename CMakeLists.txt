cmake_minimum_required(VERSION 2.8.0)

PROJECT(make-emu)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    )

SET(SRC main.cpp
        actionprocessor.cpp
        action.cpp
        fileparser.cpp
        target.cpp
        )

SET(HDR actionprocessor.h
        action.h
        fileparser.h
        target.h
        digraph.hpp
        )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET(BINNAME make-emu)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

ADD_EXECUTABLE(${BINNAME} ${SRC} ${HDR})
TARGET_LINK_LIBRARIES(${BINNAME} ${LIBS})

