module PyPartMC_gas_state
  use iso_c_binding
  use pmc_gas_state
  implicit none

  contains

  subroutine f_gas_state_ctor(ptr_c, n) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(out) :: ptr_c
    integer, intent(in) :: n

    allocate(ptr_f)
    call gas_state_set_size(ptr_f, n)
    ptr_c = transfer(c_loc(ptr_f), ptr_c)
  end subroutine

  subroutine f_gas_state_dtor(ptr_c) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c

    call c_f_pointer(ptr_c, ptr_f)
    call gas_state_set_size(ptr_f, 0)
    deallocate(ptr_f)
  end subroutine

  subroutine f_gas_state_set_item(ptr_c, values) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    double precision, dimension(:), intent(in) :: values

    type(gas_data_t), pointer :: gas_data => null()
    integer :: ncid

    call c_f_pointer(ptr_c, ptr_f)
    call gas_state_input_netcdf(ptr_f, ncid, gas_data)
  end subroutine

  subroutine f_gas_state_get_item(ptr_c, values) bind(C)
    type(gas_state_t), pointer :: ptr_f => null()
    type(c_ptr), intent(in) :: ptr_c
    double precision, dimension(:), intent(out) :: values

    type(gas_data_t), pointer :: gas_data => null()
    integer :: ncid

    call c_f_pointer(ptr_c, ptr_f)
    !call gas_state_output_netcdf(ptr_f, ncid, gas_data)
  end subroutine

end module
