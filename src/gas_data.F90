module PyPartMC_gas_data
  use iso_c_binding
  use pmc_gas_data
  implicit none

  contains

  subroutine f_gas_data_ctor(ptr_c, n) bind(C)
    type(gas_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c
    integer, intent(in) :: n

    allocate(ptr_f)
    !TODO call gas_data_initialize(ptr_f, n)
    ptr_c = transfer(c_loc(ptr_f), ptr_c)
  end subroutine

  subroutine f_gas_data_dtor(ptr_c) bind(C)
    type(gas_data_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    !TODO call gas_state_set_size(ptr_f, 0)
    deallocate(ptr_f)
  end subroutine

end module