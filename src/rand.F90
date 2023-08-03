!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module PyPartMC_rand

use iso_c_binding
use pmc_rand

implicit none

contains

  subroutine f_pmc_srand(seed) bind(C)
    integer(c_int) :: seed
    integer(c_int) :: offset = 0 ! MPI not used

    call pmc_srand(seed, offset)

  end subroutine

  subroutine f_rand_normal(mean, stddev, val) bind(C)
    real(c_double) :: mean
    real(c_double) :: stddev
    real(c_double) :: val

    val = rand_normal(mean, stddev)

  end subroutine

end module
