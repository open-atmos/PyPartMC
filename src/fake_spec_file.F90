module pmc_spec_file
    use pmc_spec_line
    implicit none

    integer, parameter :: SPEC_FILE_MAX_LIST_LINES = 1000

    type spec_file_t
        character(len=SPEC_LINE_MAX_VAR_LEN) :: name = "<JSON input>"
    end type

    interface
        subroutine c_spec_file_read_string(name_data, name_size, var_data, var_size) bind(C)
            character, intent(in) :: name_data
            character, intent(out) :: var_data
            integer, intent(in) :: name_size, var_size
        end subroutine

        subroutine c_spec_file_read_real(name_data, name_size, var) bind(C)
            import c_double
            character, intent(in) :: name_data
            integer, intent(in) :: name_size
            real(c_double) :: var
        end subroutine

        subroutine c_spec_file_read_integer(name_data, name_size, var) bind(C)
            import c_int
            character, intent(in) :: name_data
            integer, intent(in) :: name_size
            integer(c_int) :: var
        end subroutine

        subroutine c_spec_file_read_logical(name_data, name_size, var) bind(C)
            import c_bool
            character, intent(in) :: name_data
            integer, intent(in) :: name_size
            logical(c_bool) :: var
        end subroutine

        subroutine c_spec_file_open(filename_data, filename_size) bind(C)
            character, intent(in) :: filename_data
            integer, intent(in) :: filename_size
        end subroutine

        subroutine c_spec_file_close() bind(C)
        end subroutine

        subroutine c_spec_file_read_timed_real_array_size(& 
            name_data, name_size, &
            times_size, &
            vals_size &
        ) bind(C)
            character, intent(in) :: name_data
            integer, intent(in) :: name_size, times_size, vals_size
        end subroutine

        subroutine c_spec_file_read_timed_real_array_data(& 
            name_data, name_size, &
            times_data, times_size, &
            vals_data, vals_size &
        ) bind(C)
            import c_double
            character, intent(in) :: name_data
            integer, intent(in) :: name_size, times_size, vals_size
            real(c_double), intent(out) :: times_data
            real(c_double), intent(out) :: vals_data
        end subroutine

        subroutine c_spec_file_read_real_named_array_size(n_rows, n_cols) bind(C)
            integer, intent(in) :: n_rows, n_cols
        end subroutine

        subroutine c_spec_file_read_real_named_array_data( &
            row, &
            names_data, names_size, &
            vals_data, vals_size &
        ) bind(C)
            import c_double
            character, intent(in) :: names_data
            real(c_double), intent(out) :: vals_data
            integer, intent(in) :: row, vals_size, names_size
        end subroutine
    end interface

    contains
  
    subroutine spec_file_check_line_name(file, line, name)
        type(spec_file_t), intent(in) :: file
        type(spec_line_t), intent(in) :: line
        character(len=*), intent(in) :: name
    end subroutine

    subroutine spec_file_read_real_named_array(file, max_lines, names, vals)
        type(spec_file_t), intent(inout) :: file
        integer, intent(in) :: max_lines
        character(len=SPEC_LINE_MAX_VAR_LEN), allocatable :: names(:)
        real(kind=c_double), allocatable :: vals(:,:), vals_row(:)

        integer :: row, col, n_rows, n_cols, name_size;

        call c_spec_file_read_real_named_array_size(n_rows, n_cols)
        allocate(names(n_rows))
        allocate(vals(n_rows, n_cols))
        allocate(vals_row(n_cols))
        ! TODO: handle max_lines
        do row = 1, n_rows
            name_size = len(names(row))
            call c_spec_file_read_real_named_array_data( &
                row, &
                names(row), name_size, &
                vals_row(1), size(vals, 2) &
            )
            names(row) = names(row)(1:name_size)
            do col = 1, n_cols
                vals(row, col) = vals_row(col)
            end do
        end do
        deallocate(vals_row)
    end subroutine

    subroutine spec_file_assert_msg(code, file, condition_ok, msg)
        integer, intent(in) :: code
        type(spec_file_t), intent(in) :: file
        logical, intent(in) :: condition_ok
        character(len=*), intent(in) :: msg
    end subroutine

    subroutine spec_file_check_line_length(file, line, length)
        type(spec_file_t), intent(in) :: file
        type(spec_line_t), intent(in) :: line
        integer, intent(in) :: length
    end subroutine

    subroutine spec_file_read_logical(file, name, var)
        type(spec_file_t), intent(inout) :: file
        character(len=*), intent(in) :: name
        logical, intent(out) :: var
        logical(c_bool) :: c_var
        call c_spec_file_read_logical(name, len(name), c_var)
        var = c_var
    end subroutine

    subroutine spec_file_read_line_no_eof(file, line)
        type(spec_file_t), intent(inout) :: file
        type(spec_line_t), intent(inout) :: line
        ! TODO!
        allocate(line%data(1))
        line%data(1) = "dist"
    end subroutine

    subroutine spec_file_read_string(file, name, var)
        type(spec_file_t), intent(inout) :: file
        character(len=*), intent(in) :: name
        character(len=*), intent(out) :: var

        integer :: var_size
        var_size = len(var)
        call c_spec_file_read_string(name, len(name), var, var_size)
        var = var(1:var_size)
    end subroutine

    subroutine spec_file_read_real(file, name, var)
        type(spec_file_t), intent(inout) :: file
        character(len=*), intent(in) :: name
        real(kind=c_double), intent(out) :: var
        call c_spec_file_read_real(name, len(name), var)
    end subroutine

    subroutine spec_file_read_integer(file, name, var)
        type(spec_file_t), intent(inout) :: file
        character(len=*), intent(in) :: name
        integer(kind=c_int), intent(out) :: var
        call c_spec_file_read_integer(name, len(name), var)
    end subroutine

    subroutine spec_file_read_line(file, line, eof)
        type(spec_file_t), intent(inout) :: file
        type(spec_line_t), intent(inout) :: line
        logical, intent(out) :: eof
        ! TODO!
        eof = .true.
    end subroutine 

    subroutine spec_file_read_timed_real_array(file, name, times, vals)
        type(spec_file_t), intent(inout) :: file
        character(len=*), intent(in) :: name
        real(kind=c_double), allocatable, intent(inout) :: times(:)
        real(kind=c_double), allocatable, intent(inout) :: vals(:)

        integer :: times_size, vals_size

        call c_spec_file_read_timed_real_array_size(&
            name, len(name), &
            times_size, vals_size&
        )
        allocate(times(times_size))
        allocate(vals(vals_size))
        call c_spec_file_read_timed_real_array_data(&
            name, len(name), &
            times(1), size(times), &
            vals(1), size(vals) &
        )
    end subroutine

    subroutine spec_file_open(filename, file)
        character(len=*), intent(in) :: filename
        type(spec_file_t), intent(out) :: file
        file%name = trim(file%name) // "::" // filename
        call c_spec_file_open(filename, index(filename, " ")-1)
    end subroutine

    subroutine spec_file_close(file)
        type(spec_file_t), intent(out) :: file
        call c_spec_file_close()
    end subroutine

    subroutine spec_file_check_name(file, name, read_name)
        type(spec_file_t), intent(inout) :: file
        character(len=*), intent(in) :: name
        character(len=*), intent(in) :: read_name
    end subroutine

    subroutine spec_file_die_msg(code, file, msg)
        integer, intent(in) :: code
        type(spec_file_t), intent(in) :: file
        character(len=*), intent(in) :: msg
    end subroutine

end module
