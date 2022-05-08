module mpi

  integer, parameter :: MPI_STATUS_SIZE = 4

  contains

  subroutine MPI_ABORT(COMM, ERRORCODE, IERROR)
    INTEGER :: COMM, ERRORCODE, IERROR
  end subroutine MPI_ABORT

  subroutine MPI_Bcast(buffer, count, datatype, root, comm, ierror)
    TYPE(*), DIMENSION(..) :: buffer
    INTEGER, INTENT(IN) :: count, root
    INTEGER, INTENT(IN) :: datatype
    INTEGER, INTENT(IN) :: comm 
    INTEGER, OPTIONAL, INTENT(OUT) :: ierror
  end subroutine

  subroutine MPI_COMM_RANK(COMM, RANK, IERR)
    INTEGER :: COMM, RANK, IERROR
    RANK = 0
  end subroutine

  subroutine MPI_COMM_SIZE(COMM, SIZE, IERR)
    INTEGER :: COMM, SIZE, IERROR
    SIZE = 1
  end subroutine

  subroutine MPI_Pack(inbuf, incount, datatype, outbuf, outsize, position, comm, ierror)
    TYPE(*), DIMENSION(..), INTENT(IN) :: inbuf
    TYPE(*), DIMENSION(..) :: outbuf
    INTEGER, INTENT(IN) :: incount, outsize, datatype, comm
    INTEGER, INTENT(INOUT) :: position
    INTEGER, OPTIONAL, INTENT(OUT) :: ierror
  end subroutine

  subroutine MPI_Unpack(inbuf, insize, position, outbuf, outcount, datatype, comm, ierror)
    TYPE(*), DIMENSION(..), INTENT(IN) :: inbuf
    TYPE(*), DIMENSION(..) :: outbuf
    INTEGER, INTENT(IN) :: insize, outcount, datatype, comm
    INTEGER, INTENT(INOUT) :: position
    INTEGER, OPTIONAL, INTENT(OUT) :: ierror
  end subroutine

  subroutine MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm, ierror)
    TYPE(*), DIMENSION(..), INTENT(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    INTEGER, INTENT(IN) :: count, root, datatype, op, comm
    INTEGER, OPTIONAL, INTENT(OUT) :: ierror
  end subroutine

  subroutine MPI_Recv(buf, count, datatype, source, tag, comm, status, ierror)
    TYPE(*), DIMENSION(..) :: buf
    INTEGER, INTENT(IN) :: count, source, tag, datatype, comm
    INTEGER, DIMENSION(MPI_STATUS_SIZE) :: status
    INTEGER, OPTIONAL, INTENT(OUT) :: ierror
  end subroutine

  subroutine MPI_Send(buf, count, datatype, dest, tag, comm, ierror)
    TYPE(*), DIMENSION(..), INTENT(IN) :: buf
    INTEGER, INTENT(IN) :: count, dest, tag, datatype, comm
    INTEGER, OPTIONAL, INTENT(OUT) :: ierror
  end subroutine

  subroutine MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm, ierror)
    TYPE(*), DIMENSION(..), INTENT(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    INTEGER, INTENT(IN) :: count, datatype, op, comm
    INTEGER, OPTIONAL, INTENT(OUT) :: ierror
  end subroutine

  subroutine MPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm, ierror)
    TYPE(*), DIMENSION(..), INTENT(IN) :: sendbuf
    TYPE(*), DIMENSION(..) :: recvbuf
    INTEGER, INTENT(IN) :: sendcount, recvcount, sendtype, recvtype, comm
    INTEGER, OPTIONAL, INTENT(OUT) :: ierror
  end subroutine
end module
