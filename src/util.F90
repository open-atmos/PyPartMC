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

end module
