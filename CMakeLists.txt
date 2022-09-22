####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

cmake_minimum_required(VERSION 3.4...3.18)
project(_PyPartMC LANGUAGES C CXX Fortran)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
  add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-fimplicit-none>)
  add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-ffree-line-length-none>)
  # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58175
  add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-Wno-surprising>)
endif()

macro(add_prefix prefix rootlist)
  set(outlist)
  foreach(root ${${rootlist}})
    list(APPEND outlist ${prefix}${root})
  endforeach()
  set(${rootlist} ${outlist})
endmacro(add_prefix)

add_definitions("-DSuiteSparse_long=long")
add_definitions("-DSuiteSparse_long_max=LONG_MAX")
add_definitions("-DSuiteSparse_long_idd=ld")
add_definitions("-DSUNDIALS_INT64_T=1")

### sources ########################################################################################

set(PyPartMC_sources 
  pypartmc.cpp gimmicks.cpp fake_netcdf.cpp fake_spec_file.cpp sys.cpp
  run_part.F90 run_part_opt.F90 util.F90 aero_data.F90 aero_state.F90 env_state.F90 gas_data.F90 
  gas_state.F90 scenario.F90 condense.F90 aero_particle.F90 bin_grid.F90
  camp_core.F90 photolysis.F90
)
add_prefix(src/ PyPartMC_sources)

set(camp_SOURCES
  Jacobian.c 
  aero_phase_data.F90 
  aero_phase_solver.c 
  aero_rep_data.F90
  aero_rep_solver.c 
  aero_rep_factory.F90
  camp_core.F90
  camp_solver_data.F90 
  camp_solver.c 
  camp_state.F90 
  chem_spec_data.F90
  constants.F90 
  debug_diff_check.F90
  env_state.F90
  mechanism_data.F90
  mpi.F90
  property.F90
  rand.F90
  rxn_data.F90 
  rxn_factory.F90
  rxn_solver.c 
  solver_stats.F90
  sub_model_data.F90
  sub_model_solver.c 
  sub_model_factory.F90
  time_derivative.c
  util.F90 
  aero_reps/aero_rep_modal_binned_mass.F90
  aero_reps/aero_rep_single_particle.F90
  aero_reps/aero_rep_modal_binned_mass.c
  aero_reps/aero_rep_single_particle.c
  rxns/rxn_aqueous_equilibrium.c 
  rxns/rxn_aqueous_equilibrium.F90
  rxns/rxn_ternary_chemical_activation.c 
  rxns/rxn_ternary_chemical_activation.F90
  rxns/rxn_arrhenius.c
  rxns/rxn_arrhenius.F90
  rxns/rxn_CMAQ_H2O2.c 
  rxns/rxn_CMAQ_H2O2.F90
  rxns/rxn_CMAQ_OH_HNO3.c 
  rxns/rxn_CMAQ_OH_HNO3.F90
  rxns/rxn_condensed_phase_arrhenius.c 
  rxns/rxn_condensed_phase_arrhenius.F90
  rxns/rxn_emission.c 
  rxns/rxn_emission.F90
  rxns/rxn_first_order_loss.c 
  rxns/rxn_first_order_loss.F90
  rxns/rxn_HL_phase_transfer.c 
  rxns/rxn_HL_phase_transfer.F90
  rxns/rxn_photolysis.c 
  rxns/rxn_photolysis.F90
  rxns/rxn_SIMPOL_phase_transfer.c 
  rxns/rxn_SIMPOL_phase_transfer.F90
  rxns/rxn_troe.c 
  rxns/rxn_troe.F90
  rxns/rxn_wennberg_no_ro2.c 
  rxns/rxn_wennberg_no_ro2.F90
  rxns/rxn_wennberg_tunneling.c 
  rxns/rxn_wennberg_tunneling.F90
  rxns/rxn_wet_deposition.c 
  rxns/rxn_wet_deposition.F90
  sub_models/sub_model_PDFiTE.c
  sub_models/sub_model_PDFiTE.F90
  sub_models/sub_model_UNIFAC.c 
  sub_models/sub_model_UNIFAC.F90
  sub_models/sub_model_ZSR_aerosol_water.F90
  sub_models/sub_model_ZSR_aerosol_water.c 
)
add_prefix(gitmodules/camp/src/ camp_SOURCES)

set(json_fortran_SOURCES
  json_kinds.F90
  json_parameters.F90
  json_string_utilities.F90
  json_value_module.F90
  json_file_module.F90
  json_module.F90
)
add_prefix(gitmodules/json-fortran/src/ json_fortran_SOURCES)

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
list(APPEND partmclib_SOURCES src/fake_netcdf.F90 src/fake_spec_file.F90 src/sys.F90)

set(klu_SOURCES
  KLU/Source/klu_analyze.c
  KLU/Source/klu_memory.c
  KLU/Source/klu_tsolve.c
  KLU/Source/klu_solve.c
  KLU/Source/klu.c
  KLU/Source/klu_kernel.c 
  KLU/Source/klu_defaults.c
  KLU/Source/klu_dump.c
  KLU/Source/klu_factor.c 
  KLU/Source/klu_free_numeric.c 
  KLU/Source/klu_free_symbolic.c
  KLU/Source/klu_scale.c
  KLU/Source/klu_refactor.c
  KLU/Source/klu_diagnostics.c
  KLU/Source/klu_sort.c
  KLU/Source/klu_extract.c
  KLU/Source/klu_analyze_given.c
  COLAMD/Source/colamd.c
  SuiteSparse_config/SuiteSparse_config.c
  AMD/Source/amd_aat.c
  AMD/Source/amd_1.c
  AMD/Source/amd_2.c
  AMD/Source/amd_dump.c
  AMD/Source/amd_postorder.c
  AMD/Source/amd_defaults.c
  AMD/Source/amd_post_tree.c
  AMD/Source/amd_order.c
  AMD/Source/amd_control.c
  AMD/Source/amd_info.c
  AMD/Source/amd_valid.c
  AMD/Source/amd_preprocess.c
  BTF/Source/btf_order.c
  BTF/Source/btf_maxtrans.c
  BTF/Source/btf_strongcomp.c
)
add_prefix(gitmodules/SuiteSparse/ klu_SOURCES)

