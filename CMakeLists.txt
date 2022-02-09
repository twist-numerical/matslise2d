cmake_minimum_required(VERSION 3.13)
project(matslise2d LANGUAGES CXX)
cmake_policy(SET CMP0077 NEW)

set(MATSLISE2D_VERSION "0.0.1")

set(CMAKE_CXX_STANDARD 17)

# -fPIC needed for pybind
set(BUILD_SHARED_LIBS ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-Wall -Wextra -Werror -Wno-undefined-var-template)
endif ()

set(MATSLISE2D_SRC
        matslise2d/matslise2d/halfrange.cpp matslise2d/matslise2d/eigenfunctions.cpp
        matslise2d/matslise2d/matslise2d.cpp matslise2d/matslise2d/sector.cpp matslise2d/matslise2d/basisQuadrature.cpp
        matslise2d/matsliseNd/sector.cpp
        matslise2d/matslise3d/matslise3d.cpp matslise2d/matslise3d/sector.cpp)

set(PYSLISE2D_SRC matslise2d/pyslise2d/module.cpp matslise2d/pyslise2d/pyslise2d.cpp matslise2d/pyslise2d/pyslise3d.cpp)


set(MATSLISE2D_TEST_SRC
        test/main.cpp
        test/matslise2d/henon.cpp test/matslise2d/ixaru.cpp test/matslise2d/zero.cpp test/matslise2d/quartic.cpp
        test/matslise2d/derivatives.cpp test/matslise2d/coulomb.cpp test/matslise2d/pentagon.cpp test/matslise2d/twoSextic.cpp
        test/matslise2d/quadrature.cpp
        test/matslise3d/coulomb.cpp test/matslise3d/harmonic_oscillator.cpp
        test/util/quadrature.cpp test/util/calculateEta.cpp test/util/rectangle.cpp test/util/heap.cpp)

add_library(matslise2d ${MATSLISE2D_SRC})

SET(MATSLISE_STATIC ON)
SET(MATSLISE_MATSCS ON)
SET(MATSLISE_2D OFF)
SET(MATSLISE_3D OFF)
SET(MATSLISE_PYTHON OFF)

OPTION(MATSLISE2D_LONG_DOUBLE "Compile with support for long double" OFF)
if (MATSLISE2D_LONG_DOUBLE)
    add_definitions(-DMATSLISE2D_LONG_DOUBLE)
    message("-- with support for long double")
    set(LONG_DOUBLE ON)
else ()
    set(LONG_DOUBLE OFF)
    message("-- without support for long double")
endif ()

add_subdirectory(lib/matslise)
include_directories(lib/matslise)
set_property(TARGET matslise PROPERTY POSITION_INDEPENDENT_CODE ON)

set(libraries matslise)

target_link_libraries(matslise2d PUBLIC ${libraries})

add_executable(matslise2d_test ${MATSLISE2D_TEST_SRC})

target_link_libraries(matslise2d_test PRIVATE matslise2d)
add_dependencies(matslise2d_test matslise2d)

enable_testing()
add_test(NAME matslise2d_test COMMAND matslise2d_test)


OPTION(MATSLISE2D_PYTHON "Also build the python bindings" ON)
if (MATSLISE2D_PYTHON)
    if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/pybind11)
        add_subdirectory(lib/pybind11)
    else ()
        find_package(pybind11 QUIET)
    endif ()

    if (pybind11_FOUND)

    else ()
        message("To enable python bindings, make sure pybind11 is available in /lib/pybind11. Or disable the python bindings with -DSCHRODINGER_PYTHON=OFF")
    endif ()
endif ()
