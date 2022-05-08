cmake_minimum_required(VERSION 3.4...3.18)
project(PyPartMC)

enable_language(CXX)
enable_language(Fortran)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 14)

include(partmc/netcdf.cmake)

add_library(partmclib
#  src/fake_mpi.F90
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
#target_compile_definitions(partmclib PRIVATE PMC_USE_MPI="1")

add_subdirectory(pybind11)
pybind11_add_module(PyPartMC 
  src/pypartmc.cpp
  src/run_part.F90
  src/util.F90
  src/gas_state.F90
)
add_dependencies(PyPartMC partmclib)
target_compile_definitions(PyPartMC PRIVATE VERSION_INFO=${VERSION_INFO})
target_link_libraries(PyPartMC PRIVATE partmclib ${NETCDF_LIBS})
