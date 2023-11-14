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

  subroutine f_aero_state_init(ptr_c, aero_data_ptr_c, n_part, weight_c) bind(C)
    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr) :: ptr_c, aero_data_ptr_c
    real(c_double), intent(in) :: n_part
    character(c_char), intent(in) :: weight_c
    integer :: weight_f

    if (weight_c == "-") then
      weight_f = AERO_STATE_WEIGHT_NONE
    else if (weight_c == "n") then
      weight_f = AERO_STATE_WEIGHT_NUMMASS
    else if (weight_c == "N") then
      weight_f = AERO_STATE_WEIGHT_NUMMASS_SOURCE
    else if (weight_c == "f") then
      weight_f = AERO_STATE_WEIGHT_FLAT
    else if (weight_c == "F") then
      weight_f = AERO_STATE_WEIGHT_FLAT_SOURCE
    else if (weight_c == "p") then
      weight_f = AERO_STATE_WEIGHT_POWER
    else if (weight_c == "P") then
      weight_f = AERO_STATE_WEIGHT_POWER_SOURCE
    else
      call pmc_stop(666)
    end if

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    call aero_state_zero(ptr_f)
    call fractal_set_spherical(aero_data_ptr_f%fractal)
    call aero_state_set_weight(ptr_f, aero_data_ptr_f, weight_f)
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

  subroutine f_aero_state_masses(ptr_c, aero_data_ptr_c, masses, n_parts, &
       include_len, exclude_len, include, exclude) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    integer(c_int), intent(in) :: n_parts
    integer(c_int), intent(in) :: include_len
    integer(c_int), intent(in) :: exclude_len
    type(c_ptr), dimension(include_len), target, intent(in), optional :: include
    type(c_ptr), dimension(exclude_len), target, intent(in), optional :: exclude
    real(c_double), intent(out) :: masses(n_parts)

    character(len=AERO_NAME_LEN), allocatable :: include_array(:)
    character(len=AERO_NAME_LEN), allocatable :: exclude_array(:)
    character, pointer :: fstring(:)
    integer :: j, slen, i
    character(len=:), allocatable :: string_tmp_alloc

    if (present(include)) then
       allocate(include_array(include_len))
       do i = 1, include_len
          slen = 0
          call c_f_pointer(include(i), fstring, [AERO_NAME_LEN])
          do while(fstring(slen+1) /= c_null_char)
             slen = slen + 1
          end do
          allocate(character(len=slen) :: string_tmp_alloc)
          do j = 1,slen
             string_tmp_alloc(j:j) = fstring(j)
          end do
          include_array(i) = trim(string_tmp_alloc)
          deallocate(string_tmp_alloc)
      end do
    end if
    if (present(exclude)) then
       allocate(exclude_array(exclude_len))
       do i = 1,exclude_len
          slen = 0
          call c_f_pointer(exclude(i), fstring, [AERO_NAME_LEN])
          do while(fstring(slen+1) /= c_null_char)
             slen = slen + 1
          end do
          allocate(character(len=slen) :: string_tmp_alloc)
          do j = 1,slen
             string_tmp_alloc(j:j) = fstring(j)
          end do
          exclude_array(i) = trim(string_tmp_alloc)
          deallocate(string_tmp_alloc)
       end do
    end if

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    masses =  aero_state_masses(ptr_f, aero_data_ptr_f, include=include_array, &
        exclude=exclude_array)

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
       n_parts, include_len, exclude_len, include, exclude) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    integer(c_int), intent(in) :: n_parts
    real(c_double), intent(out) :: diameters(n_parts)
    integer(c_int), intent(in) :: include_len
    integer(c_int), intent(in) :: exclude_len
    type(c_ptr), dimension(include_len), target, intent(in), optional :: include
    type(c_ptr), dimension(exclude_len), target, intent(in), optional :: exclude

    character(len=AERO_NAME_LEN), allocatable :: include_array(:)
    character(len=AERO_NAME_LEN), allocatable :: exclude_array(:)
    character, pointer :: fstring(:)
    integer :: j, slen, i
    character(len=:), allocatable :: string_tmp_alloc

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    if (present(include)) then
       allocate(include_array(include_len))
       do i = 1, include_len
          slen = 0
          call c_f_pointer(include(i), fstring, [AERO_NAME_LEN])
          do while(fstring(slen+1) /= c_null_char)
             slen = slen + 1
          end do
          allocate(character(len=slen) :: string_tmp_alloc)
          do j = 1,slen
             string_tmp_alloc(j:j) = fstring(j)
          end do
          include_array(i) = trim(string_tmp_alloc)
          deallocate(string_tmp_alloc)
      end do
    end if
    if (present(exclude)) then
       allocate(exclude_array(exclude_len))
       do i = 1,exclude_len
          slen = 0
          call c_f_pointer(exclude(i), fstring, [AERO_NAME_LEN])
          do while(fstring(slen+1) /= c_null_char)
             slen = slen + 1
          end do
          allocate(character(len=slen) :: string_tmp_alloc)
          do j = 1,slen
             string_tmp_alloc(j:j) = fstring(j)
          end do
          exclude_array(i) = trim(string_tmp_alloc)
          deallocate(string_tmp_alloc)
       end do
    end if

    diameters =  aero_state_diameters(ptr_f, aero_data_ptr_f, &
         include=include_array, exclude=exclude_array)

  end subroutine

  subroutine f_aero_state_volumes(ptr_c, aero_data_ptr_c, volumes, n_parts, &
       include_len, exclude_len, include, exclude) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    integer(c_int), intent(in) :: n_parts
    real(c_double), intent(out) :: volumes(n_parts)
    integer(c_int), intent(in) :: include_len
    integer(c_int), intent(in) :: exclude_len
    type(c_ptr), dimension(include_len), target, intent(in), optional :: include
    type(c_ptr), dimension(exclude_len), target, intent(in), optional :: exclude

    character(len=AERO_NAME_LEN), allocatable :: include_array(:)
    character(len=AERO_NAME_LEN), allocatable :: exclude_array(:)
    character, pointer :: fstring(:)
    integer :: j, slen, i
    character(len=:), allocatable :: string_tmp_alloc

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    if (present(include)) then
       allocate(include_array(include_len))
       do i = 1, include_len
          slen = 0
          call c_f_pointer(include(i), fstring, [AERO_NAME_LEN])
          do while(fstring(slen+1) /= c_null_char)
             slen = slen + 1
          end do
          allocate(character(len=slen) :: string_tmp_alloc)
          do j = 1,slen
             string_tmp_alloc(j:j) = fstring(j)
          end do
          include_array(i) = trim(string_tmp_alloc)
          deallocate(string_tmp_alloc)
      end do
    end if
    if (present(exclude)) then
       allocate(exclude_array(exclude_len))
       do i = 1,exclude_len
          slen = 0
          call c_f_pointer(exclude(i), fstring, [AERO_NAME_LEN])
          do while(fstring(slen+1) /= c_null_char)
             slen = slen + 1
          end do
          allocate(character(len=slen) :: string_tmp_alloc)
          do j = 1,slen
             string_tmp_alloc(j:j) = fstring(j)
          end do
          exclude_array(i) = trim(string_tmp_alloc)
          deallocate(string_tmp_alloc)
       end do
    end if

    volumes =  aero_state_volumes(ptr_f, aero_data_ptr_f, &
         include=include_array, exclude=exclude_array)

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

  ! TODO #130: Add groups
  subroutine f_aero_state_mixing_state_metrics(ptr_c, aero_data_ptr_c, & 
       d_alpha, d_gamma, chi, include_len, exclude_len, group_len, &
       include, exclude, group) bind(C)

    type(aero_state_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, aero_data_ptr_c
    real(c_double) :: d_alpha
    real(c_double) :: d_gamma
    real(c_double) :: chi
    integer(c_int), intent(in) :: include_len
    integer(c_int), intent(in) :: exclude_len
    integer(c_int), intent(in) :: group_len
    type(c_ptr), dimension(include_len), target, intent(in), optional :: include
    type(c_ptr), dimension(exclude_len), target, intent(in), optional :: exclude
    type(c_ptr), dimension(exclude_len), target, intent(in), optional :: group

    character(len=AERO_NAME_LEN), allocatable :: include_array(:)
    character(len=AERO_NAME_LEN), allocatable :: exclude_array(:)
    character(len=AERO_NAME_LEN), allocatable :: group_array(:)
    character, pointer :: fstring(:)
    integer :: j, slen, i
    character(len=:), allocatable :: string_tmp_alloc

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    if (present(include)) then
       allocate(include_array(include_len))
       do i = 1, include_len
          slen = 0
          call c_f_pointer(include(i), fstring, [AERO_NAME_LEN])
          do while(fstring(slen+1) /= c_null_char)
             slen = slen + 1
          end do
          allocate(character(len=slen) :: string_tmp_alloc)
          do j = 1,slen
             string_tmp_alloc(j:j) = fstring(j)
          end do
          include_array(i) = trim(string_tmp_alloc)
          deallocate(string_tmp_alloc)
      end do
    end if
    if (present(exclude)) then
       allocate(exclude_array(exclude_len))
       do i = 1,exclude_len
          slen = 0
          call c_f_pointer(exclude(i), fstring, [AERO_NAME_LEN])
          do while(fstring(slen+1) /= c_null_char)
             slen = slen + 1
          end do
          allocate(character(len=slen) :: string_tmp_alloc)
          do j = 1,slen
             string_tmp_alloc(j:j) = fstring(j)
          end do
          exclude_array(i) = trim(string_tmp_alloc)
          deallocate(string_tmp_alloc)
       end do
    end if
    if (present(group)) then
       allocate(group_array(group_len))
       do i = 1,group_len
          slen = 0
          call c_f_pointer(group(i), fstring, [AERO_NAME_LEN])
          do while(fstring(slen+1) /= c_null_char)
             slen = slen + 1
          end do
          allocate(character(len=slen) :: string_tmp_alloc)
          do j = 1,slen
             string_tmp_alloc(j:j) = fstring(j)
          end do
          group_array(i) = trim(string_tmp_alloc)
          deallocate(string_tmp_alloc)
       end do
    end if

    call aero_state_mixing_state_metrics(ptr_f, aero_data_ptr_f, d_alpha, &
            d_gamma, chi, include=include_array, exclude=exclude_array, &
            group=group_array)

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
