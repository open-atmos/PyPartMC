module PyPartMC_run_part_opt  
  use pmc_run_part
  use iso_c_binding
  implicit none

  contains

  subroutine f_run_part_opt_ctor(ptr_c, n) bind(C)
    type(run_part_opt_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c
    integer, intent(in) :: n

    allocate(ptr_f)
    ptr_c = transfer(c_loc(ptr_f), ptr_c)
  end subroutine

  subroutine f_run_part_opt_dtor(ptr_c) bind(C)
    type(run_part_opt_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine
end module