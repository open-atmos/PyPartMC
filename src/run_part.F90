!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_run_part

  use iso_c_binding
  use pmc_run_part

  implicit none

  contains

  subroutine f_run_part( &
    scenario_ptr_c, &
    env_state_ptr_c, &
    aero_data_ptr_c, &
    aero_state_ptr_c, &
    gas_data_ptr_c, &
    gas_state_ptr_c, &
    run_part_opt_ptr_c &
  ) bind(C)

    type(c_ptr), intent(in) :: scenario_ptr_c
    type(scenario_t), pointer :: scenario_ptr_f => null()

    type(c_ptr), intent(in) :: env_state_ptr_c
    type(env_state_t), pointer :: env_state_ptr_f => null()

    type(c_ptr), intent(in) :: aero_data_ptr_c
    type(aero_data_t), pointer :: aero_data_ptr_f => null()

    type(c_ptr), intent(in) :: aero_state_ptr_c
    type(aero_state_t), pointer :: aero_state_ptr_f => null()

    type(c_ptr), intent(in) :: gas_data_ptr_c
    type(gas_data_t), pointer :: gas_data_ptr_f => null()

    type(c_ptr), intent(in) :: gas_state_ptr_c
    type(gas_state_t), pointer :: gas_state_ptr_f => null()

    type(c_ptr), intent(in) :: run_part_opt_ptr_c
    type(run_part_opt_t), pointer :: run_part_opt_ptr_f => null()

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(aero_state_ptr_c, aero_state_ptr_f)
    call c_f_pointer(gas_data_ptr_c, gas_data_ptr_f)
    call c_f_pointer(gas_state_ptr_c, gas_state_ptr_f)
    call c_f_pointer(run_part_opt_ptr_c, run_part_opt_ptr_f)
    call run_part( &
      scenario_ptr_f, &
      env_state_ptr_f, &
      aero_data_ptr_f, &
      aero_state_ptr_f, &
      gas_data_ptr_f, &
      gas_state_ptr_f, &
      run_part_opt_ptr_f &
    )

  end subroutine

end module
