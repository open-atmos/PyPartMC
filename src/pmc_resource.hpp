/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include <iostream>

class PMCResource {
    void *ptr;
    void (*f_ctor)(void*);
    void (*f_dtor)(void*);

    PMCResource(const PMCResource&) = delete;
    PMCResource& operator= (const PMCResource&) = delete;

  public:
    PMCResource(
        decltype(f_ctor) f_ctor,
        decltype(f_dtor) f_dtor
    ) : 
        f_ctor(f_ctor),
        f_dtor(f_dtor) 
    {
        this->f_ctor(&this->ptr);
    }

    ~PMCResource() {
        this->f_dtor(&this->ptr);
    }

    const void *f_arg() const {
        return &this->ptr;
    }
};

