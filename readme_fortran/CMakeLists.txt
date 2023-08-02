cmake_minimum_required(VERSION 3.17)
project(main LANGUAGES Fortran)
add_executable(main
  main.f90
  $ENV{PARTMC_HOME}/src/spec_file.F90
  $ENV{PARTMC_HOME}/src/spec_line.F90
  $ENV{PARTMC_HOME}/src/util.F90
  $ENV{PARTMC_HOME}/src/constants.F90
  $ENV{PARTMC_HOME}/src/aero_data.F90
  $ENV{PARTMC_HOME}/src/aero_mode.F90
  $ENV{PARTMC_HOME}/src/aero_dist.F90
  $ENV{PARTMC_HOME}/src/aero_state.F90
  $ENV{PARTMC_HOME}/src/mpi.F90
  $ENV{PARTMC_HOME}/src/fractal.F90
  $ENV{PARTMC_HOME}/src/rand.F90
  $ENV{PARTMC_HOME}/src/bin_grid.F90
  $ENV{PARTMC_HOME}/src/aero_weight.F90
  $ENV{PARTMC_HOME}/src/aero_weight_array.F90
  $ENV{PARTMC_HOME}/src/aero_info.F90
  $ENV{PARTMC_HOME}/src/aero_info_array.F90
  $ENV{PARTMC_HOME}/src/aero_binned.F90
  $ENV{PARTMC_HOME}/src/aero_particle.F90
  $ENV{PARTMC_HOME}/src/aero_particle_array.F90
  $ENV{PARTMC_HOME}/src/aero_sorted.F90
  $ENV{PARTMC_HOME}/src/netcdf.F90
  $ENV{PARTMC_HOME}/src/env_state.F90
  $ENV{PARTMC_HOME}/src/integer_varray.F90
  $ENV{PARTMC_HOME}/src/integer_rmap.F90
  $ENV{PARTMC_HOME}/src/integer_rmap2.F90
)
target_link_libraries(main netcdff)
