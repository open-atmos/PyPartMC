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
    type(c_ptr) :: ptr_c, aero_data_ptr_c
    real(c_double), intent(in) :: n_part

    ! local testing
    type(aero_dist_t) :: aero_dist_init
    real(kind=dp), parameter, dimension(2) :: num_conc = &
         [3.2d9, 2.9d9]
    real(kind=dp), parameter, dimension(2) :: diams = [2d-8, 1.16d-7]
    real(kind=dp), parameter, dimension(2) :: std = [1.45,1.65]
    character(len=10), parameter, dimension(2) :: mode_names = ["init_small","init_large"]
    integer :: n_spec, n_modes, i_mode, i_water
    integer :: n_part_added, source

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    call aero_state_zero(ptr_f)
    call aero_state_set_weight(ptr_f, aero_data_ptr_f, AERO_STATE_WEIGHT_FLAT)
    call aero_state_set_n_part_ideal(ptr_f, n_part)

    call fractal_set_spherical(aero_data_ptr_f%fractal)

    n_modes = 2 
    n_spec = aero_data_n_spec(aero_data_ptr_f)
    allocate(aero_dist_init%mode(n_modes))
    do i_mode = 1,n_modes
       aero_dist_init%mode(i_mode)%name = mode_names(i_mode)
       aero_dist_init%mode(i_mode)%type = AERO_MODE_TYPE_LOG_NORMAL
       aero_dist_init%mode(i_mode)%char_radius = diams(i_mode) / 2
       aero_dist_init%mode(i_mode)%log10_std_dev_radius = & 
          log10(std(i_mode))
       aero_dist_init%mode(i_mode)%num_conc = num_conc(i_mode)
       allocate(aero_dist_init%mode(i_mode)%vol_frac(n_spec))
       aero_dist_init%mode(i_mode)%vol_frac = 0.0
       i_water = aero_data_spec_by_name(aero_data_ptr_f, "H2O")
       aero_dist_init%mode(i_mode)%vol_frac(i_water) = 1.0
       allocate(aero_dist_init%mode(i_mode)%vol_frac_std(n_spec))
       aero_dist_init%mode(i_mode)%vol_frac_std = 0.0
       source = aero_data_source_by_name(aero_data_ptr_f, mode_names(i_mode))
       aero_dist_init%mode(i_mode)%source = source 
    end do

    call aero_state_add_aero_dist_sample(ptr_f, aero_data_ptr_f, &
           aero_dist_init, 1d0, 0d0, &
           .true., & ! TODO run_part_opt%allow_doubling, &
           .true., & ! TODO run_part_opt%allow_halving)
           n_part_added)

  end subroutine

  subroutine f_aero_state_len(ptr_c, len) bind(C)
    type(aero_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(out) :: len 

    call c_f_pointer(ptr_c, ptr_f)
    len = aero_state_n_part(ptr_f)
  end subroutine

  subroutine f_aero_state_num_concs(ptr_c, aero_data_ptr_c, &
    num_concs, n_parts) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    integer(c_int) :: n_parts
    real(c_double) :: num_concs(n_parts)

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    num_concs = aero_state_num_concs(ptr_f, aero_data_ptr_f)

  end subroutine

  subroutine f_aero_state_total_num_conc(ptr_c, aero_data_ptr_c, &
      total_num_conc) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    real(c_double) :: total_num_conc
 
    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    total_num_conc = aero_state_total_num_conc(ptr_f, aero_data_ptr_f)

  end  subroutine

  ! FIXME: add include and exclude 
  subroutine f_aero_state_masses(ptr_c, aero_data_ptr_c, masses, n_parts) &
       bind(C) 

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    integer(c_int) :: n_parts
    real(c_double) :: masses(n_parts)

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    masses =  aero_state_masses(ptr_f, aero_data_ptr_f)

  end subroutine

  subroutine f_aero_state_dry_diameters(ptr_c, aero_data_ptr_c, diameters, n_parts) &
       bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    integer(c_int) :: n_parts
    real(c_double) :: diameters(n_parts)

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    diameters =  aero_state_dry_diameters(ptr_f, aero_data_ptr_f)

  end subroutine

  subroutine f_aero_state_diameters(ptr_c, aero_data_ptr_c, diameters, &
       n_parts) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    integer(c_int) :: n_parts
    real(c_double) :: diameters(n_parts)

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    diameters =  aero_state_diameters(ptr_f, aero_data_ptr_f)

  end subroutine

  ! FIXME: add include and exclude
  subroutine f_aero_state_volumes(ptr_c, aero_data_ptr_c, volumes, n_parts) &
       bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    integer(c_int) :: n_parts
    real(c_double) :: volumes(n_parts)

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    volumes =  aero_state_volumes(ptr_f, aero_data_ptr_f)

  end subroutine

end module
