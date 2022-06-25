!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_env_state
    use iso_c_binding
    use pmc_env_state
    implicit none

    contains

    subroutine f_env_state_ctor(ptr_c) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(out) :: ptr_c

        allocate(ptr_f)
        ptr_c = c_loc(ptr_f)
    end subroutine

    subroutine f_env_state_dtor(ptr_c) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c

        call c_f_pointer(ptr_c, ptr_f)
        deallocate(ptr_f)
    end subroutine

    subroutine f_env_state_from_json(ptr_c) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        type(spec_file_t) :: file
        call c_f_pointer(ptr_c, ptr_f)
        call spec_file_read_env_state(file, ptr_f)
    end subroutine

    subroutine f_env_state_set_temperature(ptr_c, temperature) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(in) :: temperature

        call c_f_pointer(ptr_c, ptr_f)

        ptr_f%temp = temperature

    end subroutine

end module
