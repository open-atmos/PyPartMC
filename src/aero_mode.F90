!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_aero_mode
  use iso_c_binding
  use pmc_aero_mode
  implicit none

  contains

  subroutine f_aero_mode_ctor(ptr_c) bind(C)
    type(aero_mode_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_aero_mode_dtor(ptr_c) bind(C)
    type(aero_mode_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine

  subroutine f_aero_mode_init(ptr_c) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
  
    call c_f_pointer(ptr_c, aero_mode)

    ! Hard code some things so we can do testing
    aero_mode%name = 'test_mode'
    aero_mode%type = 1
    aero_mode%char_radius = 2e-8
    aero_mode%log10_std_dev_radius = log10(1.6)
    aero_mode%num_conc = 1e9
!    allocate(aero_mode%sample_radius(0))
!    allocate(aero_mode%sample_num_conc(0))

!    allocate(aero_mode%vol_frac(20))
!    allocate(aero_mode%vol_frac_std(20))
!    aero_mode%vol_frac = 0.0
!    aero_mode%vol_frac(1) = 1.0
!    aero_mode%vol_frac_std = 0.0
    aero_mode%source = 1
 
  end subroutine

  subroutine f_aero_mode_total_num_conc(ptr_c, val) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    real(c_double), intent(out) :: val

    call c_f_pointer(ptr_c, aero_mode)
    val = aero_mode_total_num_conc(aero_mode)

  end subroutine

end module
