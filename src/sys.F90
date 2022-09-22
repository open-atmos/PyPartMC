!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module pmc_sys
  use iso_c_binding

  interface
    subroutine c_stop(code) bind(C)
      integer, intent(in) :: code
    end subroutine
  end interface

  contains

  subroutine pmc_stop(code)
    integer, intent(in) :: code
    call c_stop(code)
  end subroutine
end module
