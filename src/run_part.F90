module PyPartMC_run_part

  use iso_c_binding
  use pmc_run_part

  implicit none

  type, bind(C) :: run_part_opt_c
    real(kind=dp) :: t_max
    real(kind=dp) :: t_output
  end type run_part_opt_c

  contains

  subroutine f_run_part(arg_run_part_opt, gas_data_ptr_c) bind(C)

    type(scenario_t) :: scenario
    type(env_state_t) :: env_state
    type(aero_data_t) :: aero_data
    type(aero_state_t) :: aero_state

    type(c_ptr), intent(in) :: gas_data_ptr_c
    type(gas_data_t), pointer :: gas_data_ptr_f => null()

    type(gas_state_t) :: gas_state

    type(run_part_opt_c) :: arg_run_part_opt
    type(run_part_opt_t) :: run_part_opt

    call c_f_pointer(gas_data_ptr_c, gas_data_ptr_f)
    call run_part(scenario, env_state, aero_data, aero_state, gas_data_ptr_f, gas_state, run_part_opt)

  end subroutine

end module
