module pmc_spec_file
    use pmc_spec_line
    implicit none

    integer, parameter :: SPEC_FILE_MAX_LIST_LINES = 1000

    type spec_file_t
        character(len=SPEC_LINE_MAX_VAR_LEN) :: name
    end type

    interface
        subroutine c_spec_file_read_string(name_data, name_size, var_data, var_size) bind(C)
            character, intent(in) :: name_data
            character, intent(out) :: var_data
            integer, intent(in) :: name_size, var_size
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

        subroutine c_spec_file_read_real_named_array( &
            max_lines, &
            names_data, names_size, &
            vals_data, vals_size &
        ) bind(C)
            import c_double
            character, intent(in) :: names_data
            real(c_double), intent(out) :: vals_data
            integer, intent(in) :: vals_size, names_size, max_lines
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
        real(kind=c_double), allocatable :: vals(:,:)

allocate(names(1))  ! TODO!
        call c_spec_file_read_real_named_array( &
            max_lines, &
            names(1), size(names), &
            vals(1,1), size(vals) &
        )
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
    end subroutine

    subroutine spec_file_read_line_no_eof(file, line)
        type(spec_file_t), intent(inout) :: file
        type(spec_line_t), intent(inout) :: line
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
    end subroutine

    subroutine spec_file_read_line(file, line, eof)
        type(spec_file_t), intent(inout) :: file
        type(spec_line_t), intent(inout) :: line
        logical, intent(out) :: eof
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

    subroutine spec_file_read_integer(file, name, var)
        type(spec_file_t), intent(inout) :: file
        character(len=*), intent(in) :: name
        integer, intent(out) :: var
    end subroutine

end module
