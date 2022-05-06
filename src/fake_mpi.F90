module mpi
  contains
  subroutine MPI_ABORT(COMM, ERRORCODE, IERROR)
    INTEGER        COMM, ERRORCODE, IERROR
  end subroutine MPI_ABORT
end module
