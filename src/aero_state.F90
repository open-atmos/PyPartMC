!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_aero_state
  use iso_c_binding
  use pmc_aero_state
  implicit none

  contains

  subroutine f_aero_state_ctor(ptr_c) bind(C)
    type(aero_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_aero_state_dtor(ptr_c) bind(C)
    type(aero_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine

  subroutine f_aero_state_init(ptr_c, n_part, aero_data_ptr_c) bind(C)
    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    real(c_double), intent(in) :: n_part

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    call aero_state_zero(ptr_f)
    call aero_state_set_weight(ptr_f, aero_data_ptr_f, AERO_STATE_WEIGHT_FLAT)
    call aero_state_set_n_part_ideal(ptr_f, n_part)
    !call aero_state_add_aero_dist_sample(aero_state, aero_data_ptr_f, &
    !       aero_dist_init, 1d0, 0d0, &
    !       .false. & ! TODO run_part_opt%allow_doubling, &
    !       .false. & ! TODO run_part_opt%allow_halving)
    !)

  end subroutine

  subroutine f_aero_state_len(ptr_c, len) bind(C)
    type(aero_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(out) :: len 

    call c_f_pointer(ptr_c, ptr_f)
    len = aero_state_n_part(ptr_f)
  end subroutine

end module
