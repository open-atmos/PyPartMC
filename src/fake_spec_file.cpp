/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#include <string_view>
#include <tcb/span.hpp>

#include "gimmicks.hpp"

/*********************************************************************************/

void spec_file_read_string(
    const std::string_view &name,
    char* var_data,
    int *var_size
) noexcept {
    gimmick_ptr()->read_str(name, var_data, var_size);
}

extern "C"
void c_spec_file_read_string(
    const char *name_data,
    const int *name_size,
    char* var_data,
    int *var_size
) noexcept {
    spec_file_read_string(
        std::string_view(name_data, *name_size),
        var_data, var_size
    );
}

/*********************************************************************************/

void spec_file_open(const std::string_view &filename) {
    gimmick_ptr()->zoom_in(filename);
}

extern "C"
void c_spec_file_open(
    const char *filename_data,
    const int *filename_size
) {
    spec_file_open(
        std::string_view(filename_data, *filename_size)
    );
}

/*********************************************************************************/

void spec_file_close() {
    gimmick_ptr()->zoom_out();
}

extern "C"
void c_spec_file_close() {
    spec_file_close();
}

/*********************************************************************************/

void spec_file_read_timed_real_array_size(
    const std::string_view &name,
    int *times_size,
    int *vals_size
) {
    times_size[0] = gimmick_ptr()->n_elements("time");
    vals_size[0] = gimmick_ptr()->n_elements(name);
}

extern "C"
void c_spec_file_read_timed_real_array_size(
    const char *name_data,
    const int *name_size,
    int *times_size, 
    int *vals_size 
) {
    spec_file_read_timed_real_array_size(
        std::string_view(name_data, *name_size),
        times_size,
        vals_size
    );
}

/*********************************************************************************/

void spec_file_read_timed_real_array_data(
    const std::string_view &name,
    const tcb::span<double> &times,
    const tcb::span<double> &vals
) {
    gimmick_ptr()->read_arr("time", times);
    gimmick_ptr()->read_arr(name, vals);
}

extern "C"
void c_spec_file_read_timed_real_array_data(
    const char *name_data,
    const int *name_size,
    double *times_data,
    const int *times_size, 
    double *vals_data,
    const int *vals_size 
) {
    spec_file_read_timed_real_array_data(
        std::string_view(name_data, *name_size),
        tcb::span(times_data, *times_size),
        tcb::span(vals_data, *vals_size)
    );
}

/*********************************************************************************/

void spec_file_read_real_named_array() {
}

extern "C"
void c_spec_file_read_real_named_array(
    const int *max_lines,
    char *names_data,
    const int *names_size,
    double *vals_data,
    const int *vals_size
) {
    spec_file_read_real_named_array();
}

