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

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    call aero_state_zero(ptr_f)
    call fractal_set_spherical(aero_data_ptr_f%fractal)
    call aero_state_set_weight(ptr_f, aero_data_ptr_f, AERO_STATE_WEIGHT_NUMMASS_SOURCE)
    call aero_state_set_n_part_ideal(ptr_f, n_part)
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

  end subroutine

  subroutine f_aero_state_total_mass_conc(ptr_c, aero_data_ptr_c, &
      total_mass_conc) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    real(c_double) :: total_mass_conc

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    total_mass_conc = sum(aero_state_num_concs(ptr_f, aero_data_ptr_f) &
         * aero_state_masses(ptr_f, aero_data_ptr_f))

  end subroutine

  ! TODO #130: add include and exclude 
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

  ! TODO #130: add include and exclude
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


  subroutine f_aero_state_crit_rel_humids(ptr_c, aero_data_ptr_c, &
       env_state_ptr_c, crit_rel_humids, n_parts) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c, env_state_ptr_c
    integer(c_int), intent(in) :: n_parts
    real(c_double) :: crit_rel_humids(n_parts)

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    crit_rel_humids = aero_state_crit_rel_humids(ptr_f, aero_data_ptr_f, &
         env_state_ptr_f)

  end subroutine

  ! TODO #130: Add include, exclude, group and groups
  subroutine f_aero_state_mixing_state_metrics(ptr_c, aero_data_ptr_c, & 
       d_alpha, d_gamma, chi) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    real(c_double) :: d_alpha
    real(c_double) :: d_gamma
    real(c_double) :: chi

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    call aero_state_mixing_state_metrics(ptr_f, aero_data_ptr_f, d_alpha, &
         d_gamma, chi)

  end subroutine

  subroutine f_aero_state_bin_average_comp(ptr_c, bin_grid_ptr_c, &
       aero_data_ptr_c) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(bin_grid_t), pointer :: bin_grid_ptr_f
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c, bin_grid_ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(bin_grid_ptr_c, bin_grid_ptr_f)

    call aero_state_bin_average_comp(ptr_f, bin_grid_ptr_f, aero_data_ptr_f)

  end subroutine

  subroutine f_aero_state_copy(ptr_c,ptr_new_c) bind(C)
    type(c_ptr) :: ptr_c, ptr_new_c
    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_state_t), pointer :: ptr_new_f => null()

    call c_f_pointer(ptr_c,ptr_f)
    call c_f_pointer(ptr_new_c, ptr_new_f)

    ptr_new_f = ptr_f
  end subroutine

  subroutine f_aero_state_particle(ptr_c, ptr_particle_c, index) bind(C)
    type(c_ptr) :: ptr_c, ptr_particle_c
    integer(c_int) :: index
    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_particle_t), pointer :: ptr_particle_f => null()

    call c_f_pointer(ptr_c,ptr_f)
    call c_f_pointer(ptr_particle_c, ptr_particle_f)

    ptr_particle_f = ptr_f%apa%particle(index + 1)

  end subroutine

  subroutine f_aero_state_rand_particle(ptr_c, ptr_particle_c) bind(C)
    type(c_ptr) :: ptr_c, ptr_particle_c
    integer(c_int) :: index
    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_particle_t), pointer :: ptr_particle_f => null()

    call c_f_pointer(ptr_c,ptr_f)
    call c_f_pointer(ptr_particle_c, ptr_particle_f)

    call aero_state_rand_particle(ptr_f, index)

    ptr_particle_f = ptr_f%apa%particle(index)

  end subroutine

  subroutine f_aero_state_add_aero_dist_sample(ptr_c, ptr_aero_data_c, &
       ptr_aero_dist_c, sample_prop, create_time, allow_doubling, &
       allow_halving, n_part_add) bind(C)

    type(c_ptr) :: ptr_c, ptr_aero_data_c, ptr_aero_dist_c
    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: ptr_aero_data_f => null()
    type(aero_dist_t), pointer :: ptr_aero_dist_f => null()
    real(c_double) :: sample_prop, create_time
    logical(c_bool) :: allow_doubling, allow_halving
    integer(c_int) :: n_part_add

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(ptr_aero_data_c,ptr_aero_data_f)
    call c_f_pointer(ptr_aero_dist_c,ptr_aero_dist_f)

    call aero_state_add_aero_dist_sample(ptr_f, ptr_aero_data_f, &
       ptr_aero_dist_f, sample_prop, create_time, LOGICAL(allow_doubling), &
       logical(allow_halving), n_part_add)

  end subroutine

end module
