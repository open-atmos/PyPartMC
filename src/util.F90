module PyPartMC_util

use iso_c_binding
use pmc_util

implicit none

contains

subroutine py_pow2_above(n, res) bind(C)
   integer(c_int), intent(in) :: n
   integer(c_int), intent(out) :: res
   res = pow2_above(n)
end subroutine

double precision function py_deg2rad(deg) bind(C)
   double precision, intent(in) :: deg
   py_deg2rad = deg2rad(deg)
end function

end module
