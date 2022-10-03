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

  subroutine f_aero_mode_init(ptr_c, aero_data_ptr_c) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(c_ptr), intent(in) :: aero_data_ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    type(aero_data_t), pointer :: aero_data => null()

    call c_f_pointer(ptr_c, aero_mode)
    call c_f_pointer(aero_data_ptr_c, aero_data)

    ! Hard code some things so we can do testing
    call f_aero_mode_default(aero_mode, aero_data)
 
  end subroutine

  subroutine f_aero_mode_total_num_conc(ptr_c, val) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    real(c_double), intent(out) :: val

    call c_f_pointer(ptr_c, aero_mode)
    val = aero_mode_total_num_conc(aero_mode)

  end subroutine

  subroutine f_aero_mode_num_conc(ptr_c, bin_grid_ptr_c, &
       aero_data_ptr_c, arr_data, arr_size) bind(C)

    type(c_ptr), intent(in) :: ptr_c, bin_grid_ptr_c, &
         aero_data_ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    type(bin_grid_t), pointer :: bin_grid => null()
    type(aero_data_t), pointer :: aero_data => null()
    integer(c_int), intent(in) :: arr_size
    real(c_double), dimension(arr_size), intent(out) :: arr_data

    call c_f_pointer(ptr_c, aero_mode)
    call c_f_pointer(bin_grid_ptr_c, bin_grid)
    call c_f_pointer(aero_data_ptr_c, aero_data)

    call aero_mode_num_conc(aero_mode, bin_grid, aero_data, &
       arr_data)

  end subroutine 

  subroutine f_aero_mode_default(aero_mode, aero_data)
    type(aero_mode_t), intent(inout) :: aero_mode
    type(aero_data_t), intent(inout) :: aero_data

    aero_mode%name = 'test_mode'
    aero_mode%type = 1
    aero_mode%char_radius = 2e-8
    aero_mode%log10_std_dev_radius = log10(1.6)
    aero_mode%num_conc = 1e9

    if (allocated(aero_mode%sample_radius)) deallocate(aero_mode%sample_radius)
    if (allocated(aero_mode%sample_num_conc)) &
         deallocate(aero_mode%sample_num_conc)
    allocate(aero_mode%sample_radius(0))
    allocate(aero_mode%sample_num_conc(0))

    if (allocated(aero_mode%vol_frac)) deallocate(aero_mode%vol_frac)
    if (allocated(aero_mode%vol_frac_std)) deallocate(aero_mode%vol_frac_std)
    allocate(aero_mode%vol_frac(aero_data_n_spec(aero_data)))
    allocate(aero_mode%vol_frac_std(aero_data_n_spec(aero_data)))
    aero_mode%vol_frac = 0.0
    aero_mode%vol_frac(1) = 1.0
    aero_mode%vol_frac_std = 0.0
    aero_mode%source = 1

  end subroutine

  subroutine f_aero_mode_get_n_spec(ptr_c, len) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    integer(c_int) :: len

    call c_f_pointer(ptr_c, aero_mode)

    len = size(aero_mode%vol_frac)
      
  end subroutine

  subroutine f_aero_mode_get_vol_frac(ptr_c, arr_data, arr_size) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    integer(c_int) :: arr_size
    real(c_double) :: arr_data(arr_size)

    call c_f_pointer(ptr_c, aero_mode)

    arr_data = aero_mode%vol_frac

  end subroutine

  subroutine f_aero_mode_set_vol_frac(ptr_c, arr_data, arr_size) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    integer(c_int) :: arr_size
    real(c_double) :: arr_data(arr_size)

    call c_f_pointer(ptr_c, aero_mode)
    aero_mode%vol_frac = arr_data

  end subroutine

  subroutine f_aero_mode_get_vol_frac_std(ptr_c, arr_data, arr_size) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    integer(c_int) :: arr_size
    real(c_double) :: arr_data(arr_size)

    call c_f_pointer(ptr_c, aero_mode)

    arr_data = aero_mode%vol_frac_std

  end subroutine

  subroutine f_aero_mode_set_vol_frac_std(ptr_c, arr_data, arr_size) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    integer(c_int) :: arr_size
    real(c_double) :: arr_data(arr_size)

    call c_f_pointer(ptr_c, aero_mode)
    aero_mode%vol_frac_std = arr_data

  end subroutine


  subroutine f_aero_mode_get_char_radius(ptr_c, char_radius) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    real(c_double) :: char_radius

    call c_f_pointer(ptr_c, aero_mode)

    char_radius = aero_mode%char_radius

  end subroutine

  subroutine f_aero_mode_set_char_radius(ptr_c, char_radius) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    real(c_double) :: char_radius

    call c_f_pointer(ptr_c, aero_mode)
    aero_mode%char_radius = char_radius 

  end subroutine

  subroutine f_aero_mode_get_gsd(ptr_c, gsd) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    real(c_double) :: gsd 

    call c_f_pointer(ptr_c, aero_mode)

    gsd = 10**(aero_mode%log10_std_dev_radius)

  end subroutine

  subroutine f_aero_mode_set_gsd(ptr_c, gsd) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    real(c_double) :: gsd

    call c_f_pointer(ptr_c, aero_mode)
    aero_mode%log10_std_dev_radius = log10(gsd)

  end subroutine

end module
