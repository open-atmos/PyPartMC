!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
!###################################################################################################

module mpi
  use iso_c_binding
  implicit none
  integer, parameter :: MPI_STATUS_SIZE = 4

  integer, parameter :: &
    MPI_COMM_WORLD = -1, &
    MPI_DOUBLE_PRECISION = -1, &
    MPI_INTEGER = -1, &
    MPI_CHARACTER = -1, &
    MPI_LOGICAL = -1, &
    MPI_DOUBLE_COMPLEX = -1, &
    MPI_MIN = -1, &
    MPI_MAX = -1, &
    MPI_SUM = -1, &
    MPI_SUCCESS = -1, &
    MPI_TAG = 1, &  ! TODO this is used as status array index
    MPI_ANY_TAG = -1, &
    MPI_SOURCE = 1, & ! TODO this is used as status array index
    MPI_ANY_SOURCE = -1

  interface
    subroutine mpi_abort(comm, errorcode, ierror) bind(C)
      import :: c_int
      integer(kind=c_int), intent(in) :: comm, errorcode, ierror
    end subroutine
  end interface

  contains

  subroutine mpi_comm_rank(COMM, RANK, IERR)
    integer :: COMM, RANK, IERR
    rank = 0
    ierr = MPI_SUCCESS
  end subroutine

  subroutine mpi_comm_size(COMM, SIZE, IERR)
    integer :: COMM, SIZE, IERR
    size = 1
    ierr = MPI_SUCCESS
  end subroutine

  subroutine mpi_bcast(buffer, count, datatype, root, comm, ierror)
    TYPE(*), DIMENSION(..) :: buffer
    integer, intent(IN) :: count, root, datatype, comm 
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_pack(inbuf, incount, datatype, outbuf, outsize, position, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: inbuf
    TYPE(*), DIMENSION(..) :: outbuf
    integer, intent(in) :: incount, outsize, datatype, comm
    integer, intent(inout) :: position
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_pack_size(incount, datatype, comm, size, ierror)
    integer, intent(in) :: incount, datatype, comm
    integer, intent(out) :: size
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_unpack(inbuf, insize, position, outbuf, outcount, datatype, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: inbuf
    TYPE(*), DIMENSION(..) :: outbuf
    integer, intent(IN) :: insize, outcount, datatype, comm
    integer, intent(INout) :: position
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_reduce(sendbuf, recvbuf, count, datatype, op, root, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    integer, intent(IN) :: count, root, datatype, op, comm
    integer, optional, intent(out) :: ierror
    !recvbuf = sendbuf
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_recv(buf, count, datatype, source, tag, comm, status, ierror)
    TYPE(*), DIMENSION(..) :: buf
    integer, intent(IN) :: count, source, tag, datatype, comm
    integer, DIMENSION(mpi_STATUS_SIZE) :: status
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_send(buf, count, datatype, dest, tag, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: buf
    integer, intent(IN) :: count, dest, tag, datatype, comm
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_allreduce(sendbuf, recvbuf, count, datatype, op, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    integer, intent(IN) :: count, datatype, op, comm
    integer, optional, intent(out) :: ierror
    !recvbuf = sendbuf
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    integer, intent(IN) :: sendcount, recvcount, sendtype, recvtype, comm
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_get_count(status, datatype, count, ierror)
    integer, DIMENSION(mpi_STATUS_SIZE), intent(IN) :: status
    integer, intent(IN) :: datatype
    integer, intent(out) :: count
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    integer, intent(IN) :: sendcount, recvcount, sendtype, recvtype, comm
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_barrier(comm, ierror)
    integer, intent(IN) :: comm
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine
 
  subroutine mpi_buffer_detach(buffer_addr, size, ierror)
    TYPE(*), DIMENSION(..) :: buffer_addr
    integer, intent(out) :: size
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, &
      recvtype, comm, ierror &
  )
    TYPE(*), DIMENSION(..), intent(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    integer, intent(IN) :: sendcounts(*), sdispls(*), recvcounts(*), rdispls(*)
    integer, intent(in) :: sendtype, recvtype, comm
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_probe(source, tag, comm, status, ierror)
    integer, intent(IN) :: source, tag, comm
    integer, DIMENSION(mpi_STATUS_SIZE) :: status
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_init(ierror)
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_bsend(buf, count, datatype, dest, tag, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: buf
    integer, intent(IN) :: count, dest, tag, datatype, comm
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_finalize(ierror)
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

  subroutine mpi_buffer_attach(buffer, size, ierror)
    type(*), dimension(..), ASYNCHRONOUS :: buffer
    integer, intent(IN) :: size
    integer, optional, intent(out) :: ierror
    ierror = MPI_SUCCESS
  end subroutine

end module
