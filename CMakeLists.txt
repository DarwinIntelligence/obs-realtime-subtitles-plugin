cmake_minimum_required(VERSION 3.16...3.26)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(${_name} VERSION ${_version})

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" ON)
option(ENABLE_QT "Use Qt functionality" ON)


include(compilerconfig)
include(defaults)
include(helpers)
include_directories(include)

add_library(${CMAKE_PROJECT_NAME} MODULE)

add_definitions(-DASIO_STANDALONE)
add_definitions(-DASIO_HAS_STD_ADDRESSOF)
add_definitions(-DASIO_HAS_STD_ARRAY)
add_definitions(-DASIO_HAS_CSTDINT)
add_definitions(-DASIO_HAS_STD_SHARED_PTR)
add_definitions(-DASIO_HAS_STD_TYPE_TRAITS)
add_definitions(-DASIO_HAS_VARIADIC_TEMPLATES)
add_definitions(-DASIO_HAS_STD_FUNCTION)
add_definitions(-DASIO_HAS_STD_CHRONO)
add_definitions(-DBOOST_ALL_NO_LIB)
include_directories(src/plugin src/window src/transcript_plugin src/util lib .deps/obs-deps-2024-03-19-x64/include)

# Set the module linker flags with proper quoting

find_package(libobs REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

if(ENABLE_FRONTEND_API)
  find_package(obs-frontend-api REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)
endif()

if(ENABLE_QT)
  find_package(Qt6 COMPONENTS Widgets Core)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)
  target_compile_options(
    ${CMAKE_PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-quoted-include-in-framework-header
                                  -Wno-comma>)
  set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES AUTOMOC ON
               AUTOUIC ON
               AUTORCC ON)
endif()

#set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")

#find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)
#link_directories("C:/Program Files/OpenSSL-Win64/lib/VC/x64/MD") 
#target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")

# Manually specify the exact path to OpenSSL libraries
set(OPENSSL_CRYPTO_LIBRARY "C:/Program Files/OpenSSL-Win64/lib/VC/x64/MD/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "C:/Program Files/OpenSSL-Win64/lib/VC/x64/MD/libssl.lib")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})





file(GLOB Window src/window/*.cpp)

file(GLOB Plugin src/plugin/*.cpp)

file(GLOB Transcript src/transcript_plugin/*.cpp)

file(GLOB CFile src/transcript_plugin/*.c)

file(GLOB Deepgram src/deepgram/*.cpp)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /wd4267)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE src/obs_realtime_subtitles_plugin.cpp ${Plugin} ${Window} ${Transcript}
                                             ${CFile} ${Deepgram})
set_target_properties_plugin(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name})
