/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "json_resource.hpp"

template <class X>
X& singleton()
{
    static X x;
    return x;
}

std::unique_ptr<JSONResource> &json_resource_ptr() {
    return singleton<std::unique_ptr<JSONResource>>();
}

