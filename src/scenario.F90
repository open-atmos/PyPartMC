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

  subroutine f_scenario_init_env_state(scenario_ptr_c, env_state_ptr_c, &
      time) bind(C)


    type(c_ptr), intent(in) :: scenario_ptr_c, env_state_ptr_c
    real(c_double), intent(in) :: time
    type(scenario_t), pointer :: scenario_ptr_f => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    call scenario_init_env_state(scenario_ptr_f, env_state_ptr_f, time)

  end subroutine

  subroutine f_scenario_aero_dist_emission(scenario_ptr_c, aero_dist_ptr_c, index) bind(C)

    type(c_ptr), intent(in) :: scenario_ptr_c, aero_dist_ptr_c
    integer(c_int), intent(in) :: index
    type(scenario_t), pointer :: scenario_ptr_f => null()
    type(aero_dist_t), pointer :: aero_dist_ptr_f => null()

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)
    call c_f_pointer(aero_dist_ptr_c, aero_dist_ptr_f)

    aero_dist_ptr_f = scenario_ptr_f%aero_emission(index + 1)

  end subroutine

  subroutine f_scenario_aero_emission_n_times(scenario_ptr_c, n_times) bind(C)

    type(c_ptr), intent(in) :: scenario_ptr_c
    integer(c_int), intent(out) :: n_times
    type(scenario_t), pointer :: scenario_ptr_f => null()

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)

    n_times = size(scenario_ptr_f%aero_emission_time)

  end subroutine

  subroutine f_scenario_emission_rates(scenario_ptr_c, emission_rates, &
    n_times) bind(C)

    type(c_ptr), intent(in) :: scenario_ptr_c
    type(scenario_t), pointer :: scenario_ptr_f => null()
    integer(c_int) :: n_times
    real(c_double) :: emission_rates(n_times)

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)

    emission_rates = scenario_ptr_f%aero_emission_rate_scale

  end subroutine

  subroutine f_scenario_emission_time(scenario_ptr_c, emission_time, &
    n_times) bind(C)

    type(c_ptr), intent(in) :: scenario_ptr_c
    type(scenario_t), pointer :: scenario_ptr_f => null()
    integer(c_int) :: n_times
    real(c_double) :: emission_time(n_times)

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)

    emission_time = scenario_ptr_f%aero_emission_time

  end subroutine

  subroutine f_scenario_aero_dist_background(scenario_ptr_c, aero_dist_ptr_c, index) bind(C)

    type(c_ptr), intent(in) :: scenario_ptr_c, aero_dist_ptr_c
    integer(c_int), intent(in) :: index
    type(scenario_t), pointer :: scenario_ptr_f => null()
    type(aero_dist_t), pointer :: aero_dist_ptr_f => null()

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)
    call c_f_pointer(aero_dist_ptr_c, aero_dist_ptr_f)

    aero_dist_ptr_f = scenario_ptr_f%aero_background(index + 1)

  end subroutine

  subroutine f_scenario_aero_background_n_times(scenario_ptr_c, n_times) bind(C)

    type(c_ptr), intent(in) :: scenario_ptr_c
    integer(c_int), intent(out) :: n_times
    type(scenario_t), pointer :: scenario_ptr_f => null()

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)

    n_times = size(scenario_ptr_f%aero_dilution_time)

  end subroutine

  subroutine f_scenario_aero_background_rate_scale(scenario_ptr_c, &
    aero_background_rate_scale, n_times) bind(C)

    type(c_ptr), intent(in) :: scenario_ptr_c
    type(scenario_t), pointer :: scenario_ptr_f => null()
    integer(c_int) :: n_times
    real(c_double) :: aero_background_rate_scale(n_times)

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)

    aero_background_rate_scale = scenario_ptr_f%aero_dilution_rate

  end subroutine

  subroutine f_scenario_aero_background_time(scenario_ptr_c, aero_background_time, &
    n_times) bind(C)

    type(c_ptr), intent(in) :: scenario_ptr_c
    type(scenario_t), pointer :: scenario_ptr_f => null()
    integer(c_int) :: n_times
    real(c_double) :: aero_background_time(n_times)

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)

    aero_background_time = scenario_ptr_f%aero_dilution_time

  end subroutine

end module
