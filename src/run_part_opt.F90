!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Author: Sylwester Arabas                                                                         #
!###################################################################################################

module PyPartMC_run_part_opt  
    use pmc_run_part
    use pmc_spec_file
    use iso_c_binding
    implicit none

    contains

    subroutine f_run_part_opt_ctor(ptr_c) bind(C)
        type(run_part_opt_t), pointer :: ptr_f => null()
        type(c_ptr), intent(out) :: ptr_c

        allocate(ptr_f)
        ptr_c = c_loc(ptr_f)
    end subroutine

    subroutine f_run_part_opt_dtor(ptr_c) bind(C)
        type(run_part_opt_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c

        call c_f_pointer(ptr_c, ptr_f)
        deallocate(ptr_f)
    end subroutine

    subroutine f_run_part_opt_from_json(ptr_c) bind(C)
        type(run_part_opt_t), pointer :: run_part_opt => null()
        type(c_ptr), intent(in) :: ptr_c
        type(spec_file_t) :: file

        call c_f_pointer(ptr_c, run_part_opt)

        !!! TODO #55
        call spec_file_read_logical(file, 'do_coagulation', run_part_opt%do_coagulation)    
    end subroutine
end module
