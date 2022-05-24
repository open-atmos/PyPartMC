!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module netcdf
  use iso_c_binding
  implicit none
  integer, parameter :: &
    nf90_max_name=100, &
    nf90_noerr=666, &
    nf90_int=-1, &
    nf90_double=-1, &
    nf90_ebaddim=-1, &
    nf90_enotvar=-1, &
    nf90_clobber=-1, &
    nf90_nowrite=-1, &
    nf90_global=-1

  interface
    ! TODO: use C types here?
    subroutine nf90_put_var_dbl(ncid, varid, values, start, count) bind(C)
      integer, intent(in) :: ncid, varid
      type(double precision), dimension(..), intent(in) :: values
      integer, dimension(:), optional, intent(in) :: start, count
    end subroutine

    subroutine nf90_put_var_int(ncid, varid, values, start, count) bind(C)
      integer, intent(in) :: ncid, varid
      type(integer), dimension(..), intent(in) :: values
      integer, dimension(:), optional, intent(in) :: start, count
    end subroutine

    subroutine nf90_get_var_dbl(ncid, varid, values, start, count) bind(C)
      integer, intent(in) :: ncid, varid
      type(double precision), dimension(..), intent(in) :: values
      integer, dimension(:), optional, intent(in) :: start, count
    end subroutine

    subroutine nf90_get_var_int(ncid, varid, values, start, count) bind(C)
      integer, intent(in) :: ncid, varid
      type(integer), dimension(..), intent(in) :: values
      integer, dimension(:), optional, intent(in) :: start, count
    end subroutine

    subroutine nf90_inq_varid_str(ncid, name_data, name_size, varid) bind(C)
      integer, intent( in) :: ncid
      integer, intent(out) :: varid
      character, intent(in) :: name_data
      integer, intent(in) :: name_size
    end subroutine
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

  function nf90_inq_varid(ncid, name, varid)
    integer, intent( in) :: ncid
    integer, intent(out) :: varid
    integer :: nf90_inq_varid
    character (len=*), intent(in) :: name
    call nf90_inq_varid_str(ncid, name(1:1), len(name), varid)
    nf90_inq_varid = NF90_NOERR
  end function

  function nf90_strerror(ncerr)
    integer, intent(in) :: ncerr
    character(len = 80)  :: nf90_strerror
    nf90_strerror = "TODO"
  end function

  function nf90_put_var_dbl_(ncid, varid, values, start, count)
    integer, intent(in) :: ncid, varid
    type(double precision), dimension(..), intent(in) :: values
    integer, dimension(:), optional, intent(in) :: start, count
    integer :: nf90_put_var_dbl_
    call nf90_put_var_dbl(ncid, varid, values, start, count)
    nf90_put_var_dbl_ = nf90_noerr
  end function

  function nf90_put_var_int_(ncid, varid, values, start, count)
    integer, intent(in) :: ncid, varid
    type(integer), dimension(..), intent(in) :: values
    integer, dimension(:), optional, intent(in) :: start, count
    integer :: nf90_put_var_int_
    call nf90_put_var_int(ncid, varid, values, start, count)
    nf90_put_var_int_ = nf90_noerr
  end function

  function nf90_inquire_variable(ncid, varid, name, xtype, ndims, dimids, nAtts)
    integer, intent(in) :: ncid, varid
    character (len = *), optional, intent(out) :: name
    integer, dimension(:), optional, intent(out) :: dimids
    integer, optional, intent(out) :: xtype, ndims 
    integer, optional, intent(out) :: nAtts
    integer :: nf90_inquire_variable
    nf90_inquire_variable = nf90_noerr
  end function

  function nf90_inquire_dimension(ncid, dimid, name, len)
    integer, intent( in) :: ncid, dimid
    character (len = *), optional, intent(out) :: name
    integer, optional, intent(out) :: len
    integer :: nf90_inquire_dimension
    nf90_inquire_dimension = nf90_noerr
  end function 

  function nf90_get_var_dbl_(ncid, varid, values, start, count)
    integer, intent(in) :: ncid, varid
    type(double precision), dimension(..) :: values
    integer, dimension(:), optional, intent(in) :: start, count
    integer :: nf90_get_var_dbl_
    call nf90_get_var_dbl(ncid, varid, values, start, count)
    nf90_get_var_dbl_ = nf90_noerr
  end function

  function nf90_get_var_int_(ncid, varid, values, start, count)
    integer, intent(in) :: ncid, varid
    type(integer), dimension(..) :: values
    integer, dimension(:), optional, intent(in) :: start, count
    integer :: nf90_get_var_int_
    call nf90_get_var_int(ncid, varid, values, start, count)
    nf90_get_var_int_ = nf90_noerr
  end function

  function nf90_create(path, cmode, ncid)
    character (len = *), intent(in) :: path
    integer, intent(in) :: cmode
    integer, intent(out) :: ncid
    integer :: nf90_create
    nf90_create = nf90_noerr
  end function

  function nf90_inq_dimid(ncid, name, dimid)
    integer,             intent( in) :: ncid
    integer,             intent(out) :: dimid
    integer                          :: nf90_inq_dimid
    character (len = *), intent(in) :: name
    nf90_inq_dimid = nf90_noerr
  end function 

  function nf90_enddef(ncid)
    integer, intent( in) :: ncid
    integer :: nf90_enddef
    nf90_enddef = nf90_noerr
  end function

  function nf90_put_att(ncid, varid, name, values)
    integer,            intent( in) :: ncid, varid
    character(len = *), intent( in) :: name
    type(*), dimension(..), intent( in) :: values
    integer :: nf90_put_att
    nf90_put_att = nf90_noerr
  end function

  function nf90_def_dim(ncid, name, len, dimid)
    integer, intent( in) :: ncid, len
    character (len = *), intent( in) :: name
    integer, intent(out) :: dimid
    integer :: nf90_def_dim
    nf90_def_dim = nf90_noerr
  end function 

  function nf90_open(path, mode, ncid)
    character (len = *), intent(in) :: path
    integer, intent(in) :: mode
    integer, intent(out) :: ncid
    integer :: nf90_open
    nf90_open = nf90_noerr
  end function

  function nf90_redef(ncid)
    integer, intent( in) :: ncid
    integer :: nf90_redef
    nf90_redef = nf90_noerr
  end function

  function nf90_def_var(ncid, name, xtype, dimids, varid)
    integer, intent(in) :: ncid
    character (len = *), intent(in) :: name
    integer, intent( in) :: xtype
    integer, dimension(..), intent(in), optional :: dimids
    integer, intent(out) :: varid
    integer :: nf90_def_var
    nf90_def_var = nf90_noerr
  end function

  function nf90_close(ncid)
    integer, intent( in) :: ncid
    integer :: nf90_close
    nf90_close = nf90_noerr
  end function

  function nf90_get_att(ncid, varid, name, values)
    integer,            intent( in) :: ncid, varid
    character(len = *), intent( in) :: name
    type(*), dimension(..) :: values
    integer :: nf90_get_att
    nf90_get_att = nf90_noerr
  end function
end module
