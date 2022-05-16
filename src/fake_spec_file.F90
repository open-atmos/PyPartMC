module pmc_spec_file
    use pmc_spec_line
    implicit none

    integer, parameter :: SPEC_FILE_MAX_LIST_LINES = 1000

    type spec_file_t
        character(len=SPEC_LINE_MAX_VAR_LEN) :: name
    end type

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
        real(kind=dp), allocatable :: vals(:,:)
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
    end subroutine

    subroutine spec_file_read_real(file, name, var)
        type(spec_file_t), intent(inout) :: file
        character(len=*), intent(in) :: name
        real(kind=dp), intent(out) :: var
    end subroutine

    subroutine spec_file_read_line(file, line, eof)
        type(spec_file_t), intent(inout) :: file
        type(spec_line_t), intent(inout) :: line
        logical, intent(out) :: eof
    end subroutine 

    subroutine spec_file_read_timed_real_array(file, name, times, vals)
        type(spec_file_t), intent(inout) :: file
        character(len=*), intent(in) :: name
        real(kind=dp), allocatable :: times(:)
        real(kind=dp), allocatable :: vals(:)
    end subroutine

    subroutine spec_file_open(filename, file)
        character(len=*), intent(in) :: filename
        type(spec_file_t), intent(out) :: file
    end subroutine

    subroutine spec_file_close(file)
        type(spec_file_t), intent(out) :: file
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
