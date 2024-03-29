cmake_minimum_required(VERSION 3.13)
project(matslise2d LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/matslise/cmake)
cmake_policy(SET CMP0077 NEW)

set(MATSLISE2D_VERSION "0.0.1")

set(CMAKE_CXX_STANDARD 17)

# -fPIC needed for pybind
set(BUILD_SHARED_LIBS ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-Wall -Wextra -Wno-undefined-var-template)
endif ()

set(libraries matslise Eigen3::Eigen)

set(MATSLISE2D_SRC
        matslise2d/util/quadrature.cpp
        matslise2d/matslise2d/halfrange.cpp matslise2d/matslise2d/eigenfunctions.cpp
        matslise2d/matslise2d/matslise2d.cpp matslise2d/matslise2d/sector.cpp # matslise2d/matslise2d/basisQuadrature.cpp
        matslise2d/matslise2d/basisQuadrature_lobatto.cpp
        matslise2d/matsliseNd/sector.cpp
        matslise2d/matslise3d/matslise3d.cpp matslise2d/matslise3d/sector.cpp)

set(PYSLISE2D_SRC matslise2d/pyslise2d/module.cpp matslise2d/pyslise2d/pyslise2d.cpp matslise2d/pyslise2d/pyslise3d.cpp)


set(MATSLISE2D_TEST_SRC
        test/main.cpp
        test/matslise2d/henon.cpp test/matslise2d/ixaru.cpp test/matslise2d/zero.cpp test/matslise2d/quartic.cpp
        test/matslise2d/derivatives.cpp test/matslise2d/coulomb.cpp test/matslise2d/pentagon.cpp test/matslise2d/twoSextic.cpp
        test/matslise2d/quadrature.cpp
        test/matslise3d/coulomb.cpp test/matslise3d/harmonic_oscillator.cpp
        test/util/quadrature.cpp)

add_library(matslise2d ${MATSLISE2D_SRC})

SET(MATSLISE_STATIC ON)
SET(MATSLISE_MATSCS ON)
SET(MATSLISE_PYTHON OFF)

OPTION(MATSLISE2D_LONG_DOUBLE "Compile with support for long double" OFF)
set(MATSLISE_LONG_DOUBLE ${MATSLISE2D_LONG_DOUBLE})
if (MATSLISE2D_LONG_DOUBLE)
    add_definitions(-DMATSLISE_LONG_DOUBLE -DMATSLISE2D_LONG_DOUBLE)
    message("-- with support for long double")
else ()
    message("-- without support for long double")
endif ()

OPTION(MATSLISE2D_QUADMATH "Compile with support for float128" OFF)
set(MATSLISE_QUADMATH ${MATSLISE2D_QUADMATH})
if (MATSLISE2D_QUADMATH)
    find_package(Boost REQUIRED)
    if (Boost_FOUND)
        add_definitions(-DBOOST)
        include_directories(${Boost_INCLUDE_DIRS})

        find_package(Quadmath REQUIRED)

        add_definitions(-DMATSLISE_QUADMATH -DMATSLISE2D_QUADMATH)
        list(APPEND libraries Quadmath::quadmath)
    endif ()
    message("-- with support for float128")
else ()
    message("-- without support for float128")
endif ()

add_subdirectory(lib/matslise)
include_directories(lib/matslise)
set_property(TARGET matslise PROPERTY POSITION_INDEPENDENT_CODE ON)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

target_link_libraries(matslise2d PUBLIC ${libraries})

add_executable(matslise2d_test ${MATSLISE2D_TEST_SRC})

target_link_libraries(matslise2d_test PRIVATE matslise2d)
add_dependencies(matslise2d_test matslise2d)

enable_testing()
add_test(NAME matslise2d_test COMMAND matslise2d_test)


OPTION(MATSLISE2D_PYTHON "Also build the python bindings" ON)
if (MATSLISE2D_PYTHON)
    if (EXISTS ${PROJECT_SOURCE_DIR}/lib/pybind11/CMakeLists.txt)
        add_subdirectory(lib/pybind11)
    else ()
        find_package(pybind11 QUIET)
    endif ()

    if (pybind11_FOUND)
        message("-- python: ${PYTHON_EXECUTABLE}")
        pybind11_add_module(pyslise2d MODULE ${MATSLISE2D_SRC} ${PYSLISE2D_SRC})
        target_link_libraries(pyslise2d PRIVATE ${libraries})
        IF (PARALLEL)
            target_link_libraries(pyslise2d PUBLIC OpenMP::OpenMP_CXX)
        ENDIF (PARALLEL)

        configure_file(matslise2d/pyslise2d/setup.py setup.py NEWLINE_STYLE LF)
        configure_file(matslise2d/pyslise2d/description.md description.md NEWLINE_STYLE LF)
        add_custom_target(build_wheel
                COMMAND ${CMAKE_COMMAND} -E env PYSLISE2D_LIBRARY="$<TARGET_FILE:pyslise2d>"
                ${PYTHON_EXECUTABLE} setup.py bdist_wheel
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        add_dependencies(build_wheel pyslise2d)

        add_custom_target(pyslise2d_install
                # COMMAND ${PYTHON_EXECUTABLE} -m pip uninstall -y --verbose pyslise2d
                COMMAND ${CMAKE_COMMAND} -E env PYSLISE2D_LIBRARY="$<TARGET_FILE:pyslise2d>"
                ${PYTHON_EXECUTABLE} -m pip install --upgrade --ignore-installed .
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        add_dependencies(pyslise2d_install pyslise2d)

    else ()
        message("To enable python bindings, make sure pybind11 is available in /lib/pybind11. Or disable the python bindings with -DSCHRODINGER_PYTHON=OFF")
    endif ()
endif ()
