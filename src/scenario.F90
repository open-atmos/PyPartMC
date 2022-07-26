!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_scenario
  use iso_c_binding
  use pmc_scenario
  implicit none

  contains

  subroutine f_scenario_ctor(ptr_c) bind(C)
    type(scenario_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_scenario_dtor(ptr_c) bind(C)
    type(scenario_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine

  subroutine f_scenario_from_json(gas_ptr_c, aer_ptr_c, ptr_c) bind(C)
    type(scenario_t), pointer :: ptr_f => null()
    type(gas_data_t), pointer :: gas_ptr_f => null()
    type(aero_data_t), pointer :: aer_ptr_f => null()
    type(c_ptr), intent(in) :: gas_ptr_c, aer_ptr_c, ptr_c
    type(spec_file_t) :: file 

    call c_f_pointer(gas_ptr_c, gas_ptr_f)
    call c_f_pointer(aer_ptr_c, aer_ptr_f)
    call c_f_pointer(ptr_c, ptr_f)
    call spec_file_read_scenario(file, gas_ptr_f, aer_ptr_f, ptr_f)
  end subroutine

  subroutine f_scenario_loss_rate( &
    scenario_ptr_c, vol, &
    density, aero_data_ptr_c, &
    env_state_ptr_c, &
    rate &
  ) bind (C)

    type(scenario_t), pointer :: scenario_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()
    real(c_double), intent(in) :: vol, density
    type(c_ptr), intent(in) :: scenario_ptr_c, aero_data_ptr_c, env_state_ptr_c
    real(c_double), intent(out) :: rate

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    rate = scenario_loss_rate( &
      scenario_ptr_f, &
      vol, &
      density, &
      aero_data_ptr_f, &
      env_state_ptr_f &
    )

  end subroutine

  subroutine f_scenario_loss_rate_dry_dep( &
    vol, &
    density, &
    aero_data_ptr_c, &
    env_state_ptr_c, &
    rate &
  ) bind(C)

    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()
    real(c_double), intent(in) :: vol, density
    type(c_ptr), intent(in) :: aero_data_ptr_c, env_state_ptr_c
    real(c_double), intent(out) :: rate

    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    rate = scenario_loss_rate_dry_dep( &
      vol, &
      density, &
      aero_data_ptr_f, &
      env_state_ptr_f &
    )

  end subroutine

end module
