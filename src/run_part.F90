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
    run_part_opt_ptr_c, &
    camp_core_ptr_c, &
    photolysis_ptr_c &
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

    type(c_ptr), intent(in) :: camp_core_ptr_c
    type(camp_core_t), pointer :: camp_core_ptr_f => null()
    type(c_ptr), intent(in) :: photolysis_ptr_c
    type(photolysis_t), pointer :: photolysis_ptr_f => null()

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(aero_state_ptr_c, aero_state_ptr_f)
    call c_f_pointer(gas_data_ptr_c, gas_data_ptr_f)
    call c_f_pointer(gas_state_ptr_c, gas_state_ptr_f)
    call c_f_pointer(run_part_opt_ptr_c, run_part_opt_ptr_f)
    call c_f_pointer(camp_core_ptr_c, camp_core_ptr_f)
    call c_f_pointer(photolysis_ptr_c, photolysis_ptr_f)

    call run_part( &
      scenario_ptr_f, &
      env_state_ptr_f, &
      aero_data_ptr_f, &
      aero_state_ptr_f, &
      gas_data_ptr_f, &
      gas_state_ptr_f, &
      run_part_opt_ptr_f, &
      camp_core_ptr_f, &
      photolysis_ptr_f &
    )

  end subroutine

  subroutine f_run_part_timestep( &
    scenario_ptr_c, &
    env_state_ptr_c, &
    aero_data_ptr_c, &
    aero_state_ptr_c, &
    gas_data_ptr_c, &
    gas_state_ptr_c, &
    run_part_opt_ptr_c, &
    camp_core_ptr_c, &
    photolysis_ptr_c, &
    i_time, &
    t_start, &
    last_output_time, &
    last_progress_time, &
    i_output &
  ) bind(C)

    type(c_ptr), intent(in) :: scenario_ptr_c
    type(scenario_t), pointer :: scenario_ptr_f => null()

    type(c_ptr), intent(inout) :: env_state_ptr_c
    type(env_state_t), pointer :: env_state_ptr_f => null()

    type(c_ptr), intent(inout) :: aero_data_ptr_c
    type(aero_data_t), pointer :: aero_data_ptr_f => null()

    type(c_ptr), intent(inout) :: aero_state_ptr_c
    type(aero_state_t), pointer :: aero_state_ptr_f => null()

    type(c_ptr), intent(inout) :: gas_data_ptr_c
    type(gas_data_t), pointer :: gas_data_ptr_f => null()

    type(c_ptr), intent(inout) :: gas_state_ptr_c
    type(gas_state_t), pointer :: gas_state_ptr_f => null()

    type(c_ptr), intent(in) :: run_part_opt_ptr_c
    type(run_part_opt_t), pointer :: run_part_opt_ptr_f => null()

    type(c_ptr), intent(in) :: camp_core_ptr_c
    type(camp_core_t), pointer :: camp_core_ptr_f => null()
    type(c_ptr), intent(in) :: photolysis_ptr_c
    type(photolysis_t), pointer :: photolysis_ptr_f => null()
    integer(c_int), intent(in) :: i_time
    real(c_double), intent(in) :: t_start

    real(c_double), intent(inout) :: last_output_time
    real(c_double), intent(inout) :: last_progress_time
    integer(c_int), intent(inout) :: i_output

    integer(c_int) :: progress_n_samp, progress_n_coag, &
        progress_n_emit, progress_n_dil_in, progress_n_dil_out, &
        progress_n_nuc

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(aero_state_ptr_c, aero_state_ptr_f)
    call c_f_pointer(gas_data_ptr_c, gas_data_ptr_f)
    call c_f_pointer(gas_state_ptr_c, gas_state_ptr_f)
    call c_f_pointer(run_part_opt_ptr_c, run_part_opt_ptr_f)
    call c_f_pointer(camp_core_ptr_c, camp_core_ptr_f)
    call c_f_pointer(photolysis_ptr_c, photolysis_ptr_f)

    progress_n_samp = 0
    progress_n_coag = 0
    progress_n_emit = 0
    progress_n_dil_in = 0
    progress_n_dil_out = 0
    progress_n_nuc = 0

    if (env_state_ptr_f%elapsed_time < run_part_opt_ptr_f%del_t) then
       call mosaic_init(env_state_ptr_f, aero_data_ptr_f, run_part_opt_ptr_f%del_t, &
            run_part_opt_ptr_f%do_optical)
       if (run_part_opt_ptr_f%t_output > 0) then
          call output_state(run_part_opt_ptr_f%output_prefix, &
               run_part_opt_ptr_f%output_type, aero_data_ptr_f, aero_state_ptr_f, gas_data_ptr_f, &
               gas_state_ptr_f, env_state_ptr_f, 1, .0d0, run_part_opt_ptr_f%del_t, &
               run_part_opt_ptr_f%i_repeat, run_part_opt_ptr_f%record_removals, &
               run_part_opt_ptr_f%do_optical, run_part_opt_ptr_f%uuid)
       end if
    end if
    call run_part_timestep(scenario_ptr_f, env_state_ptr_f, aero_data_ptr_f, aero_state_ptr_f, &
       gas_data_ptr_f, gas_state_ptr_f, run_part_opt_ptr_f, camp_core_ptr_f, photolysis_ptr_f, &
       i_time, t_start, last_output_time, &
       last_progress_time, i_output, progress_n_samp, progress_n_coag, &
       progress_n_emit, progress_n_dil_in, progress_n_dil_out, &
       progress_n_nuc)

  end subroutine

  subroutine f_run_part_timeblock( &
    scenario_ptr_c, &
    env_state_ptr_c, &
    aero_data_ptr_c, &
    aero_state_ptr_c, &
    gas_data_ptr_c, &
    gas_state_ptr_c, &
    run_part_opt_ptr_c, &
    camp_core_ptr_c, &
    photolysis_ptr_c, &
    i_time, &
    i_next, &
    t_start, &
    last_output_time, &
    last_progress_time, &
    i_output &
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

    type(c_ptr), intent(in) :: camp_core_ptr_c
    type(camp_core_t), pointer :: camp_core_ptr_f => null()

    type(c_ptr), intent(in) :: photolysis_ptr_c
    type(photolysis_t), pointer :: photolysis_ptr_f => null()

    integer(c_int), intent(in) :: i_time
    integer(c_int), intent(in) :: i_next
    real(c_double), intent(in) :: t_start
    real(c_double), intent(inout) :: last_output_time
    real(c_double), intent(inout) :: last_progress_time
    integer(c_int), intent(inout) :: i_output

    integer(c_int) :: progress_n_samp, progress_n_coag, &
        progress_n_emit, progress_n_dil_in, progress_n_dil_out, &
        progress_n_nuc

    call c_f_pointer(scenario_ptr_c, scenario_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(aero_state_ptr_c, aero_state_ptr_f)
    call c_f_pointer(gas_data_ptr_c, gas_data_ptr_f)
    call c_f_pointer(gas_state_ptr_c, gas_state_ptr_f)
    call c_f_pointer(run_part_opt_ptr_c, run_part_opt_ptr_f)
    call c_f_pointer(camp_core_ptr_c, camp_core_ptr_f)
    call c_f_pointer(photolysis_ptr_c, photolysis_ptr_f)

    progress_n_samp = 0
    progress_n_coag = 0
    progress_n_emit = 0
    progress_n_dil_in = 0
    progress_n_dil_out = 0
    progress_n_nuc = 0

    if (env_state_ptr_f%elapsed_time < run_part_opt_ptr_f%del_t) then
       call mosaic_init(env_state_ptr_f, aero_data_ptr_f, run_part_opt_ptr_f%del_t, &
            run_part_opt_ptr_f%do_optical)
       if (run_part_opt_ptr_f%t_output > 0) then
          call output_state(run_part_opt_ptr_f%output_prefix, &
               run_part_opt_ptr_f%output_type, aero_data_ptr_f, aero_state_ptr_f, gas_data_ptr_f, &
               gas_state_ptr_f, env_state_ptr_f, 1, .0d0, run_part_opt_ptr_f%del_t, &
               run_part_opt_ptr_f%i_repeat, run_part_opt_ptr_f%record_removals, &
               run_part_opt_ptr_f%do_optical, run_part_opt_ptr_f%uuid)
       end if
    end if

    call run_part_timeblock(scenario_ptr_f, env_state_ptr_f, aero_data_ptr_f, aero_state_ptr_f, &
       gas_data_ptr_f, gas_state_ptr_f, run_part_opt_ptr_f, camp_core_ptr_f, photolysis_ptr_f, &
       i_time, i_next, t_start, last_output_time, &
       last_progress_time, i_output, progress_n_samp, progress_n_coag, &
       progress_n_emit, progress_n_dil_in, progress_n_dil_out, &
       progress_n_nuc)

  end subroutine

end module
