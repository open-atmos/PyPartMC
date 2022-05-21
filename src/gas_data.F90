!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Author: Sylwester Arabas                                                                         #
!###################################################################################################

module PyPartMC_gas_data
  use iso_c_binding
  use pmc_gas_data
  implicit none

  contains

  subroutine f_gas_data_ctor(ptr_c) bind(C)
    type(gas_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    !TODO call gas_data_initialize(ptr_f, n)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_gas_data_dtor(ptr_c) bind(C)
    type(gas_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    !TODO ? call gas_state_set_size(ptr_f, 0)
    deallocate(ptr_f)
  end subroutine

  subroutine f_gas_data_from_json(ptr_c) bind(C)
    type(gas_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    type(spec_file_t) :: nofile

    call c_f_pointer(ptr_c, ptr_f)
    call spec_file_read_gas_data(nofile, ptr_f)
  end subroutine

end module
