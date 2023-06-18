!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_output

use iso_c_binding
use pmc_output
use pmc_util

implicit none

contains

  subroutine f_output_state(aero_data_ptr_c, aero_state_ptr_c, &
       gas_data_ptr_c, gas_state_ptr_c, env_state_ptr_c, index, time, del_t, &
       i_repeat, record_removals, record_optical) bind(C)

    type(aero_state_t), pointer :: aero_state_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()
    type(gas_state_t), pointer :: gas_state_ptr_f => null()
    type(gas_data_t), pointer :: gas_data_ptr_f => null()

    type(c_ptr) :: aero_data_ptr_c, aero_state_ptr_c, gas_data_ptr_c, &
         gas_state_ptr_c, env_state_ptr_c
    integer(c_int) :: index, i_repeat
    real(c_double) :: time, del_t
    logical(c_bool) :: record_removals, record_optical

    character(len=PMC_UUID_LEN) :: uuid
    character(len=100) :: prefix
    integer :: output_type

    prefix = 'test'
    output_type = 3 
    call uuid4_str(uuid) 

    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(aero_state_ptr_c, aero_state_ptr_f)
    call c_f_pointer(gas_data_ptr_c, gas_data_ptr_f)
    call c_f_pointer(gas_state_ptr_c, gas_state_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    call output_state(prefix, output_type, aero_data_ptr_f, aero_state_ptr_f, &
         gas_data_ptr_f, gas_state_ptr_f, env_state_ptr_f, index, time, del_t, &
         i_repeat, logical(record_removals), logical(record_optical), uuid)

  end subroutine

  subroutine f_input_state(filename_data, filename_size, index, time, del_t, &
       i_repeat, aero_data_ptr_c, aero_state_ptr_c, gas_data_ptr_c, &
       gas_state_ptr_c, env_state_ptr_c) bind(C)

    type(aero_state_t), pointer :: aero_state_ptr_f => null()
    type(aero_data_t), pointer :: aero_data_ptr_f => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()
    type(gas_state_t), pointer :: gas_state_ptr_f => null()
    type(gas_data_t), pointer :: gas_data_ptr_f => null()
    character(kind=c_char), dimension(*), intent(in) :: filename_data
    integer(c_int), intent(in) :: filename_size
    character(len=filename_size) :: name
    integer :: i
    type(c_ptr) :: aero_data_ptr_c, aero_state_ptr_c, gas_data_ptr_c, &
         gas_state_ptr_c, env_state_ptr_c
    integer(c_int) :: index, i_repeat
    real(c_double) :: time, del_t
    character(len=PMC_UUID_LEN) :: uuid

    do i=1, filename_size
      name(i:i) = filename_data(i)
    end do

    call c_f_pointer(aero_data_ptr_c, aero_data_ptr_f)
    call c_f_pointer(aero_state_ptr_c, aero_state_ptr_f)
    call c_f_pointer(gas_data_ptr_c, gas_data_ptr_f)
    call c_f_pointer(gas_state_ptr_c, gas_state_ptr_f)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    call input_state(name, index, time, del_t, i_repeat, uuid, &
         aero_data_ptr_f, aero_state_ptr_f, gas_data_ptr_f, gas_state_ptr_f, &
         env_state_ptr_f)

  end subroutine

end module
