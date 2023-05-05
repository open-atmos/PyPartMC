/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include <bpstd/string_view.hpp>
#include <tcb/span.hpp>

#include "gimmicks.hpp"


/*********************************************************************************/

extern "C"
void c_spec_file_read_real(
    const char *name_data, const int *name_size, double *var
) noexcept {
    gimmick_ptr()->read_value(bpstd::string_view(name_data, *name_size), var);
}

/*********************************************************************************/

extern "C"
void c_spec_file_read_integer(
    const char *name_data, const int *name_size, int *var
) noexcept {
    gimmick_ptr()->read_value(bpstd::string_view(name_data, *name_size), var);
}

/*********************************************************************************/

extern "C"
void c_spec_file_read_logical(
    const char *name_data, const int *name_size, bool *var
) noexcept {
    gimmick_ptr()->read_value(bpstd::string_view(name_data, *name_size), var);
}

/*********************************************************************************/

void spec_file_read_string(
    const bpstd::string_view &name,
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
        bpstd::string_view(name_data, *name_size),
        var_data, var_size
    );
}

/*********************************************************************************/

void spec_file_open(const bpstd::string_view &filename) noexcept {
    gimmick_ptr()->zoom_in(filename);
}

extern "C"
void c_spec_file_open(
    const char *filename_data,
    const int *filename_size
) noexcept {
    spec_file_open(
        bpstd::string_view(filename_data, *filename_size)
    );
}

/*********************************************************************************/

void spec_file_close() noexcept {
    gimmick_ptr()->zoom_out();
}

extern "C"
void c_spec_file_close() noexcept {
    spec_file_close();
}

/*********************************************************************************/

void spec_file_read_timed_real_array_size(
    const bpstd::string_view &name,
    int *times_size,
    int *vals_size
) noexcept {
    times_size[0] = gimmick_ptr()->n_elements("time");
    vals_size[0] = gimmick_ptr()->n_elements(name);
}

extern "C"
void c_spec_file_read_timed_real_array_size(
    const char *name_data,
    const int *name_size,
    int *times_size, 
    int *vals_size 
) noexcept {
    spec_file_read_timed_real_array_size(
        bpstd::string_view(name_data, *name_size),
        times_size,
        vals_size
    );
}

/*********************************************************************************/

void spec_file_read_timed_real_array_data(
    const bpstd::string_view &name,
    const tcb::span<double> &times,
    const tcb::span<double> &vals
) noexcept {
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
) noexcept {
    spec_file_read_timed_real_array_data(
        bpstd::string_view(name_data, *name_size),
        tcb::span<double>(times_data, *times_size),
        tcb::span<double>(vals_data, *vals_size)
    );
}

/*********************************************************************************/

void spec_file_read_real_named_array_size(
    int *n_rows,
    int *n_cols
) noexcept {
    auto first_field = gimmick_ptr()->first_field_name();
    *n_rows = gimmick_ptr()->n_numeric_array_entries();
    *n_cols = gimmick_ptr()->n_elements(first_field);
    // TODO #112: check each line has the same number of elements as time
}

extern "C"
void c_spec_file_read_real_named_array_size(
    int *n_rows,
    int *n_cols
) noexcept {
    spec_file_read_real_named_array_size(n_rows, n_cols);
}

/*********************************************************************************/

void spec_file_read_real_named_array_data(
    const unsigned int row,
    char *name_data,
    int *name_size,
    const tcb::span<double> &vals
) noexcept {
    auto i = 0u, n_numeric_array_entries = gimmick_ptr()->n_numeric_array_entries();
    for (
        auto it = gimmick_ptr()->begin();
        i < n_numeric_array_entries;
        ++i, ++it
    ) {
        assert(it->is_object());
        if (i == row-1) {
            assert(it->size() == 1);
            for (auto &entry : it->items()) {
                // TODO #112: use input name_size as limit param
                for (auto c=0u; c < entry.key().size(); ++c)
                    name_data[c] = entry.key()[c];
                *name_size = entry.key().size();
                for (auto idx=0u; idx < entry.value().size(); ++idx) {
                    vals[idx] = entry.value().at(idx).get<double>();
                }
                break; // TODO #112
            }
        }
    }
}

extern "C"
void c_spec_file_read_real_named_array_data(
    const int *row,
    char *name_data,
    int *name_size,
    double *vals_data,
    const int *vals_size
) noexcept {
    spec_file_read_real_named_array_data(
        *row,
        name_data, name_size,
        tcb::span<double>(vals_data, *vals_size)
    );
}

/*********************************************************************************/
extern "C"
void c_spec_file_read_line_data_size(int *size) noexcept {
    *size = gimmick_ptr()->read_line_data_size_and_start_enumerating();
}

/*********************************************************************************/
extern "C"
void c_spec_file_read_line(
    char *name_data,
    int *name_size,
    char *data0_data,
    int *data0_size,
    bool *eof
) noexcept {
    std::string name, data;
    *eof = gimmick_ptr()->read_line(name, data);

    {
        int i = 0;
        for (const auto &ch : name) {
            assert(i < *name_size);
            name_data[i++] = ch;
        }
        *name_size = i;
    }
    {
        int i = 0;
        for (const auto &ch : data) {
            assert(i < *data0_size);
            data0_data[i++] = ch;
        }
        *data0_size = i;
    }
}
