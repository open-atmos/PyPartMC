!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_gas_state
  use iso_c_binding
  use pmc_gas_state
  implicit none

  contains

  subroutine f_gas_state_ctor(ptr_c) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    ptr_c = c_loc(ptr_f)
  end subroutine

  ! integer(c_int), intent(in) :: n
  ! TODO  call gas_state_set_size(ptr_f, n)

  subroutine f_gas_state_dtor(ptr_c) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    call gas_state_set_size(ptr_f, 0)
    deallocate(ptr_f)
  end subroutine

  subroutine f_gas_state_len(ptr_c, len) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(out) :: len

    call c_f_pointer(ptr_c, ptr_f)
    len = size(ptr_f%mix_rat)
  end subroutine

  subroutine f_gas_state_set_item(ptr_c, idx, val) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(in) :: idx
    real(c_double), intent(in) :: val

    call c_f_pointer(ptr_c, ptr_f)
    ptr_f%mix_rat(idx+1) = val
  end subroutine

  subroutine f_gas_state_get_item(ptr_c, idx, val) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(in) :: idx
    real(c_double), intent(out) :: val

    call c_f_pointer(ptr_c, ptr_f)
    val = ptr_f%mix_rat(idx+1)
  end subroutine

  subroutine f_gas_state_from_json(ptr_c) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    
    type(gas_data_t), pointer :: gas_data => null()
    integer :: ncid

    call c_f_pointer(ptr_c, ptr_f)
    call gas_state_input_netcdf(ptr_f, ncid, gas_data)
  end subroutine

  subroutine f_gas_state_to_json(ptr_c) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    type(gas_data_t), pointer :: gas_data => null()
    integer :: ncid

    call c_f_pointer(ptr_c, ptr_f)
    allocate(gas_data)  ! TODO
    call gas_state_output_netcdf(ptr_f, ncid, gas_data)
    deallocate(gas_data)  ! TODO
  end subroutine
end module
