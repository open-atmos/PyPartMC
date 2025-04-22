!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2025 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_aero_binned
  use iso_c_binding
  use pmc_aero_binned
  implicit none

  contains

  subroutine f_aero_binned_ctor(ptr_c) bind(C)
    type(aero_binned_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_aero_binned_dtor(ptr_c) bind(C)
    type(aero_binned_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine

  subroutine f_aero_binned_num_conc(ptr_c, num_conc, n_bins) bind(C)
    type(aero_binned_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(in) :: n_bins
    real(c_double), intent(inout) :: num_conc(n_bins)

    call c_f_pointer(ptr_c, ptr_f)

    num_conc = ptr_f%num_conc

  end subroutine

  subroutine f_aero_binned_species_vol_conc(ptr_c, vol_conc, n_bins, i_spec) bind(C)
    type(aero_binned_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(in) :: n_bins, i_spec
    real(c_double), intent(inout) :: vol_conc(n_bins)

    call c_f_pointer(ptr_c, ptr_f)

    vol_conc = ptr_f%vol_conc(:,i_spec+1)

  end subroutine

  subroutine f_aero_binned_len(ptr_c, len) bind(C)
    type(aero_binned_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    integer(c_int), intent(out) :: len

    call c_f_pointer(ptr_c, ptr_f)

    len = size(ptr_f%num_conc)

  end subroutine

  subroutine f_aero_binned_add_aero_dist(ptr_c, bin_grid_ptr_c, aero_data_ptr_c, &
       aero_dist_ptr_c) bind(C)
    type(c_ptr), intent(in) :: ptr_c, bin_grid_ptr_c, aero_data_ptr_c, &
        aero_dist_ptr_c
    type(aero_binned_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(aero_dist_t), pointer :: aero_dist_ptr_f => null()
    type(bin_grid_t), pointer :: bin_grid_ptr_f => null()

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(bin_grid_ptr_c, bin_grid_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(aero_dist_ptr_c, aero_dist_ptr_f)

    call aero_binned_add_aero_dist(ptr_f, bin_grid_ptr_f, &
         aero_data_ptr_f, aero_dist_ptr_f)

  end subroutine

  subroutine f_aero_binned_set_sizes(ptr_c, aero_data_ptr_c, bin_grid_ptr_c) bind(C)
    type(c_ptr), intent(in) :: ptr_c, bin_grid_ptr_c, aero_data_ptr_c
    type(aero_binned_t), pointer :: ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(bin_grid_t), pointer :: bin_grid_ptr_f => null()

    call c_f_pointer(ptr_c, ptr_f)
    call c_f_pointer(bin_grid_ptr_c, bin_grid_ptr_f)
    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)

    call aero_binned_set_sizes(ptr_f, bin_grid_size(bin_grid_ptr_f), &
         aero_data_n_spec(aero_data_ptr_f))

  end subroutine

end module
