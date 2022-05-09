module PyPartMC_run_part

  use iso_c_binding
  use pmc_run_part

  implicit none

  contains

  subroutine f_run_part(aero_data_ptr_c, gas_data_ptr_c, run_part_opt_ptr_c) bind(C)

    type(scenario_t) :: scenario
    type(env_state_t) :: env_state

    type(c_ptr), intent(in) :: aero_data_ptr_c
    type(aero_data_t), pointer :: aero_data_ptr_f => null()

    type(aero_state_t) :: aero_state

    type(c_ptr), intent(in) :: gas_data_ptr_c
    type(gas_data_t), pointer :: gas_data_ptr_f => null()

    type(gas_state_t) :: gas_state

    type(c_ptr), intent(in) :: run_part_opt_ptr_c
    type(run_part_opt_t), pointer :: run_part_opt_ptr_f => null()

    call c_f_pointer(gas_data_ptr_c, gas_data_ptr_f)
    call c_f_pointer(run_part_opt_ptr_c, run_part_opt_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call run_part(scenario, env_state, aero_data_ptr_f, aero_state, gas_data_ptr_f, gas_state, run_part_opt_ptr_f)

  end subroutine

end module
