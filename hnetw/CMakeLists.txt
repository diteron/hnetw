cmake_minimum_required(VERSION 3.14)

if (WIN32)
    project(hnetw LANGUAGES CXX)
#elseif(UNIX)
#    project(hnetw)
endif()

set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

#=========================== Qt setup ===========================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)

#=========================== Project files ===========================#
set(CAPTURE_DIR "${PROJECT_SOURCE_DIR}/capture")
set(UI_DIR "${PROJECT_SOURCE_DIR}/ui")
set(UI_UTILS_DIR "${PROJECT_SOURCE_DIR}/ui/utils")
set(UI_MODELS_DIR "${PROJECT_SOURCE_DIR}/ui/qtmodels")
set(NETW_DIR "${PROJECT_SOURCE_DIR}/network")
set(PACKET_DIR "${PROJECT_SOURCE_DIR}/packet")
set(PACK_CLASS_DIR "${PROJECT_SOURCE_DIR}/packet/packets")
set(PACK_TREE_DIR "${PROJECT_SOURCE_DIR}/packet/packetstrees")

file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${CAPTURE_DIR}/*.cpp"
    "${UI_DIR}/*.cpp"
    "${UI_UTILS_DIR}/*.cpp"
    "${UI_MODELS_DIR}/*.cpp"
    "${NETW_DIR}/*.cpp"
    "${PACKET_DIR}/*.cpp"
    "${PACK_TREE_DIR}/*.cpp"
    "${PACK_CLASS_DIR}/*.cpp"
)

include_directories(${PROJECT_SOURCE_DIR}
                    ${CAPTURE_DIR}
                    ${UI_DIR}
                    ${UI_UTILS_DIR}
                    ${UI_MODELS_DIR}                    
                    ${NETW_DIR}
                    ${PACKET_DIR}
                    ${PACK_TREE_DIR}
                    ${PACK_CLASS_DIR})

#=========================== Executable setup ===========================#
if (WIN32)
    add_compile_definitions(_WINSOCKAPI_)
    add_compile_definitions( _CRT_SECURE_NO_WARNINGS)
    add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})
#elseif(UNIX)
#    add_executable(${PROJECT_NAME} ${SRC_FILES})
endif()

target_precompile_headers(${PROJECT_NAME} PRIVATE <QtWidgets>)
target_link_libraries(${PROJECT_NAME} Qt6::Widgets)

if (WIN32)
    target_precompile_headers(${PROJECT_NAME} PRIVATE <winsock2.h>
                                                      <WS2tcpip.h>
                                                      <mstcpip.h>)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
#elseif(UNIX)
#    
endif()

if (WIN32) 
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/bin)
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
endif()


# Expose project public includes to other subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
                                 ${CAPTURE_DIR}
                                 ${UI_DIR}
                                 ${UI_UTILS_DIR}
                                 ${UI_MODELS_DIR}                    
                                 ${NETW_DIR}
                                 ${PACKET_DIR}
                                 ${PACK_TREE_DIR}
                                 ${PACK_CLASS_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
