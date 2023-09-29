!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_aero_data
  use iso_c_binding
  use pmc_aero_data
  implicit none

  contains

  subroutine f_aero_data_ctor(ptr_c) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    call fractal_set_spherical(ptr_f%fractal)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_aero_data_dtor(ptr_c) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine

  subroutine f_aero_data_from_json(ptr_c) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    type(spec_file_t) :: file
    call c_f_pointer(ptr_c, ptr_f)
    call spec_file_read_aero_data(file, ptr_f)
  end subroutine

  subroutine f_aero_data_spec_by_name(ptr_c, value, name_data, name_size) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    character(kind=c_char), dimension(*), intent(in) :: name_data
    integer(c_int), intent(out) :: value
    integer(c_int), intent(in) :: name_size
    character(len=name_size) :: name
    integer :: i
    call c_f_pointer(ptr_c, ptr_f)
    do i=1, name_size
      name(i:i) = name_data(i)
    end do
    value = aero_data_spec_by_name(ptr_f, name)
  end subroutine

  subroutine f_aero_data_len(ptr_c, len) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(out) :: len 

    call c_f_pointer(ptr_c, ptr_f)
    len = aero_data_n_spec(ptr_f)
  end subroutine

  subroutine f_aero_data_set_frac_dim(ptr_c, frac_dim) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(in) :: frac_dim

    call c_f_pointer(ptr_c, ptr_f)

    ptr_f%fractal%frac_dim = frac_dim

  end subroutine

  subroutine f_aero_data_get_frac_dim(ptr_c, frac_dim) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(out) :: frac_dim

    call c_f_pointer(ptr_c, ptr_f)

    frac_dim = ptr_f%fractal%frac_dim

  end subroutine

  subroutine f_aero_data_set_vol_fill_factor(ptr_c, vol_fill_factor) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(in) :: vol_fill_factor

    call c_f_pointer(ptr_c, ptr_f)

    ptr_f%fractal%vol_fill_factor = vol_fill_factor

  end subroutine

  subroutine f_aero_data_get_vol_fill_factor(ptr_c, vol_fill_factor) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(out) :: vol_fill_factor

    call c_f_pointer(ptr_c, ptr_f)

    vol_fill_factor = ptr_f%fractal%vol_fill_factor

  end subroutine

  subroutine f_aero_data_set_prime_radius(ptr_c, prime_radius) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(in) :: prime_radius

    call c_f_pointer(ptr_c, ptr_f)

    ptr_f%fractal%prime_radius = prime_radius

  end subroutine

  subroutine f_aero_data_get_prime_radius(ptr_c, prime_radius) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(out) :: prime_radius

    call c_f_pointer(ptr_c, ptr_f)

    prime_radius = ptr_f%fractal%prime_radius

  end subroutine

  subroutine f_aero_data_rad2vol(ptr_c, radius, vol) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(in) :: radius
    real(c_double), intent(out) :: vol

    call c_f_pointer(ptr_c, ptr_f)

    vol = aero_data_rad2vol(ptr_f, radius)

  end subroutine

  subroutine f_aero_data_vol2rad(ptr_c, vol, radius) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(in) :: vol
    real(c_double), intent(out) :: radius

    call c_f_pointer(ptr_c, ptr_f)

    radius = aero_data_vol2rad(ptr_f, vol)

  end subroutine

  subroutine f_aero_data_diam2vol(ptr_c, diam, vol) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(in) :: diam
    real(c_double), intent(out) :: vol

    call c_f_pointer(ptr_c, ptr_f)

    vol = aero_data_diam2vol(ptr_f, diam)

  end subroutine

  subroutine f_aero_data_vol2diam(ptr_c, vol, diam) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(in) :: vol
    real(c_double), intent(out) :: diam

    call c_f_pointer(ptr_c, ptr_f)

    diam = aero_data_vol2diam(ptr_f, vol)

  end subroutine

  subroutine f_aero_data_get_species_density(ptr_c, idx, val) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(in) :: idx
    real(c_double), intent(out) :: val

    call c_f_pointer(ptr_c, ptr_f)
    val = ptr_f%density(idx+1)

  end subroutine

  subroutine f_aero_data_n_source(ptr_c, n_source) bind(C)
    type(aero_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int) :: n_source

    call c_f_pointer(ptr_c, ptr_f)

    n_source = aero_data_n_source(ptr_f)

  end subroutine

end module
