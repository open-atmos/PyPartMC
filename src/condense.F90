!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_condense

  use iso_c_binding
  use pmc_condense

  implicit none

  contains

  subroutine f_condense_equilib_particles( &
    env_state_ptr_c, &
    aero_data_ptr_c, &
    aero_state_ptr_c &
  ) bind(C)

    type(c_ptr), intent(in) :: env_state_ptr_c
    type(env_state_t), pointer :: env_state_ptr_f => null()

    type(c_ptr), intent(in) :: aero_data_ptr_c
    type(aero_data_t), pointer :: aero_data_ptr_f => null()

    type(c_ptr), intent(in) :: aero_state_ptr_c
    type(aero_state_t), pointer :: aero_state_ptr_f => null()

    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(aero_state_ptr_c, aero_state_ptr_f)
    call condense_equilib_particles( &
      env_state_ptr_f, &
      aero_data_ptr_f, &
      aero_state_ptr_f &
    )

  end subroutine

end module
