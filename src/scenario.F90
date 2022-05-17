!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Author: Sylwester Arabas                                                                         #
!###################################################################################################

module PyPartMC_scenario
  use iso_c_binding
  use pmc_scenario
  implicit none

  contains

  subroutine f_scenario_ctor(ptr_c) bind(C)
    type(scenario_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_scenario_dtor(ptr_c) bind(C)
    type(scenario_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine

  subroutine f_scenario_from_json(ptr_c) bind(C)
    type(scenario_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    type(gas_data_t), pointer :: gas_data
    type(aero_data_t), pointer :: aero_data
    type(spec_file_t) :: file 

    call c_f_pointer(ptr_c, ptr_f)
    allocate(gas_data) ! TODO
    allocate(aero_data) ! TODO
    call spec_file_read_scenario(file, gas_data, aero_data, ptr_f)
    deallocate(aero_data) ! TODO
    deallocate(gas_data) ! TODO
  end subroutine

end module
