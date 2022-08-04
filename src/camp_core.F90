!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_camp_core
    use iso_c_binding
    use camp_camp_core
    implicit none

    contains

    subroutine f_camp_core_ctor(ptr_c) bind(C)
        type(camp_core_t), pointer :: ptr_f => null()
        type(c_ptr), intent(out) :: ptr_c

        ptr_f => camp_core_t()
        call ptr_f%initialize()
        ptr_c = c_loc(ptr_f)
    end subroutine

    subroutine f_camp_core_dtor(ptr_c) bind(C)
        type(camp_core_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c

        call c_f_pointer(ptr_c, ptr_f)
        deallocate(ptr_f)
    end subroutine
end module
