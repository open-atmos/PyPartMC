!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_aero_particle
  use iso_c_binding
  use pmc_aero_particle
  implicit none

  contains

  subroutine f_aero_particle_ctor(ptr_c) bind(C)
    type(aero_particle_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_aero_particle_dtor(ptr_c) bind(C)
    type(aero_particle_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f%vol)
    deallocate(ptr_f)
  end subroutine

  subroutine f_aero_particle_init(ptr_c, aero_data_ptr_c, arr_data, arr_size) bind(C)
    type(aero_particle_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    integer(c_int), intent(in) :: arr_size
    real(c_double), dimension(arr_size), intent(in) :: arr_data
    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    allocate(ptr_f%vol(arr_size))
    ptr_f%vol = arr_data
  end subroutine

  subroutine f_aero_particle_volumes(ptr_c, arr_data, arr_size) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(aero_particle_t), pointer :: aero_particle => null()
    integer(c_int), intent(in) :: arr_size
    real(c_double), dimension(arr_size), intent(out) :: arr_data

    call c_f_pointer(ptr_c, aero_particle)
    arr_data = aero_particle%vol
  end subroutine
end module
