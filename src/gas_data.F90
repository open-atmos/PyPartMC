!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_gas_data
    use iso_c_binding
    use pmc_gas_data
    implicit none

    contains

    subroutine f_gas_data_ctor(ptr_c) bind(C)
        type(gas_data_t), pointer :: ptr_f => null()
        type(c_ptr), intent(out) :: ptr_c

        allocate(ptr_f)
        ptr_c = c_loc(ptr_f)
    end subroutine

    subroutine f_gas_data_dtor(ptr_c) bind(C)
        type(gas_data_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c

        call c_f_pointer(ptr_c, ptr_f)
        deallocate(ptr_f)
    end subroutine

    subroutine f_gas_data_len(ptr_c, n_spec) bind(C)
        type(gas_data_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        integer(c_int), intent(out) :: n_spec

        call c_f_pointer(ptr_c, ptr_f)
        n_spec = gas_data_n_spec(ptr_f)
    end subroutine

    subroutine f_gas_data_from_json(ptr_c) bind(C)
        type(gas_data_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c

        type(spec_file_t) :: nofile

        call c_f_pointer(ptr_c, ptr_f)
        call spec_file_read_gas_data(nofile, ptr_f)
    end subroutine

    subroutine f_gas_data_spec_by_name(ptr_c, value, name_data, name_size) &
         bind(C)
      type(gas_data_t), pointer :: ptr_f => null()
      type(c_ptr), intent(in) :: ptr_c
      character(kind=c_char), dimension(*), intent(in) :: name_data
      integer(c_int), intent(in) :: name_size
      integer(c_int), intent(out) :: value

      integer :: i
      character(len=name_size) :: name

      call c_f_pointer(ptr_c, ptr_f)
      do i = 1, name_size
          name(i:i) = name_data(i)
      end do
      value = gas_data_spec_by_name(ptr_f, name)

    end subroutine

end module
