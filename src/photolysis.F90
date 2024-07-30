!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_photolysis
    use iso_c_binding
    use camp_camp_core
    use pmc_photolysis
    implicit none

    contains

    subroutine f_photolysis_ctor(ptr_c) bind(C)
        type(photolysis_t), pointer :: ptr_f => null()
        type(c_ptr), intent(out) :: ptr_c

        allocate(ptr_f)
        ptr_c = c_loc(ptr_f)
    end subroutine

    subroutine f_photolysis_create(ptr_c, camp_core_ptr_c) bind(C)
        type(photolysis_t), pointer :: ptr_f => null()
        type(camp_core_t), pointer :: camp_core_ptr_f => null()
        type(c_ptr), intent(inout) :: ptr_c
        type(c_ptr), intent(in) :: camp_core_ptr_c

        call c_f_pointer(ptr_c, ptr_f)
        call c_f_pointer(camp_core_ptr_c, camp_core_ptr_f)

        ptr_f => photolysis_t(camp_core_ptr_f)

        ptr_c = c_loc(ptr_f)
   end subroutine

    subroutine f_photolysis_dtor(ptr_c) bind(C)
        type(photolysis_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c

        call c_f_pointer(ptr_c, ptr_f)
        deallocate(ptr_f)
    end subroutine
end module
