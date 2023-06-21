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
        integer(c_int) :: rand_init

        call c_f_pointer(ptr_c, run_part_opt)

        call spec_file_read_string(file, 'output_prefix', &
             run_part_opt%output_prefix)

        !!! TODO #55
        call spec_file_read_logical(file, 'do_coagulation', run_part_opt%do_coagulation)    
        if (run_part_opt%do_coagulation) then
           call spec_file_read_coag_kernel_type(file, &
                run_part_opt%coag_kernel_type)
        else
           run_part_opt%coag_kernel_type = COAG_KERNEL_TYPE_INVALID
        end if
        call spec_file_read_logical(file, 'do_parallel', run_part_opt%do_parallel)
       if (run_part_opt%do_parallel) then
#ifndef PMC_USE_MPI
          call spec_file_die_msg(929006384, file, &
               'cannot use parallel mode, support is not compiled in')
#endif
          call spec_file_read_output_type(file, run_part_opt%output_type)
          call spec_file_read_real(file, 'mix_timescale', &
               run_part_opt%mix_timescale)
          call spec_file_read_logical(file, 'gas_average', &
               run_part_opt%gas_average)
          call spec_file_read_logical(file, 'env_average', &
               run_part_opt%env_average)
          call spec_file_read_parallel_coag_type(file, &
               run_part_opt%parallel_coag_type)
       else
          run_part_opt%output_type = OUTPUT_TYPE_SINGLE
          run_part_opt%mix_timescale = 0d0
          run_part_opt%gas_average = .false.
          run_part_opt%env_average = .false.
          run_part_opt%parallel_coag_type = PARALLEL_COAG_TYPE_LOCAL
       end if

       call spec_file_read_logical(file, 'do_mosaic', run_part_opt%do_mosaic)
       if (run_part_opt%do_mosaic .and. (.not. mosaic_support())) then
          call spec_file_die_msg(230495366, file, &
               'cannot use MOSAIC, support is not compiled in')
       end if

       if (run_part_opt%do_mosaic) then
          call spec_file_read_logical(file, 'do_optical', &
               run_part_opt%do_optical)
       else
          run_part_opt%do_optical = .false.
       end if

       call spec_file_read_logical(file, 'do_nucleation', &
            run_part_opt%do_nucleation)
       !!! TODO #214
       if (run_part_opt%do_nucleation) then
          call spec_file_die_msg(230495367, file, &
               'cannot use nucleation, support is not compiled in')
       end if
!       if (run_part_opt%do_nucleation) then
!          call spec_file_read_nucleate_type(file, aero_data, &
!               run_part_opt%nucleate_type, run_part_opt%nucleate_source)
!       else
!          run_part_opt%nucleate_type = NUCLEATE_TYPE_INVALID
!       end if

       call spec_file_read_logical(file, 'do_condensation', &
            run_part_opt%do_condensation)
#ifndef PMC_USE_SUNDIALS
       call assert_msg(121370218, &
            run_part_opt%do_condensation .eqv. .false., &
            "cannot use condensation, SUNDIALS support is not compiled in")
#endif

       if (run_part_opt%do_mosaic .and. run_part_opt%do_condensation) then
          call spec_file_die_msg(599877805, file, &
               'cannot use MOSAIC and condensation simultaneously')
       end if

       call spec_file_read_real(file, 't_max', run_part_opt%t_max)
       call spec_file_read_real(file, 'del_t', run_part_opt%del_t)
       call spec_file_read_real(file, 't_output', run_part_opt%t_output)
       call spec_file_read_real(file, 't_progress', run_part_opt%t_progress)

       call spec_file_read_integer(file, 'rand_init', rand_init)
       call spec_file_read_logical(file, 'allow_doubling', run_part_opt%allow_doubling)
       call spec_file_read_logical(file, 'allow_halving', run_part_opt%allow_halving)

       call spec_file_read_logical(file, 'do_camp_chem', run_part_opt%do_camp_chem)

       run_part_opt%output_type = OUTPUT_TYPE_SINGLE

       run_part_opt%i_repeat = 1
       run_part_opt%n_repeat = 1

       call pmc_srand(0, 0)
       call uuid4_str(run_part_opt%uuid)

       call pmc_srand(rand_init, 0)

    end subroutine

    subroutine f_run_part_opt_t_max(ptr_c, t_max) bind(C)
        type(run_part_opt_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double) :: t_max

        call c_f_pointer(ptr_c, ptr_f)

        t_max = ptr_f%t_max
               
    end subroutine

    subroutine f_run_part_opt_del_t(ptr_c, del_t) bind(C)
        type(run_part_opt_t), pointer :: ptr_f => null()
        type(c_ptr), intent(in) :: ptr_c
        real(c_double) :: del_t 

        call c_f_pointer(ptr_c, ptr_f)

        del_t = ptr_f%del_t

    end subroutine

end module
