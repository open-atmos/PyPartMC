!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_env_state
    use iso_c_binding
    use pmc_env_state
    use camp_env_state, only: camp_env_state_t => env_state_t
    implicit none

    contains

    subroutine f_env_state_ctor(ptr_c) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(out) :: ptr_c

        allocate(ptr_f)
        ptr_f%elapsed_time = 0
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

    subroutine f_env_state_get_temperature(ptr_c, temperature) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(out) :: temperature

        call c_f_pointer(ptr_c, ptr_f)

        temperature = ptr_f%temp

    end subroutine

    subroutine f_env_state_get_rel_humid(ptr_c, rel_humid) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(out) :: rel_humid

        call c_f_pointer(ptr_c, ptr_f)

        rel_humid = ptr_f%rel_humid

    end subroutine



    subroutine f_env_state_set_height(ptr_c, height) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(in) :: height

        call c_f_pointer(ptr_c, ptr_f)

        ptr_f%height = height

    end subroutine

    subroutine f_env_state_get_height(ptr_c, height) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(out) :: height
        
        call c_f_pointer(ptr_c, ptr_f)

        height = ptr_f%height

    end subroutine

    subroutine f_env_state_set_pressure(ptr_c, pressure) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(in) :: pressure

        call c_f_pointer(ptr_c, ptr_f)

        ptr_f%pressure = pressure

    end subroutine

    subroutine f_env_state_get_pressure(ptr_c, pressure) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(out) :: pressure

        call c_f_pointer(ptr_c, ptr_f)

        pressure = ptr_f%pressure

    end subroutine

    subroutine f_env_state_get_elapsed_time(ptr_c, elapsed_time) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(out) :: elapsed_time 

        call c_f_pointer(ptr_c, ptr_f)

        elapsed_time = ptr_f%elapsed_time

    end subroutine

    subroutine f_env_state_get_start_time(ptr_c, start_time) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(out) :: start_time

        call c_f_pointer(ptr_c, ptr_f)

        start_time = ptr_f%start_time

    end subroutine

end module
