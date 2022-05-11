!###################################################################################################
! This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
! Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
! Author: Sylwester Arabas                                                                         #
!###################################################################################################

module mpi
  integer, parameter :: mpi_STATUS_SIZE = 4

  interface
    subroutine mpi_abort(comm, errorcode, ierror) bind(C)
      integer :: comm, errorcode, ierror
    end subroutine
  end interface

  contains

  subroutine mpi_Bcast(buffer, count, datatype, root, comm, ierror)
    TYPE(*), DIMENSION(..) :: buffer
    integer, intent(IN) :: count, root
    integer, intent(IN) :: datatype
    integer, intent(IN) :: comm 
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_COMM_RANK(COMM, RANK, IERR)
    integer :: COMM, RANK, IERROR
    RANK = 0
  end subroutine

  subroutine mpi_COMM_SIZE(COMM, SIZE, IERR)
    integer :: COMM, SIZE, IERROR
    SIZE = 1
  end subroutine

  subroutine mpi_Pack(inbuf, incount, datatype, outbuf, outsize, position, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: inbuf
    TYPE(*), DIMENSION(..) :: outbuf
    integer, intent(IN) :: incount, outsize, datatype, comm
    integer, intent(INout) :: position
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Pack_size(incount, datatype, comm, size, ierror)
    integer, intent(IN) :: incount, datatype, comm
    integer, intent(out) :: size
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Unpack(inbuf, insize, position, outbuf, outcount, datatype, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: inbuf
    TYPE(*), DIMENSION(..) :: outbuf
    integer, intent(IN) :: insize, outcount, datatype, comm
    integer, intent(INout) :: position
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    integer, intent(IN) :: count, root, datatype, op, comm
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Recv(buf, count, datatype, source, tag, comm, status, ierror)
    TYPE(*), DIMENSION(..) :: buf
    integer, intent(IN) :: count, source, tag, datatype, comm
    integer, DIMENSION(mpi_STATUS_SIZE) :: status
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Send(buf, count, datatype, dest, tag, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: buf
    integer, intent(IN) :: count, dest, tag, datatype, comm
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Allreduce(sendbuf, recvbuf, count, datatype, op, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    integer, intent(IN) :: count, datatype, op, comm
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    integer, intent(IN) :: sendcount, recvcount, sendtype, recvtype, comm
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Get_count(status, datatype, count, ierror)
    integer, DIMENSION(mpi_STATUS_SIZE), intent(IN) :: status
    integer, intent(IN) :: datatype
    integer, intent(out) :: count
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    integer, intent(IN) :: sendcount, recvcount, sendtype, recvtype, comm
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Barrier(comm, ierror)
    integer, intent(IN) :: comm
    integer, optional, intent(out) :: ierror
  end subroutine
 
  subroutine mpi_Buffer_detach(buffer_addr, size, ierror)
    TYPE(*), DIMENSION(..) :: buffer_addr
    integer, intent(out) :: size
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf, recvcounts, rdispls, &
      recvtype, comm, ierror &
  )
    TYPE(*), DIMENSION(..), intent(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    integer, intent(IN) :: sendcounts(*), sdispls(*), recvcounts(*), rdispls(*)
    integer, intent(in) :: sendtype, recvtype, comm
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Probe(source, tag, comm, status, ierror)
    integer, intent(IN) :: source, tag, comm
    integer, DIMENSION(mpi_STATUS_SIZE) :: status
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Init(ierror)
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Bsend(buf, count, datatype, dest, tag, comm, ierror)
    TYPE(*), DIMENSION(..), intent(IN) :: buf
    integer, intent(IN) :: count, dest, tag, datatype, comm
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Finalize(ierror)
    integer, optional, intent(out) :: ierror
  end subroutine

  subroutine mpi_Buffer_attach(buffer, size, ierror)
    type(*), dimension(..), ASYNCHRONOUS :: buffer
    integer, intent(IN) :: size
    integer, optional, intent(out) :: ierror
  end subroutine
end module
