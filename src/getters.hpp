/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include <utility>
#include <valarray>

namespace pypartmc { // possible namespace to avoid name conflicts

template <typename T = double, typename SelfType, typename Func>
inline T get_value(const SelfType &self, Func func) { // inline to keep it header-only
    T value{};
    func(self.ptr.f_arg(), &value);
    return value;
}

template <typename T = double, typename SelfType, typename Func>
inline void set_value(SelfType &self, Func func, T value) {
    func(self.ptr.f_arg_non_const(), &value);
}

template <typename T = double, typename SelfType, typename Func, typename... Args>
inline T get_derived_value(const SelfType &self, Func func, Args&&... args) {
    T value{};
    func(self.ptr.f_arg(), std::forward<Args>(args)..., &value);
    return value;
}

template<typename T = double, typename SelfType, typename Func, typename LenF, typename... ExtraArgs>
static auto get_array_values(const SelfType &self, Func f, LenF len_fn,
    ExtraArgs&&... extra) {
        int len;
        len_fn(self.ptr.f_arg(), &len);
        std::valarray<T> arr(len);
        f(self.ptr.f_arg(), std::forward<ExtraArgs>(extra)..., begin(arr), &len);
        return arr;
    }
}
