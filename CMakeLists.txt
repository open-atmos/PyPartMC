####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

cmake_minimum_required(VERSION 3.4...3.18)
project(PyPartMC LANGUAGES C CXX Fortran)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# TODO
#set(CMAKE_Fortran_STANDARD 2008)
#set(CMAKE_Fortran_STANDARD_REQUIRED YES)
#set(CMAKE_Fortran_EXTENSIONS OFF)

if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
  add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-fimplicit-none>)
endif()

macro(add_prefix prefix rootlist)
  set(outlist)
  foreach(root ${${rootlist}})
    list(APPEND outlist ${prefix}${root})
  endforeach()
  set(${rootlist} ${outlist})
endmacro(add_prefix)

### sources ########################################################################################

set(PyPartMC_sources 
  pypartmc.cpp gimmicks.cpp fake_netcdf.cpp fake_mpi.cpp fake_spec_file.cpp
  run_part.F90 run_part_opt.F90 util.F90 aero_data.F90 aero_state.F90 env_state.F90 gas_data.F90 
  gas_state.F90 scenario.F90 condense.F90
)
add_prefix(src/ PyPartMC_sources)

set(partmclib_SOURCES condense_solver.c aero_state.F90 integer_varray.F90 integer_rmap.F90 
  integer_rmap2.F90 aero_sorted.F90 aero_binned.F90 bin_grid.F90 constants.F90 scenario.F90
  env_state.F90 aero_mode.F90 aero_dist.F90 aero_weight.F90 aero_weight_array.F90 
  coag_kernel_additive.F90 coag_kernel_sedi.F90 coag_kernel_constant.F90 coag_kernel_brown.F90 
  coag_kernel_zero.F90 coag_kernel_brown_free.F90 coag_kernel_brown_cont.F90 aero_data.F90 
  run_exact.F90 run_part.F90 util.F90 stats.F90 run_sect.F90 output.F90 mosaic.F90 gas_data.F90
  gas_state.F90 coagulation.F90 exact_soln.F90 coagulation_dist.F90 coag_kernel.F90 spec_line.F90 
  rand.F90 aero_particle.F90 aero_particle_array.F90 mpi.F90 netcdf.F90 aero_info.F90 
  aero_info_array.F90 nucleate.F90 condense.F90 fractal.F90 chamber.F90 camp_interface.F90
  photolysis.F90
)
add_prefix(gitmodules/partmc/src/ partmclib_SOURCES)
list(APPEND partmclib_SOURCES src/fake_mpi.F90 src/fake_netcdf.F90 src/fake_spec_file.F90)

### SUNDIALS #######################################################################################

set(SUNDIALS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/gitmodules/sundials")

macro(sundials_option NAME TYPE DOCSTR DEFAULT_VALUE)
  set(options DEPENDS_ON_THROW_ERROR ADVANCED)   # macro options
  set(multiValueArgs OPTIONS SHOW_IF DEPENDS_ON) # macro keyword inputs followed by multiple values
  cmake_parse_arguments(sundials_option "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
endmacro()

macro(sundials_add_library target)
  set(options STATIC_ONLY SHARED_ONLY OBJECT_LIB_ONLY)
  set(oneValueArgs INCLUDE_SUBDIR OUTPUT_NAME VERSION SOVERSION)
  set(multiValueArgs SOURCES HEADERS OBJECT_LIBRARIES LINK_LIBRARIES INCLUDE_DIRECTORIES)
  cmake_parse_arguments(sundials_add_library 
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN}
  )
  add_library(${target} STATIC ${sundials_add_library_SOURCES})
  target_compile_definitions(${target} PRIVATE SUNDIALS_STATIC_DEFINE)
  target_include_directories(${target} PRIVATE ${SUNDIALS_SOURCE_DIR}/src/sundials)
  target_include_directories(${target} PRIVATE ${SUNDIALS_SOURCE_DIR}/include)
  target_include_directories(${target} PRIVATE ${CMAKE_BINARY_DIR}/include)
endmacro()

function(print_error)
endfunction()

function(scoped_sundials_setup_config)
  set(PROJECT_SOURCE_DIR ${SUNDIALS_SOURCE_DIR})
  set(SUNDIALS_PRECISION "double")
  set(SUNDIALS_CINDEX_TYPE "int64_t")
  # TODO: read from submodule files!
  set(PACKAGE_VERSION_MAJOR "5")
  set(PACKAGE_VERSION_MINOR "8")
  set(PACKAGE_VERSION_PATCH "0")
  set(PACKAGE_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
  include(${SUNDIALS_SOURCE_DIR}/cmake/SundialsSetupConfig.cmake)
endfunction()
scoped_sundials_setup_config()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SUNDIALS_SOURCE_DIR}/cmake)
include(${SUNDIALS_SOURCE_DIR}/cmake/SundialsSetupCompilers.cmake)

foreach(item cvode;sunmatrix;sunlinsol;sunnonlinsol;nvector;sundials)
    add_subdirectory(${SUNDIALS_SOURCE_DIR}/src/${item})
endforeach()

set(SUNDIALS_items 
  cvode nvecserial
  sunmatrixband
  sunmatrixdense
  sunlinsolband 
  sunnonlinsolnewton 
  sunlinsolspgmr 
  generic
)
add_prefix(sundials_ SUNDIALS_items)

### partmclib ######################################################################################

add_library(partmclib STATIC ${partmclib_SOURCES})
target_compile_definitions(partmclib PRIVATE PMC_USE_SUNDIALS="1")
target_compile_definitions(partmclib PRIVATE PMC_USE_MPI="1")
add_dependencies(partmclib ${SUNDIALS_items})
target_include_directories(partmclib PRIVATE ${SUNDIALS_SOURCE_DIR}/include)
target_include_directories(partmclib PRIVATE ${CMAKE_BINARY_DIR}/include)
target_link_libraries(partmclib PRIVATE ${SUNDIALS_items})

### PYBIND11 & PyPartMC ############################################################################

add_subdirectory(gitmodules/pybind11)
pybind11_add_module(PyPartMC ${PyPartMC_sources})
add_dependencies(PyPartMC partmclib)
target_include_directories(PyPartMC PRIVATE 
  ${CMAKE_SOURCE_DIR}/gitmodules/json/include 
  ${CMAKE_SOURCE_DIR}/gitmodules/pybind11_json/include
  ${CMAKE_SOURCE_DIR}/gitmodules/span/include
  ${CMAKE_SOURCE_DIR}/gitmodules/string_view-standalone/include
)
target_compile_definitions(PyPartMC PRIVATE VERSION_INFO=${VERSION_INFO})
target_link_libraries(PyPartMC PRIVATE partmclib)

### pedantics ######################################################################################

foreach(target PyPartMC)  # TODO: the same for partmclib
  target_compile_options(${target} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
    #$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-compare-reals> # TODO
    #$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-unused-dummy-argument> # TODO
    #$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-unused-variable>  # TODO
    #$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-c-binding-type>  # TODO
    #$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-maybe-uninitialized> # TODO
    #$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-function-elimination> # TODO
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-unused-parameter> # TODO
    #$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-surprising> # TODO
  )
endforeach()

