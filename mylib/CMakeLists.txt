project(mylib)

set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    ${SOURCE_DIR}/Media.cpp
    ${SOURCE_DIR}/Client.cpp
    ${SOURCE_DIR}/MediaLibrary.cpp
    ${SOURCE_DIR}/Console.cpp
    ${SOURCE_DIR}/UseThatHelpingFunction.cpp
    ${SOURCE_DIR}/DoThatFunction.cpp
)

set(HEADERS
    ${HEADER_DIR}/Media.h
    ${HEADER_DIR}/Client.h
    ${HEADER_DIR}/MediaLibrary.h
    ${HEADER_DIR}/Console.h
    ${HEADER_DIR}/UseThatHelpingFunction.h
    ${HEADER_DIR}/DoThatFunction.h
)

add_library(${PROJECT_NAME}
STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${HEADER_DIR}>
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Libraries")