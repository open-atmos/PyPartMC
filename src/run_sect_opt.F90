!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2025 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_run_sect_opt  
  use pmc_run_sect
  use pmc_spec_file
  use pmc_output
  use iso_c_binding
  implicit none

  contains

  subroutine f_run_sect_opt_ctor(ptr_c) bind(C)
    type(run_sect_opt_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c

    allocate(ptr_f)
    ptr_c = c_loc(ptr_f)
  end subroutine

  subroutine f_run_sect_opt_dtor(ptr_c) bind(C)
    type(run_sect_opt_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine

  subroutine f_run_sect_opt_from_json(ptr_c, env_state_ptr_c) bind(C)
    type(run_sect_opt_t), pointer :: run_sect_opt => null()
    type(env_state_t), pointer :: env_state_ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c, env_state_ptr_c
    type(spec_file_t) :: file

    call c_f_pointer(ptr_c, run_sect_opt)
    call c_f_pointer(env_state_ptr_c, env_state_ptr_f)

    call spec_file_read_string(file, 'output_prefix', run_sect_opt%prefix)

    call spec_file_read_real(file, 't_max', run_sect_opt%t_max)
    call spec_file_read_real(file, 'del_t', run_sect_opt%del_t)
    call spec_file_read_real(file, 't_output', run_sect_opt%t_output)
    call spec_file_read_real(file, 't_progress', run_sect_opt%t_progress)

    call spec_file_read_logical(file, 'do_coagulation', &
         run_sect_opt%do_coagulation)
    if (run_sect_opt%do_coagulation) then
       call spec_file_read_coag_kernel_type(file, &
           run_sect_opt%coag_kernel_type)
       if (run_sect_opt%coag_kernel_type == COAG_KERNEL_TYPE_ADDITIVE) then
          call spec_file_read_real(file, 'additive_kernel_coeff', &
               env_state_ptr_f%additive_kernel_coefficient)
       end if
    else
       run_sect_opt%coag_kernel_type = COAG_KERNEL_TYPE_INVALID
    end if

  end subroutine

  subroutine f_run_sect_opt_t_max(ptr_c, t_max) bind(C)
    type(run_sect_opt_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double) :: t_max

    call c_f_pointer(ptr_c, ptr_f)

    t_max = ptr_f%t_max
               
  end subroutine

  subroutine f_run_sect_opt_del_t(ptr_c, del_t) bind(C)
    type(run_sect_opt_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    real(c_double) :: del_t 

    call c_f_pointer(ptr_c, ptr_f)

    del_t = ptr_f%del_t

  end subroutine

end module
