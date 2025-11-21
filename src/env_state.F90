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

    subroutine f_env_state_set_additive_kernel_coefficient(ptr_c, value) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(in) :: value

        call c_f_pointer(ptr_c, ptr_f)

        ptr_f%additive_kernel_coefficient = value

    end subroutine

    subroutine f_env_state_get_additive_kernel_coefficient(ptr_c, target) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(out) :: target

        call c_f_pointer(ptr_c, ptr_f)

        target = ptr_f%additive_kernel_coefficient

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

    subroutine f_env_state_air_dens(ptr_c, air_density) bind(C)
        type(env_state_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double), intent(out) :: air_density

        call c_f_pointer(ptr_c, ptr_f)

        air_density = env_state_air_den(ptr_f)

    end subroutine

    subroutine f_env_state_air_molar_dens(ptr_c, air_molar_density) bind(C)
      type(env_state_t), pointer :: ptr_f => null()
      type(c_ptr), intent(in) :: ptr_c
      real(c_double), intent(out) :: air_molar_density

      call c_f_pointer(ptr_c, ptr_f)

      air_molar_density = env_state_air_molar_den(ptr_f)

    end subroutine

    subroutine f_env_state_get_latitude(ptr_c, latitude) bind(C)
      type(env_state_t), pointer :: ptr_f => null()
      type(c_ptr), intent(in) :: ptr_c
      real(c_double), intent(out) :: latitude

      call c_f_pointer(ptr_c, ptr_f)

      latitude = ptr_f%latitude

    end subroutine

    subroutine f_env_state_set_latitude(ptr_c, latitude) bind(C)
      type(env_state_t), pointer :: ptr_f => null()
      type(c_ptr), intent(inout) :: ptr_c
      real(c_double), intent(in) :: latitude

      call c_f_pointer(ptr_c, ptr_f)

      ptr_f%latitude = latitude

    end subroutine

    subroutine f_env_state_get_longitude(ptr_c, longitude) bind(C)
      type(env_state_t), pointer :: ptr_f => null()
      type(c_ptr), intent(in) :: ptr_c
      real(c_double), intent(out) :: longitude

      call c_f_pointer(ptr_c, ptr_f)

      longitude = ptr_f%longitude

    end subroutine

    subroutine f_env_state_set_longitude(ptr_c, longitude) bind(C)
      type(env_state_t), pointer :: ptr_f => null()
      type(c_ptr), intent(inout) :: ptr_c
      real(c_double), intent(in) :: longitude

      call c_f_pointer(ptr_c, ptr_f)

      ptr_f%longitude = longitude

    end subroutine

    subroutine f_env_state_get_altitude(ptr_c, altitude) bind(C)
      type(env_state_t), pointer :: ptr_f => null()
      type(c_ptr), intent(in) :: ptr_c
      real(c_double), intent(out) :: altitude

      call c_f_pointer(ptr_c, ptr_f)

      altitude = ptr_f%altitude

    end subroutine

    subroutine f_env_state_set_altitude(ptr_c, altitude) bind(C)
      type(env_state_t), pointer :: ptr_f => null()
      type(c_ptr), intent(inout) :: ptr_c
      real(c_double), intent(in) :: altitude

      call c_f_pointer(ptr_c, ptr_f)

      ptr_f%altitude = altitude

    end subroutine

    subroutine f_env_state_ppb_to_conc(ptr_c, ppb, conc) bind(C)
      type(env_state_t), pointer :: ptr_f => null()
      type(c_ptr), intent(in) :: ptr_c
      real(c_double), intent(in) :: ppb
      real(c_double), intent(out) :: conc

      call c_f_pointer(ptr_c, ptr_f)

      conc = env_state_ppb_to_conc(ptr_f, ppb)

    end subroutine

    subroutine f_env_state_conc_to_ppb(ptr_c, conc, ppb) bind(C)
      type(env_state_t), pointer :: ptr_f => null()
      type(c_ptr), intent(in) :: ptr_c
      real(c_double), intent(out) :: ppb
      real(c_double), intent(in) :: conc

      call c_f_pointer(ptr_c, ptr_f)

      ppb = env_state_conc_to_ppb(ptr_f, conc)

    end subroutine

    subroutine f_env_state_sat_vapor_pressure(ptr_c, sat_vapor_pressure) bind(C)
      type(env_state_t), pointer :: ptr_f => null()
      type(c_ptr), intent(in) :: ptr_c
      real(c_double), intent(out) :: sat_vapor_pressure

      call c_f_pointer(ptr_c, ptr_f)

      sat_vapor_pressure = env_state_sat_vapor_pressure(ptr_f)

    end subroutine

end module
