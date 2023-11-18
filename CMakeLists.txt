####################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
####################################################################################################

cmake_minimum_required(VERSION 3.8)  # CXX17

file(GLOB mods RELATIVE ${CMAKE_SOURCE_DIR} gitmodules/*)
foreach(mod ${mods})
  file(GLOB tmp RELATIVE ${CMAKE_SOURCE_DIR} ${mod}/*)
  if (tmp STREQUAL "")
    message(FATAL_ERROR "git submodule ${mod} not initialised.\n Please run `git submodule update --init`")
  endif()
endforeach()

project(_PyPartMC LANGUAGES C CXX Fortran)

find_package(PythonInterp REQUIRED)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
  add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-fimplicit-none>)
  add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-ffree-line-length-none>)
  # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58175
  add_compile_options($<$<COMPILE_LANGUAGE:Fortran>:-Wno-surprising>)

  add_compile_options($<$<AND:$<COMPILE_LANGUAGE:Fortran>,$<CONFIG:DEBUG>>:-fcheck=bounds>)
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
  pypartmc.cpp gimmicks.cpp fake_spec_file.cpp sys.cpp
  run_part.F90 run_part_opt.F90 util.F90 aero_data.F90 aero_state.F90 env_state.F90 gas_data.F90 
  gas_state.F90 scenario.F90 condense.F90 aero_particle.F90 bin_grid.F90
  camp_core.F90 photolysis.F90 aero_mode.F90 aero_dist.F90 bin_grid.cpp condense.cpp run_part.cpp
  scenario.cpp util.cpp output.cpp output.F90 rand.cpp rand.F90
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

set(netcdf_c_dispatch_SOURCES dvar.c ddim.c dvarput.c dvarget.c ddispatch.c dcompound.c denum.c daux.c
  dvlen.c nc.c dfile.c dnotnc4.c dstring.c nclist.c nchashmap.c dinstance_intern.c dtype.c dgroup.c
  nclistmgr.c dattget.c dattinq.c dvarinq.c dfilter.c derror.c doffsets.c datt.c dattput.c dcopy.c drc.c
  dpathmgr.c dmissing.c nclog.c dutf8.c utf8proc.c ncbytes.c dopaque.c ncuri.c dcrc64.c dinstance.c
  ds3util.c dutil.c dauth.c dinfermodel.c
)
add_prefix(gitmodules/netcdf-c/libdispatch/ netcdf_c_dispatch_SOURCES)

set(netcdf_c_lib_SOURCES nc_initialize.c)
add_prefix(gitmodules/netcdf-c/liblib/ netcdf_c_lib_SOURCES)

set(netcdf_c_src_SOURCES nc3dispatch.c nc3internal.c dim.c var.c ncio.c v1hpg.c memio.c posixio.c)
add_prefix(gitmodules/netcdf-c/libsrc/ netcdf_c_src_SOURCES)

set(netcdf_c_src4_SOURCES nc4internal.c ncindex.c nc4cache.c nc4dispatch.c nc4type.c nc4grp.c nc4var.c)
add_prefix(gitmodules/netcdf-c/libsrc4/ netcdf_c_src4_SOURCES)

set(netcdf_f_SOURCES typeSizes.F90 module_netcdf_nf_data.F90 module_netcdf_nc_data.F90
  module_netcdf_nf_interfaces.F90 module_netcdf_nc_interfaces.F90 module_netcdf4_nf_interfaces.F90
  module_netcdf4_nc_interfaces.F90 nf_nc4.F90 netcdf4.F90 nf_varaio.F90 nf_varsio.F90 nf_varmio.F90
  nf_var1io.F90 nf_attio.F90 nf_control.F90 nf_genvar.F90 nf_dim.F90 nf_misc.F90 nf_nc_noparallel.F90
  nf_lib.c nf_genatt.F90 nf_geninq.F90
)
add_prefix(gitmodules/netcdf-fortran/fortran/ netcdf_f_SOURCES)

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
list(APPEND partmclib_SOURCES src/fake_spec_file.F90 src/sys.F90)

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

  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/sundials)
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} "-c" "import sys; print(''.join([line for line in sys.stdin if line.startswith('set(PACKAGE_VERSION_')]))"
    INPUT_FILE ${CMAKE_SOURCE_DIR}/gitmodules/sundials/CMakeLists.txt
    OUTPUT_FILE ${CMAKE_BINARY_DIR}/sundials/CMakeLists.txt
  )
  include(${CMAKE_BINARY_DIR}/sundials/CMakeLists.txt)

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

### netCDF #########################################################################################

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/netcdf)
string(CONCAT cmd
  "import sys; print(''.join([line for line in sys.stdin"
  " if ('VERSION' in line and line.strip().startswith('SET'))"
  " or 'CHUNK_' in line"
  " or line.strip().startswith('CHECK_INCLUDE_FILE')"
  " or line.strip().startswith('CHECK_TYPE_SIZE')"
  " or line.strip().startswith('CHECK_FUNCTION_EXISTS')"
  " or 'IF(SIZEOF_' in line"
  " or '_T TRUE)' in line"
  "]))"
)
include(CheckFunctionExists)
execute_process(
  COMMAND ${PYTHON_EXECUTABLE} "-c" "${cmd}"
  INPUT_FILE ${CMAKE_SOURCE_DIR}/gitmodules/netcdf-c/CMakeLists.txt
  OUTPUT_FILE ${CMAKE_BINARY_DIR}/netcdf/CMakeLists.txt
)
include(${CMAKE_BINARY_DIR}/netcdf/CMakeLists.txt)

FIND_PROGRAM(NC_M4 NAMES m4 m4.exe REQUIRED)
SET(m4_SOURCES attr ncx putget)
LIST(APPEND netcdf_c_SOURCES ${netcdf_c_lib_SOURCES})
LIST(APPEND netcdf_c_SOURCES ${netcdf_c_dispatch_SOURCES})
LIST(APPEND netcdf_c_SOURCES ${netcdf_c_src_SOURCES})
LIST(APPEND netcdf_c_SOURCES ${netcdf_c_src4_SOURCES})
foreach (f ${m4_SOURCES})
  set(tmp ${CMAKE_BINARY_DIR}/netcdf/${f}.c)
  add_custom_command(
    OUTPUT ${tmp}
    COMMAND ${NC_M4}
    ARGS ${CMAKE_SOURCE_DIR}/gitmodules/netcdf-c/libsrc/${f}.m4 > ${tmp}
    VERBATIM
  )
  LIST(APPEND netcdf_c_SOURCES ${CMAKE_BINARY_DIR}/netcdf/${f}.c)
endforeach(f)
set(NC_DISPATCH_VERSION 5)
set(NC_HAS_LOGGING 0)
configure_file(
  ${CMAKE_SOURCE_DIR}/gitmodules/netcdf-c/config.h.cmake.in
  ${CMAKE_BINARY_DIR}/netcdf/config.h
)
configure_file(
  ${CMAKE_SOURCE_DIR}/gitmodules/netcdf-c/include/netcdf_dispatch.h.in
  ${CMAKE_BINARY_DIR}/netcdf/netcdf_dispatch.h
  @ONLY NEWLINE_STYLE LF
)
configure_file(
  ${CMAKE_SOURCE_DIR}/gitmodules/netcdf-c/include/netcdf_meta.h.in
  ${CMAKE_BINARY_DIR}/netcdf/netcdf_meta.h
  @ONLY
)
add_library(netcdf_clib STATIC ${netcdf_c_SOURCES})
target_include_directories(netcdf_clib PRIVATE 
  ${CMAKE_BINARY_DIR}/netcdf
  ${CMAKE_SOURCE_DIR}/gitmodules/netcdf-c/include
  ${CMAKE_SOURCE_DIR}/gitmodules/netcdf-c/libsrc
)
target_compile_definitions(netcdf_clib PRIVATE
  HAVE_CONFIG_H
  USE_NETCDF4
)
target_compile_options(netcdf_clib PRIVATE
    $<$<C_COMPILER_ID:GNU>:-Wno-unused-result -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast>
    $<$<C_COMPILER_ID:AppleClang>:-Wno-pointer-to-int-cast -Wno-int-to-void-pointer-cast>
)

### netCDF-FORTRAN #################################################################################

include(CheckFortranCompilerFlag)
add_library(netcdf_flib STATIC ${netcdf_f_SOURCES})
target_link_libraries(netcdf_flib PRIVATE netcdf_clib)
target_compile_definitions(netcdf_flib PRIVATE
  HAVE_TS29113_SUPPORT
  USE_NETCDF4
)
target_include_directories(netcdf_flib PRIVATE 
  ${CMAKE_SOURCE_DIR}/gitmodules/netcdf-fortran/fortran
  ${CMAKE_SOURCE_DIR}/gitmodules/netcdf-c/include
)

include(${CMAKE_SOURCE_DIR}/gitmodules/netcdf-fortran/CMakeExtras/MatchNetCDFFortranTypes.cmake)

foreach(ftype DOUBLEPRECISION;INT1;INT2;INT8;INT;REAL)
  foreach(ctype DOUBLE;FLOAT;INT;LONG;SHORT;SIGNED_CHAR;LONG_LONG)
    set(def "NF_${ftype}_IS_C_${ctype}")
    if(${def})
      target_compile_definitions(netcdf_flib PRIVATE ${def})
    endif()
  endforeach()
endforeach()

check_fortran_compiler_flag("-fallow-argument-mismatch" COMPILER_HAS_ALLOW_ARGUMENT_MISMATCH)
if(COMPILER_HAS_ALLOW_ARGUMENT_MISMATCH)
  target_compile_options(netcdf_flib PRIVATE $<$<COMPILE_LANGUAGE:Fortran>:-fallow-argument-mismatch -w>)
endif()

check_fortran_compiler_flag("-mismatch_all" COMPILER_HAS_MISMATCH_ALL)
if(COMPILER_HAS_MISMATCH_ALL)
  target_compile_options(netcdf_flib PRIVATE $<$<COMPILE_LANGUAGE:Fortran>:-mismatch_all>)
endif(COMPILER_HAS_MISMATCH_ALL)

### partmclib ######################################################################################

add_library(partmclib STATIC ${partmclib_SOURCES})
target_compile_definitions(partmclib PRIVATE PMC_USE_SUNDIALS="1")
target_compile_definitions(partmclib PRIVATE PMC_USE_CAMP="1")
add_dependencies(partmclib ${SUNDIALS_items})

string(CONCAT 
  cmd
  "import sys;"
  "print(''.join([f'#define {line.split()[3]} {line.split()[5]}\\n' for line in sys.stdin if 'parameter ::' in line]))"
)
include(CheckFunctionExists)
execute_process(
  COMMAND ${PYTHON_EXECUTABLE} "-c" "${cmd}"
  INPUT_FILE ${CMAKE_SOURCE_DIR}/gitmodules/partmc/src/aero_data.F90
  OUTPUT_FILE ${CMAKE_BINARY_DIR}/include/aero_data_parameters.hpp
)

target_include_directories(partmclib PRIVATE 
  ${SUNDIALS_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/include
)
target_link_libraries(partmclib PRIVATE netcdf_flib)
target_link_libraries(partmclib PRIVATE camplib)
target_link_libraries(partmclib PRIVATE ${SUNDIALS_items})
target_link_libraries(partmclib PRIVATE klulib)
if(DEFINED ENV{MOSAIC_HOME})
  message(STATUS "MOSAIC enabled: $ENV{MOSAIC_HOME}")
  target_compile_definitions(partmclib PRIVATE PMC_USE_MOSAIC="1")
  find_library(mosaiclib mosaic
      DOC "MOSAIC library"
      PATHS  $ENV{MOSAIC_HOME})
  target_link_libraries(partmclib PRIVATE ${mosaiclib})
  target_include_directories(partmclib PRIVATE
      $ENV{MOSAIC_HOME}/datamodules
  )
endif()

### PYBIND11 & PyPartMC ############################################################################

add_subdirectory(gitmodules/pybind11)
pybind11_add_module(_PyPartMC ${PyPartMC_sources})
add_dependencies(_PyPartMC partmclib)
set(PYPARTMC_INCLUDE_DIRS 
  "${CMAKE_BINARY_DIR}/include;"
  "${CMAKE_SOURCE_DIR}/gitmodules/json/include;"
  "${CMAKE_SOURCE_DIR}/gitmodules/pybind11_json/include;"
  "${CMAKE_SOURCE_DIR}/gitmodules/span/include;"
  "${CMAKE_SOURCE_DIR}/gitmodules/string_view-standalone/include;"
  "${CMAKE_SOURCE_DIR}/gitmodules/optional/include;"
)
target_include_directories(_PyPartMC PRIVATE ${PYPARTMC_INCLUDE_DIRS})
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
if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_link_options(_PyPartMC PRIVATE -flto=auto)
endif()

### pedantics ######################################################################################

foreach(target _PyPartMC)
  target_compile_options(${target} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wno-unused-parameter>
  )
endforeach()

include(CheckCXXSourceCompiles)
file(GLOB PyPartMC_headers ${CMAKE_SOURCE_DIR}/src/*.hpp)
if (NOT "${CMAKE_REQUIRED_INCLUDES}" STREQUAL "")
  message("CMAKE_REQUIRED_INCLUDES not empty! (${CMAKE_REQUIRED_INCLUDES})")
endif()
foreach(file ${PyPartMC_headers})
  set(CMAKE_REQUIRED_INCLUDES "${PYPARTMC_INCLUDE_DIRS};${pybind11_INCLUDE_DIRS}")
  set(CMAKE_REQUIRED_FLAGS "-Werror")
  string(REGEX REPLACE "[\-./:]" "_" file_var ${file})
  check_cxx_source_compiles("
      // https://github.com/nlohmann/json/issues/1408
      #if defined(_WIN32) || defined(_WIN64)
      #  define HAVE_SNPRINTF
      #endif
      #include \"${file}\"
      int main() { return 0;}
    "
    _header_self_contained_${file_var}
  )
  unset(CMAKE_REQUIRED_INCLUDES)
  unset(CMAKE_REQUIRED_FLAGS)
  if (NOT _header_self_contained_${file_var})
    message(SEND_ERROR "non-self-contained header: ${file}")
    if (${CMAKE_VERSION} VERSION_LESS "3.26.0")
      file(READ "${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log" tmp)
    else()
      file(READ "${CMAKE_BINARY_DIR}/CMakeFiles/CMakeConfigureLog.yaml" tmp)
    endif()
    message(FATAL_ERROR ${tmp})
  endif()
  unset(file_var)
endforeach()

