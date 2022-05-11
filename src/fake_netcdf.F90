!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Author: Sylwester Arabas                                                                         #
!###################################################################################################

module netcdf
  implicit none
  integer, parameter :: NF90_MAX_NAME=100

  interface
    function nf90_put_var_dbl(ncid, varid, values, start, count) bind(C)
      integer, intent(in) :: ncid, varid
      type(double precision), dimension(..), intent(in) :: values
      integer, dimension(:), optional, intent(in) :: start, count
      integer :: nf90_put_var_dbl
    end function

    function nf90_put_var_int(ncid, varid, values, start, count) bind(C)
      integer, intent(in) :: ncid, varid
      type(integer), dimension(..), intent(in) :: values
      integer, dimension(:), optional, intent(in) :: start, count
      integer :: nf90_put_var_int
    end function

    function nf90_get_var_dbl(ncid, varid, values, start, count) bind(C)
      integer, intent(in) :: ncid, varid
      type(double precision), dimension(..), intent(in) :: values
      integer, dimension(:), optional, intent(in) :: start, count
      integer :: nf90_get_var_dbl
    end function

    function nf90_get_var_int(ncid, varid, values, start, count) bind(C)
      integer, intent(in) :: ncid, varid
      type(integer), dimension(..), intent(in) :: values
      integer, dimension(:), optional, intent(in) :: start, count
      integer :: nf90_get_var_int
    end function
  end interface

  interface nf90_put_var
    module procedure nf90_put_var_dbl_
    module procedure nf90_put_var_int_
  end interface

  interface nf90_get_var
    module procedure nf90_get_var_dbl_
    module procedure nf90_get_var_int_
  end interface

  contains

  function nf90_strerror(ncerr)
    integer, intent(in) :: ncerr
    character(len = 80)  :: nf90_strerror
  end function

  function nf90_put_var_dbl_(ncid, varid, values, start, count)
    integer, intent(in) :: ncid, varid
    type(double precision), dimension(..), intent(in) :: values
    integer, dimension(:), optional, intent(in) :: start, count
    integer :: nf90_put_var_dbl_
    nf90_put_var_dbl_ = nf90_put_var_dbl(ncid, varid, values, start, count)
  end function

  function nf90_put_var_int_(ncid, varid, values, start, count)
    integer, intent(in) :: ncid, varid
    type(integer), dimension(..), intent(in) :: values
    integer, dimension(:), optional, intent(in) :: start, count
    integer :: nf90_put_var_int_
    nf90_put_var_int_ = nf90_put_var_int(ncid, varid, values, start, count)
  end function

  function nf90_inquire_variable(ncid, varid, name, xtype, ndims, dimids, nAtts)
    integer, intent(in) :: ncid, varid
    character (len = *), optional, intent(out) :: name
    integer, dimension(:), optional, intent(out) :: dimids
    integer, optional, intent(out) :: xtype, ndims 
    integer, optional, intent(out) :: nAtts
    integer :: nf90_inquire_variable
  end function

  function nf90_inquire_dimension(ncid, dimid, name, len)
    integer,                       intent( in) :: ncid, dimid
    character (len = *), optional, intent(out) :: name
    integer,             optional, intent(out) :: len
    integer                                    :: nf90_inquire_dimension
  end function 

  function nf90_get_var_dbl_(ncid, varid, values, start, count)
    integer, intent(in) :: ncid, varid
    type(double precision), dimension(..) :: values
    integer, dimension(:), optional, intent(in) :: start, count
    integer :: nf90_get_var_dbl_
    nf90_get_var_dbl_ = nf90_get_var_dbl(ncid, varid, values, start, count)
  end function

  function nf90_get_var_int_(ncid, varid, values, start, count)
    integer, intent(in) :: ncid, varid
    type(integer), dimension(..) :: values
    integer, dimension(:), optional, intent(in) :: start, count
    integer :: nf90_get_var_int_
    nf90_get_var_int_ = nf90_get_var_int(ncid, varid, values, start, count)
  end function

  function nf90_create(path, cmode, ncid)
    character (len = *), intent(in) :: path
    integer, intent(in) :: cmode
    integer, intent(out) :: ncid
    integer :: nf90_create
  end function

  function nf90_inq_dimid(ncid, name, dimid)
    integer,             intent( in) :: ncid
    integer,             intent(out) :: dimid
    integer                          :: nf90_inq_dimid
    character (len = *), intent(in) :: name
  end function 

  function nf90_inq_varid(ncid, name, varid)
    integer,             intent( in) :: ncid
    integer,             intent(out) :: varid
    integer                          :: nf90_inq_varid
    character (len = *), intent(in) :: name
  end function

  function nf90_enddef(ncid)
    integer,           intent( in) :: ncid
    integer                          :: nf90_enddef
  end function

  function nf90_put_att(ncid, varid, name, values)
    integer,            intent( in) :: ncid, varid
    character(len = *), intent( in) :: name
    type(*), dimension(..), intent( in) :: values
    integer                         :: nf90_put_att
    print*, "nf90_put_att", varid
  end function

  function nf90_def_dim(ncid, name, len, dimid)
    integer,             intent( in) :: ncid
    character (len = *), intent( in) :: name
    integer,             intent( in) :: len
    integer,             intent(out) :: dimid
    integer                          :: nf90_def_dim
  end function 

  function nf90_open(path, mode, ncid)
    character (len = *), intent(in) :: path
    integer, intent(in) :: mode
    integer, intent(out) :: ncid
    integer :: nf90_open
  end function

  function nf90_redef(ncid)
    integer, intent( in) :: ncid
    integer              :: nf90_redef
  end function

  function nf90_def_var(ncid, name, xtype, dimids, varid)
    integer, intent(in) :: ncid
    character (len = *), intent(in) :: name
    integer, intent( in) :: xtype
    integer, dimension(..), intent(in), optional :: dimids
    integer, intent(out) :: varid
    integer                                      :: nf90_def_var
  end function

  function nf90_close(ncid)
    integer, intent( in) :: ncid
    integer              :: nf90_close
  end function

  function nf90_get_att(ncid, varid, name, values)
    integer,            intent( in) :: ncid, varid
    character(len = *), intent( in) :: name
    type(*), dimension(..) :: values
    integer                         :: nf90_get_att
  end function
end module
