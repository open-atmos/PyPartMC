!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2025 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_run_sect

  use iso_c_binding
  use pmc_run_sect

  implicit none

  contains

  subroutine f_run_sect( &
    bin_grid_ptr_c, &
    gas_data_ptr_c, &
    aero_data_ptr_c, &
    aero_dist_ptr_c, &
    scenario_ptr_c, &
    env_state_ptr_c, &
    run_sect_opt_ptr_c &
  ) bind(C)

    type(c_ptr), intent(in) :: bin_grid_ptr_c
    type(bin_grid_t), pointer :: bin_grid_ptr_f => null()

    type(c_ptr), intent(in) :: gas_data_ptr_c
    type(gas_data_t), pointer :: gas_data_ptr_f => null()

    type(c_ptr), intent(in) :: aero_data_ptr_c
    type(aero_data_t), pointer :: aero_data_ptr_f => null()

    type(c_ptr), intent(in) :: aero_dist_ptr_c
    type(aero_dist_t), pointer :: aero_dist_ptr_f => null()

    type(c_ptr), intent(in) :: scenario_ptr_c
    type(scenario_t), pointer :: scenario_ptr_f => null()

    type(c_ptr), intent(in) :: env_state_ptr_c
    type(env_state_t), pointer :: env_state_ptr_f => null()

    type(c_ptr), intent(in) :: run_sect_opt_ptr_c
    type(run_sect_opt_t), pointer :: run_sect_opt_ptr_f => null()

    call c_f_pointer(bin_grid_ptr_c, bin_grid_ptr_f)
    call c_f_pointer(gas_data_ptr_c, gas_data_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(aero_dist_ptr_c, aero_dist_ptr_f)
    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)
    call c_f_pointer(run_sect_opt_ptr_c, run_sect_opt_ptr_f)

    call run_sect( &
      bin_grid_ptr_f, &
      gas_data_ptr_f, &
      aero_data_ptr_f, &
      aero_dist_ptr_f, &
      scenario_ptr_f, &
      env_state_ptr_f, &
      run_sect_opt_ptr_f &
    )

  end subroutine

end module
