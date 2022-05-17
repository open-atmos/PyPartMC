/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#include <string_view>

#include "gimmicks.hpp"

void spec_file_read_string(
    const std::string_view &name,
    std::string_view &var
) {
    gimmick_ptr()->read_str(name, var);
}

extern "C"
void c_spec_file_read_string(
    const char *name_data,
    const int *name_size,
    char* var_data,
    const int *var_size
) {
    auto var = std::string_view(var_data, *var_size);
    spec_file_read_string(
        std::string_view(name_data, *name_size),
        var
    );
}
