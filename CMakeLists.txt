cmake_minimum_required(VERSION 3.11)
project(IPFE-RLWE C)

# C compiler settings
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Wredundant-decls \
-Wnested-externs -Wmissing-include-dirs -Wstrict-prototypes -Wwrite-strings \
-Wno-unused-parameter -Wshadow -Wold-style-definition")

# GCC warnings that Clang doesn't provide
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wjump-misses-init -Wlogical-op")
endif ()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O2")

# Header files
include_directories(
        src
	/opt/homebrew/Cellar/gmp/6.3.0/include/
        )

link_directories(
	/opt/homebrew/Cellar/gmp/6.3.0/lib
)

# Library sources
set(library_SOURCES
	src/aes256ctr.c
	src/arith_rns.c
	src/crt.c
	src/gauss.c
	src/ntt.c
	src/randombytes.c
	src/rlwe_sife.c
	src/sample.c
)

set(library_HEADERS
	src/rlwe_sife.h
	src/params.h
	src/aes256ctr.h
	src/arith_rns.h
	src/consts.h
	src/crt.h
	src/gauss.h
	src/ntt.h
	src/randombytes.h
	src/sample.h
)

add_library(ipfe_rlwe SHARED ${library_SOURCES} ${library_HEADERS})

# Link libraries that are used in our library
target_link_libraries(ipfe_rlwe gmp)

target_include_directories(ipfe_rlwe PUBLIC
  $<INSTALL_INTERFACE:src/*.h>  # <prefix>/include/cifer
)

# Install library and copy header to install dir
install(TARGETS ipfe_rlwe DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

add_custom_target(docs COMMAND doxygen WORKING_DIRECTORY .. VERBATIM)
