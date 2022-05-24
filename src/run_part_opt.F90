!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_run_part_opt  
    use pmc_run_part
    use pmc_spec_file
    use pmc_output
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
        call spec_file_read_logical(file, 'do_parallel', run_part_opt%do_parallel)

        call spec_file_read_real(file, 't_max', run_part_opt%t_max)
        call spec_file_read_real(file, 'del_t', run_part_opt%del_t)
        call spec_file_read_real(file, 't_output', run_part_opt%t_output)
        call spec_file_read_real(file, 't_progress', run_part_opt%t_progress)

        call spec_file_read_logical(file, 'allow_doubling', run_part_opt%allow_doubling)
        call spec_file_read_logical(file, 'allow_halving', run_part_opt%allow_halving)

        run_part_opt%output_type = OUTPUT_TYPE_SINGLE

    end subroutine
end module
