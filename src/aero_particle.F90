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
    call aero_particle_zero(ptr_f, aero_data_ptr_f)
    call aero_particle_set_vols(ptr_f, arr_data)
  end subroutine

  subroutine f_aero_particle_volumes(ptr_c, arr_data, arr_size) bind(C)
    type(c_ptr), intent(in) :: ptr_c
    type(aero_particle_t), pointer :: aero_particle => null()
    integer(c_int), intent(in) :: arr_size
    real(c_double), dimension(arr_size), intent(out) :: arr_data

    call c_f_pointer(ptr_c, aero_particle)
    arr_data = aero_particle%vol
  end subroutine

  subroutine f_aero_particle_volume(ptr_c, vol) bind(C)
    type(aero_particle_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double), intent(out) :: vol

    call c_f_pointer(ptr_c, ptr_f)
    
    vol = aero_particle_volume(ptr_f)
  end subroutine

  subroutine f_aero_particle_species_volume(ptr_c, i_spec, vol) bind(C)
    type(aero_particle_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(in) :: i_spec
    real(c_double), intent(out) :: vol

    call c_f_pointer(ptr_c, ptr_f)

    vol = aero_particle_species_volume(ptr_f, i_spec+1)
  end subroutine

  subroutine f_aero_particle_dry_volume(aero_particle_ptr_c, aero_data_ptr_c, vol) bind(C)
    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    real(c_double), intent(out) :: vol

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    vol = aero_particle_dry_volume(aero_particle_ptr_f, aero_data_ptr_f)
  end subroutine

  subroutine f_aero_particle_radius(aero_particle_ptr_c, aero_data_ptr_c, radius) bind(C)
    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    real(c_double), intent(out) :: radius

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    radius = aero_particle_radius(aero_particle_ptr_f, aero_data_ptr_f)
  end subroutine

  subroutine f_aero_particle_dry_radius(aero_particle_ptr_c, aero_data_ptr_c, radius) bind(C)
    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    real(c_double), intent(out) :: radius

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    radius = aero_particle_dry_radius(aero_particle_ptr_f, aero_data_ptr_f)
  end subroutine

  subroutine f_aero_particle_diameter(aero_particle_ptr_c, aero_data_ptr_c, diameter) bind(C)
    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    real(c_double), intent(out) :: diameter

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    diameter = aero_particle_diameter(aero_particle_ptr_f, aero_data_ptr_f)
  end subroutine

  subroutine f_aero_particle_dry_diameter(aero_particle_ptr_c, aero_data_ptr_c, diameter) bind(C)
    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    real(c_double), intent(out) :: diameter

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    diameter = aero_particle_dry_diameter(aero_particle_ptr_f, aero_data_ptr_f)
  end subroutine

  subroutine f_aero_particle_mass(aero_particle_ptr_c, aero_data_ptr_c, mass) bind(C)
    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    real(c_double), intent(out) :: mass

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    mass = aero_particle_mass(aero_particle_ptr_f, aero_data_ptr_f)
  end subroutine

  subroutine f_aero_particle_species_mass( &
      aero_particle_ptr_c, &
      i_spec, &
      aero_data_ptr_c, &
      mass &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    integer(c_int), intent(in) :: i_spec
    real(c_double), intent(out) :: mass

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    mass = aero_particle_species_mass(aero_particle_ptr_f, i_spec+1, aero_data_ptr_f)
  end subroutine

  subroutine f_aero_particle_species_masses( &
      aero_particle_ptr_c, &
      aero_data_ptr_c, &
      size_masses, &
      masses &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    integer(c_int), intent(in) :: size_masses
    real(c_double), dimension(size_masses), intent(out) :: masses

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    masses = aero_particle_species_masses(aero_particle_ptr_f, aero_data_ptr_f)
  end subroutine

  subroutine f_aero_particle_solute_kappa(aero_particle_ptr_c, aero_data_ptr_c, kappa) bind(C)
    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    real(c_double), intent(out) :: kappa

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    kappa = aero_particle_solute_kappa(aero_particle_ptr_f, aero_data_ptr_f)
  end subroutine

  subroutine f_aero_particle_moles(aero_particle_ptr_c, aero_data_ptr_c, moles) bind(C)
    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    real(c_double), intent(out) :: moles

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    moles = aero_particle_moles(aero_particle_ptr_f, aero_data_ptr_f)
  end subroutine

  subroutine f_aero_particle_mobility_diameter( &
      aero_particle_ptr_c, &
      aero_data_ptr_c, &
      env_state_ptr_c, &
      mobility_diameter &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c, &
      env_state_ptr_c
    real(c_double), intent(out) :: mobility_diameter

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    mobility_diameter = aero_particle_mobility_diameter( &
        aero_particle_ptr_f, &
        aero_data_ptr_f, &
        env_state_ptr_f &
      )
  end subroutine

  subroutine f_aero_particle_density( &
      aero_particle_ptr_c, &
      aero_data_ptr_c, &
      density &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c
    real(c_double), intent(out) :: density

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    density = aero_particle_density( &
        aero_particle_ptr_f, &
        aero_data_ptr_f &
      )
  end subroutine

  subroutine f_aero_particle_approx_crit_rel_humid( &
      aero_particle_ptr_c, &
      aero_data_ptr_c, &
      env_state_ptr_c, &
      approx_crit_rel_humid &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c, &
      env_state_ptr_c
    real(c_double), intent(out) :: approx_crit_rel_humid

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    approx_crit_rel_humid = aero_particle_approx_crit_rel_humid( &
        aero_particle_ptr_f, &
        aero_data_ptr_f, &
        env_state_ptr_f &
      )
  end subroutine

  subroutine f_aero_particle_crit_rel_humid( &
      aero_particle_ptr_c, &
      aero_data_ptr_c, &
      env_state_ptr_c, &
      crit_rel_humid &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c, &
      env_state_ptr_c
    real(c_double), intent(out) :: crit_rel_humid

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    crit_rel_humid = aero_particle_crit_rel_humid( &
        aero_particle_ptr_f, &
        aero_data_ptr_f, &
        env_state_ptr_f &
      )
  end subroutine

  subroutine f_aero_particle_crit_diameter( &
      aero_particle_ptr_c, &
      aero_data_ptr_c, &
      env_state_ptr_c, &
      crit_diameter &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c, &
      env_state_ptr_c
    real(c_double), intent(out) :: crit_diameter

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    crit_diameter = aero_particle_crit_diameter( &
        aero_particle_ptr_f, &
        aero_data_ptr_f, &
        env_state_ptr_f &
      )
  end subroutine

  subroutine f_aero_particle_coagulate( &
      aero_particle_1_ptr_c, &
      aero_particle_2_ptr_c, &
      aero_particle_new_ptr_c &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_1_ptr_f => null()
    type(aero_particle_t), pointer :: aero_particle_2_ptr_f => null()
    type(aero_particle_t), pointer :: aero_particle_new_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_1_ptr_c, aero_particle_2_ptr_c
    type(c_ptr), intent(inout) :: aero_particle_new_ptr_c

    call c_f_pointer(aero_particle_1_ptr_c, aero_particle_1_ptr_f)
    call c_f_pointer(aero_particle_2_ptr_c, aero_particle_2_ptr_f)
    call c_f_pointer(aero_particle_new_ptr_c, aero_particle_new_ptr_f)

    call aero_particle_coagulate( &
        aero_particle_1_ptr_f, &
        aero_particle_2_ptr_f, &
        aero_particle_new_ptr_f &
      )

    aero_particle_new_ptr_c = c_loc(aero_particle_new_ptr_f)
  end subroutine

  subroutine f_aero_particle_zero( &
      aero_particle_ptr_c, &
      aero_data_ptr_c &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c, aero_data_ptr_c

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    call aero_particle_zero( &
      aero_particle_ptr_f, &
      aero_data_ptr_f &
    )
  end subroutine

  subroutine f_aero_particle_set_vols( &
      aero_particle_ptr_c, &
      vol_size, &
      volumes &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c
    integer(c_int), intent(in) :: vol_size
    real(c_double), dimension(vol_size), intent(in) :: volumes

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)

    call aero_particle_set_vols( &
      aero_particle_ptr_f, &
      volumes &
    )
  end subroutine

  subroutine f_aero_particle_absorb_cross_sect( &
      aero_particle_ptr_c, &
      absorb_cross_sect & 
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c
    real(c_double), intent(out) :: absorb_cross_sect    

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)

    absorb_cross_sect = aero_particle_ptr_f%absorb_cross_sect

  end subroutine

  subroutine f_aero_particle_scatter_cross_sect( &
      aero_particle_ptr_c, &
      scatter_cross_sect &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c
    real(c_double), intent(out) :: scatter_cross_sect

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)

    scatter_cross_sect = aero_particle_ptr_f%scatter_cross_sect

  end subroutine

  subroutine f_aero_particle_asymmetry( &
      aero_particle_ptr_c, &
      asymmetry &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c
    real(c_double), intent(out) :: asymmetry

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)

    asymmetry = aero_particle_ptr_f%asymmetry

  end subroutine

  subroutine f_aero_particle_greatest_create_time( &
      aero_particle_ptr_c, &
      greatest_create_time &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c
    real(c_double), intent(out) :: greatest_create_time

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)

    greatest_create_time = aero_particle_ptr_f%greatest_create_time

  end subroutine

  subroutine f_aero_particle_least_create_time( &
      aero_particle_ptr_c, &
      least_create_time &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c
    real(c_double), intent(out) :: least_create_time

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)

    least_create_time = aero_particle_ptr_f%least_create_time

  end subroutine

  subroutine f_aero_particle_n_orig_part( &
      aero_particle_ptr_c, &
      n_orig_part, &
      n_orig_part_size &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c
    integer(c_int), intent(in) :: n_orig_part_size
    integer(c_int), dimension(n_orig_part_size), intent(out) :: n_orig_part

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)

    n_orig_part = aero_particle_ptr_f%n_orig_part

  end subroutine

  subroutine f_aero_particle_id( & 
      aero_particle_ptr_c, &
      id &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c
    integer(c_int), intent(out) :: id

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)

    id = aero_particle_ptr_f%id

  end subroutine

  subroutine f_aero_particle_refract_shell( &
      aero_particle_ptr_c, &
      refract_shell &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c
    complex(c_double_complex), intent(out) :: refract_shell

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)

    refract_shell = aero_particle_ptr_f%refract_shell

  end subroutine

  subroutine f_aero_particle_refract_core( &
      aero_particle_ptr_c, &
      refract_core &
    ) bind(C)

    type(aero_particle_t), pointer :: aero_particle_ptr_f => null()
    type(c_ptr), intent(in) :: aero_particle_ptr_c
    complex(c_double_complex), intent(out) :: refract_core

    call c_f_pointer(aero_particle_ptr_c, aero_particle_ptr_f)

    refract_core = aero_particle_ptr_f%refract_core

  end subroutine

end module