set(KLU_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/gitmodules/SuiteSparse/KLU/Include
  ${CMAKE_SOURCE_DIR}/gitmodules/SuiteSparse/AMD/Include
  ${CMAKE_SOURCE_DIR}/gitmodules/SuiteSparse/SuiteSparse_config
  ${CMAKE_SOURCE_DIR}/gitmodules/SuiteSparse/COLAMD/Include
  ${CMAKE_SOURCE_DIR}/gitmodules/SuiteSparse/BTF/Include
)

### KLU ############################################################################################

add_library(klulib STATIC
  ${klu_SOURCES} 
)
target_compile_definitions(klulib PRIVATE DLONG="1")
target_include_directories(klulib PRIVATE ${KLU_INCLUDE_DIRS})

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
  target_include_directories(${target} PRIVATE
    ${SUNDIALS_SOURCE_DIR}/src/sundials
    ${SUNDIALS_SOURCE_DIR}/include
    ${KLU_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/include
  )
endmacro()

function(print_error)
endfunction()

function(scoped_sundials_setup_config)
  set(PROJECT_SOURCE_DIR ${SUNDIALS_SOURCE_DIR})
  set(SUNDIALS_PRECISION "double")
  set(SUNDIALS_CINDEX_TYPE "int64_t")
  # TODO #111: read from submodule files!
  set(PACKAGE_VERSION_MAJOR "5")
  set(PACKAGE_VERSION_MINOR "8")
  set(PACKAGE_VERSION_PATCH "0")
  set(PACKAGE_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
  include(${SUNDIALS_SOURCE_DIR}/cmake/SundialsSetupConfig.cmake)
endfunction()
scoped_sundials_setup_config()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SUNDIALS_SOURCE_DIR}/cmake)
include(${SUNDIALS_SOURCE_DIR}/cmake/SundialsSetupCompilers.cmake)

foreach(item cvode;sunmatrix;sunlinsol;sunnonlinsol;nvector;sundials;sunlinsol/klu)
  add_subdirectory(${SUNDIALS_SOURCE_DIR}/src/${item})
endforeach()

set(SUNDIALS_items 
  cvode
  nvecserial
  sunmatrixband
  sunmatrixdense
  sunmatrixsparse
  sunlinsolband 
  sunlinsolklu
  sunnonlinsolnewton 
  sunlinsolspgmr 
  generic
)
add_prefix(sundials_ SUNDIALS_items)

### CAMP ###########################################################################################

add_library(camplib STATIC ${camp_SOURCES} ${json_fortran_SOURCES})
target_compile_definitions(camplib PRIVATE CAMP_USE_JSON="1")
target_compile_definitions(camplib PRIVATE CAMP_USE_SUNDIALS="1")
target_include_directories(camplib PRIVATE 
  ${KLU_INCLUDE_DIRS}
  ${SUNDIALS_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/include
)

### partmclib ######################################################################################

add_library(partmclib STATIC ${partmclib_SOURCES})
target_compile_definitions(partmclib PRIVATE PMC_USE_SUNDIALS="1")
target_compile_definitions(partmclib PRIVATE PMC_USE_CAMP="1")
add_dependencies(partmclib ${SUNDIALS_items})
target_include_directories(partmclib PRIVATE 
  ${SUNDIALS_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/include
)
target_link_libraries(partmclib PRIVATE camplib)
target_link_libraries(partmclib PRIVATE ${SUNDIALS_items})
target_link_libraries(partmclib PRIVATE klulib)

### PYBIND11 & PyPartMC ############################################################################

add_subdirectory(gitmodules/pybind11)
pybind11_add_module(_PyPartMC ${PyPartMC_sources})
add_dependencies(_PyPartMC partmclib)
target_include_directories(_PyPartMC PRIVATE 
  ${CMAKE_SOURCE_DIR}/gitmodules/json/include 
  ${CMAKE_SOURCE_DIR}/gitmodules/pybind11_json/include
  ${CMAKE_SOURCE_DIR}/gitmodules/span/include
  ${CMAKE_SOURCE_DIR}/gitmodules/string_view-standalone/include
)
target_compile_definitions(_PyPartMC PRIVATE VERSION_INFO=${VERSION_INFO})
target_link_libraries(_PyPartMC PRIVATE partmclib)
if (APPLE)
  target_link_options(_PyPartMC PRIVATE -Wl,-no_compact_unwind -Wl,-keep_dwarf_unwind)
  if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
    target_link_libraries(_PyPartMC PRIVATE -static gfortran -dynamic)
  endif()
endif()
if (WIN32)
  target_link_libraries(_PyPartMC PRIVATE -static gcc stdc++ winpthread quadmath -dynamic)
endif()

### pedantics ######################################################################################

foreach(target _PyPartMC)
  target_compile_options(${target} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-unused-parameter>
  )
endforeach()

