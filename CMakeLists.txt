cmake_minimum_required(VERSION 3.4...3.18)
project(PyPartMC)

enable_language(C)
enable_language(CXX)
enable_language(Fortran)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 14)

add_library(partmclib STATIC
  src/fake_mpi.F90
  src/fake_netcdf.F90
  partmc/src/condense_solver.c
  partmc/src/aero_state.F90
  partmc/src/integer_varray.F90
  partmc/src/integer_rmap.F90
  partmc/src/integer_rmap2.F90
  partmc/src/aero_sorted.F90
  partmc/src/aero_binned.F90
  partmc/src/bin_grid.F90
  partmc/src/constants.F90
  partmc/src/scenario.F90
  partmc/src/env_state.F90
  partmc/src/aero_mode.F90
  partmc/src/aero_dist.F90
  partmc/src/aero_weight.F90
  partmc/src/aero_weight_array.F90
  partmc/src/coag_kernel_additive.F90
  partmc/src/coag_kernel_sedi.F90
  partmc/src/coag_kernel_constant.F90
  partmc/src/coag_kernel_brown.F90
  partmc/src/coag_kernel_zero.F90
  partmc/src/coag_kernel_brown_free.F90
  partmc/src/coag_kernel_brown_cont.F90
  partmc/src/aero_data.F90
  partmc/src/run_exact.F90
  partmc/src/run_part.F90
  partmc/src/util.F90
  partmc/src/stats.F90
  partmc/src/run_sect.F90
  partmc/src/output.F90
  partmc/src/mosaic.F90 
  partmc/src/gas_data.F90 
  partmc/src/gas_state.F90
  partmc/src/coagulation.F90
  partmc/src/exact_soln.F90 
  partmc/src/coagulation_dist.F90
  partmc/src/coag_kernel.F90
  partmc/src/spec_line.F90
  partmc/src/spec_file.F90
  partmc/src/rand.F90
  partmc/src/aero_particle.F90 
  partmc/src/aero_particle_array.F90 
  partmc/src/mpi.F90
  partmc/src/netcdf.F90 
  partmc/src/aero_info.F90 
  partmc/src/aero_info_array.F90
  partmc/src/nucleate.F90 
  partmc/src/condense.F90 
  partmc/src/fractal.F90 
  partmc/src/chamber.F90
  partmc/src/camp_interface.F90
  partmc/src/photolysis.F90
)

target_compile_definitions(partmclib PRIVATE PMC_USE_SUNDIALS="1")
target_compile_definitions(partmclib PRIVATE PMC_USE_MPI="1")

macro(add_prefix prefix rootlist)
    set(outlist)
    foreach(root ${${rootlist}})
        list(APPEND outlist ${prefix}${root})
    endforeach()
    set(${rootlist} ${outlist})
endmacro(add_prefix)

macro(sundials_option NAME TYPE DOCSTR DEFAULT_VALUE)
  set(options DEPENDS_ON_THROW_ERROR ADVANCED)   # macro options
  set(multiValueArgs OPTIONS SHOW_IF DEPENDS_ON) # macro keyword inputs followed by multiple values
  cmake_parse_arguments(sundials_option "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
endmacro()

macro(sundials_add_library target)
    set(options STATIC_ONLY SHARED_ONLY OBJECT_LIB_ONLY)
    set(oneValueArgs INCLUDE_SUBDIR OUTPUT_NAME VERSION SOVERSION)
    set(multiValueArgs SOURCES HEADERS OBJECT_LIBRARIES LINK_LIBRARIES INCLUDE_DIRECTORIES COMPILE_DEFINITIONS COMPILE_OPTIONS COMPILE_FEATURES PROPERTIES)
    cmake_parse_arguments(sundials_add_library "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    add_library(${target} STATIC ${sundials_add_library_SOURCES})
    set_target_properties(${target} PROPERTIES VISIBILITY_INLINES_HIDDEN FALSE)
    target_compile_definitions(${target} PRIVATE SUNDIALS_STATIC_DEFINE)
    target_include_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/sundials/src/sundials)
    target_include_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/sundials/include)
    target_include_directories(${target} PRIVATE ${CMAKE_BINARY_DIR}/include)
endmacro()

function(print_error)
endfunction()

function(scoped_sundials_setup_config)
    # TODO: enforce git git from submodule!
    set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/sundials)
    set(SUNDIALS_PRECISION "double")
    set(SUNDIALS_CINDEX_TYPE "int64_t")
    set(PACKAGE_VERSION_MAJOR "5")
    set(PACKAGE_VERSION_MINOR "8")
    set(PACKAGE_VERSION_PATCH "0")
    #set(BUILD_SHARED_LIBS TRUE)
    set(PACKAGE_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
    include(sundials/cmake/SundialsSetupConfig.cmake)
endfunction()
scoped_sundials_setup_config()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/sundials/cmake)
include(sundials/cmake/SundialsSetupCompilers.cmake)

set(SUNDIALS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/sundials")
add_subdirectory(sundials/src/cvode/)
add_subdirectory(sundials/src/sunmatrix)
add_subdirectory(sundials/src/sunlinsol)
add_subdirectory(sundials/src/sunnonlinsol)
add_subdirectory(sundials/src/nvector)
add_subdirectory(sundials/src/sundials)

add_subdirectory(pybind11)

pybind11_add_module(PyPartMC 
  src/pypartmc.cpp
  src/run_part.F90
  src/run_part_opt.F90
  src/util.F90
  src/aero_data.F90
  src/aero_state.F90
  src/env_state.F90
  src/gas_data.F90
  src/gas_state.F90
  src/scenario.F90
)
add_dependencies(partmclib 
  sundials_cvode
  sundials_nvecserial 
  sundials_generic
  sundials_sunmatrixband 
  sundials_sunmatrixdense 
  sundials_sunlinsolband 
  sundials_sunnonlinsolnewton 
  sundials_sunlinsolspgmr
)
add_dependencies(PyPartMC partmclib)
target_include_directories(PyPartMC PRIVATE ${CMAKE_SOURCE_DIR}/json/include ${CMAKE_SOURCE_DIR}/pybind11_json/include)
target_include_directories(partmclib PRIVATE ${CMAKE_SOURCE_DIR}/sundials/include ${CMAKE_BINARY_DIR}/include)
target_compile_definitions(PyPartMC PRIVATE VERSION_INFO=${VERSION_INFO})
target_link_libraries(partmclib PRIVATE 
  sundials_cvode
  sundials_nvecserial
  sundials_generic
  sundials_sunmatrixband
  sundials_sunmatrixdense
  sundials_sunlinsolband
  sundials_sunnonlinsolnewton 
  sundials_sunlinsolspgmr
)
target_link_libraries(PyPartMC PRIVATE partmclib)
