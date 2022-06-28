!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_bin_grid
  use iso_c_binding
  use pmc_bin_grid
  implicit none

  contains

  subroutine f_bin_grid_ctor(ptr_c) bind(C)
    type(bin_grid_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_bin_grid_dtor(ptr_c) bind(C)
    type(bin_grid_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine

  subroutine f_bin_grid_init(ptr_c, n_bin, bin_grid_type, min, max) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(bin_grid_t), pointer :: bin_grid => null()
    integer(c_int), intent(in) :: n_bin
    integer(c_int), intent(in) :: bin_grid_type
    real(c_double), intent(in) :: min
    real(c_double), intent(in) :: max
  
    call c_f_pointer(ptr_c, bin_grid) 
    call bin_grid_make(bin_grid, bin_grid_type, n_bin, min, max)
 
  end subroutine

  subroutine f_bin_grid_size(ptr_c, val) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(bin_grid_t), pointer :: bin_grid => null()
    integer(c_int), intent(out) :: val

    call c_f_pointer(ptr_c, bin_grid)
    val = bin_grid_size(bin_grid)

  end subroutine

  subroutine f_bin_grid_edges(ptr_c, arr_data, arr_size) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(bin_grid_t), pointer :: bin_grid => null()
    integer(c_int), intent(in) :: arr_size
    real(c_double), dimension(arr_size), intent(out) :: arr_data

    call c_f_pointer(ptr_c, bin_grid)
    arr_data = bin_grid%edges
  end subroutine

  subroutine f_bin_grid_centers(ptr_c, arr_data, arr_size) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(bin_grid_t), pointer :: bin_grid => null()
    integer(c_int), intent(in) :: arr_size
    real(c_double), dimension(arr_size), intent(out) :: arr_data

    call c_f_pointer(ptr_c, bin_grid)
    arr_data = bin_grid%centers
  end subroutine

  subroutine f_bin_grid_histogram_1d(x_bin_grid_ptr_c, x_data, weight_data, &
       arr_size, output_data, bin_grid_size) bind(C)

    type(c_ptr), intent(in) :: x_bin_grid_ptr_c
    type(bin_grid_t), pointer :: bin_grid => null()
    integer(c_int), intent(in) :: arr_size
    integer(c_int), intent(in) :: bin_grid_size
    real(c_double), dimension(bin_grid_size) :: output_data
    real(c_double), dimension(arr_size) :: x_data
    real(c_double), dimension(arr_size) :: weight_data

    call c_f_pointer(x_bin_grid_ptr_c, bin_grid)
    output_data = bin_grid_histogram_1d(bin_grid, x_data, weight_data)

  end subroutine

  subroutine f_bin_grid_histogram_2d(x_bin_grid_ptr_c, x_data, &
       y_bin_grid_ptr_c, y_data, weight_data, &
       arr_size, output_data, x_bin_grid_size, y_bin_grid_size) bind(C)

    type(c_ptr), intent(in) :: x_bin_grid_ptr_c
    type(c_ptr), intent(in) :: y_bin_grid_ptr_c
    type(bin_grid_t), pointer :: x_bin_grid => null()
    type(bin_grid_t), pointer :: y_bin_grid => null()
    integer(c_int), intent(in) :: arr_size
    integer(c_int), intent(in) :: x_bin_grid_size
    integer(c_int), intent(in) :: y_bin_grid_size
    real(c_double), dimension(x_bin_grid_size*y_bin_grid_size), intent(out) :: output_data
    real(c_double), dimension(arr_size), intent(in) :: x_data, y_data
    real(c_double), dimension(arr_size), intent(in) :: weight_data
    real(c_double), allocatable :: output_data_local(:,:)
    integer :: i, j

    call c_f_pointer(x_bin_grid_ptr_c, x_bin_grid)
    call c_f_pointer(y_bin_grid_ptr_c, y_bin_grid)
    allocate(output_data_local(x_bin_grid_size,y_bin_grid_size))
    output_data_local = bin_grid_histogram_2d(x_bin_grid, x_data, y_bin_grid, & 
         y_data, weight_data)

    do i = 1,x_bin_grid_size
    do j = 1,y_bin_grid_size
       output_data((i-1)*y_bin_grid_size + j) = output_data_local(i,j)
    end do
    end do

  end subroutine

end module
