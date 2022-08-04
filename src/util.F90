!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

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

subroutine f_sphere_vol2rad(v, rad) bind(C)
   real(c_double), intent(in) :: v
   real(c_double), intent(out) :: rad

   rad = sphere_vol2rad(v)

end subroutine

subroutine f_rad2diam(rad, d) bind(C)
   real(c_double), intent(in) :: rad
   real(c_double), intent(out) :: d

   d = rad2diam(rad)

end subroutine

subroutine f_sphere_rad2vol(rad, v) bind(C)
   real(c_double), intent(in) :: rad
   real(c_double), intent(out) :: v

   v = sphere_rad2vol(rad)

end subroutine

subroutine f_diam2rad(d, rad) bind(C)
   real(c_double), intent(in) :: d
   real(c_double), intent(out) :: rad

   rad = diam2rad(d)

end subroutine

end module
