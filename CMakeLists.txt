cmake_minimum_required(VERSION 3.13)
project(respack)

set(SRC_FILES
    main.cpp
    pack.cpp
    File.cpp
    ResourceObject.cpp
    PngResource.cpp
)

if (NOT DEFINED LIBPNG_DIR)
    message(FATAL_ERROR "Missing libpng directory. Set with -D LIBPNG_DIR=path")
endif (NOT DEFINED LIBPNG_DIR)

if (NOT DEFINED ZLIB_DIR)
    message(FATAL_ERROR "Missing zlib directory. Set with -D ZLIB_DIR=path")
endif (NOT DEFINED ZLIB_DIR)

set(LIBRARIES
    ${LIBPNG_DIR}/lib/libpng.lib
    ${ZLIB_DIR}/lib/zlib.lib
)

set(INCLUDE_DIRS
    ${LIBPNG_DIR}/include
    ${ZLIB_DIR}/include
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ..)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ..)

if(MSVC)
    set(CMAKE_CXX_FLAGS "/W2 /EHsc")
endif(MSVC)

add_executable(${PROJECT_NAME} ${OPTIONS} ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX debug)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARIES})
