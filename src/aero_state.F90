module PyPartMC_aero_state
  use iso_c_binding
  use pmc_aero_state
  implicit none

  contains

  subroutine f_aero_state_ctor(ptr_c, n) bind(C)
    type(aero_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c
    integer, intent(in) :: n

    allocate(ptr_f)
    ptr_c = transfer(c_loc(ptr_f), ptr_c)
  end subroutine

  subroutine f_aero_state_dtor(ptr_c) bind(C)
    type(aero_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    deallocate(ptr_f)
  end subroutine

end module
