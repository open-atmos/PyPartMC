!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_aero_binned
  use iso_c_binding
  use pmc_aero_binned
  implicit none

  contains

  subroutine f_aero_binned_ctor(ptr_c) bind(C)
    type(aero_binned_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_aero_binned_dtor(ptr_c) bind(C)
    type(aero_binned_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine

  subroutine f_aero_binned_num_conc(ptr_c, num_conc, n_bins) bind(C)
    type(aero_binned_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int) :: n_bins
    real(c_double) :: num_conc(n_bins)

    call c_f_pointer(ptr_c, ptr_f)

    num_conc = ptr_f%num_conc

  end subroutine

  subroutine f_aero_binned_len(ptr_c, len) bind(C)
    type(aero_binned_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int) :: len 

    call c_f_pointer(ptr_c, ptr_f)

    len = size(ptr_f%num_conc)

  end subroutine

end module
