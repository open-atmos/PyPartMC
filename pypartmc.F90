module PyPartMC
use iso_c_binding
use pmc_run_part
implicit none

integer(C_INT), bind(C), dimension(5) :: numbers

contains

subroutine fortransub() bind(C)

   type(scenario_t) :: scenario
   type(env_state_t) :: env_state
   type(aero_data_t) :: aero_data
   type(aero_state_t) :: aero_state
   type(gas_data_t) :: gas_data
   type(gas_state_t) :: gas_state
   type(run_part_opt_t) :: run_part_opt

   print *, "Hello from Fortran!"
   numbers(1) = 1
   numbers(2) = 2
   numbers(3) = 3
   numbers(4) = 4
   numbers(5) = 5

   call run_part(scenario, env_state, aero_data, aero_state, gas_data, gas_state, run_part_opt)

end subroutine

end module
