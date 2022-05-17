/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once

extern "C" void f_run_part_opt_ctor(void *ptr);
extern "C" void f_run_part_opt_dtor(void *ptr);

struct RunPartOpt {
    void *ptr;

    RunPartOpt() {
        f_run_part_opt_ctor(&this->ptr); 
    }

    ~RunPartOpt() {
        f_run_part_opt_dtor(&this->ptr);
  }
};

