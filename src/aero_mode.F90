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

  subroutine f_aero_mode_set_num_conc(ptr_c, val) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    real(c_double), intent(in) :: val

    call c_f_pointer(ptr_c, aero_mode)
    aero_mode%num_conc = val

  end subroutine

  subroutine f_aero_mode_get_num_conc(ptr_c, val) bind(C)
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

  subroutine f_aero_mode_from_json(ptr_c, aero_data_ptr_c) bind(C)
    type(aero_mode_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(inout) :: ptr_c, aero_data_ptr_c
    type(spec_file_t) :: file
    logical :: eof

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    call spec_file_read_aero_mode(file, aero_data_ptr_f, ptr_f, eof)

  end subroutine

  subroutine f_aero_mode_set_sampled(ptr_c, diam_data, num_conc_data, &
       arr_size) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    integer(c_int) :: arr_size
    real(c_double) :: diam_data(arr_size), num_conc_data(arr_size-1)

    call c_f_pointer(ptr_c, aero_mode)

    aero_mode%type = AERO_MODE_TYPE_SAMPLED
    aero_mode%sample_radius = diam_data / 2
    aero_mode%sample_num_conc = num_conc_data

  end subroutine

  subroutine f_aero_mode_set_type(ptr_c, type) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    integer(c_int), intent(in) :: type  

    call c_f_pointer(ptr_c, aero_mode)    
    aero_mode%type = type

  end subroutine

  subroutine f_aero_mode_get_type(ptr_c, type) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    integer(c_int), intent(out) :: type

    call c_f_pointer(ptr_c, aero_mode)
    type = aero_mode%type

  end subroutine
  
  subroutine f_aero_mode_set_name(ptr_c, name_data, name_size) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    character(kind=c_char), dimension(*), intent(in) :: name_data
    integer(c_int), intent(in) :: name_size
    integer :: i

    call c_f_pointer(ptr_c, aero_mode)    
    do i=1, name_size
      aero_mode%name(i:i) = name_data(i)
    end do
    do i=name_size+1, len(aero_mode%name)
      aero_mode%name(i:i) = " " 
    end do
  end subroutine

  subroutine f_aero_mode_get_name(ptr_c, name_data, name_size) bind(C)
    type(c_ptr), intent(inout) :: ptr_c
    type(aero_mode_t), pointer :: aero_mode => null()
    type(c_ptr), intent(out) :: name_data
    integer(c_int), intent(out) :: name_size

    call c_f_pointer(ptr_c, aero_mode)
    name_data = c_loc(aero_mode%name)
    name_size = len_trim(aero_mode%name)
  end subroutine
end module
