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
  $ENV{PARTMC_HOME}/src/netcdf.F90
)
target_include_directories(
  main
  PRIVATE /home/slayoo/devel/PyPartMC/build/temp.linux-x86_64-3.10/_PyPartMC
)
