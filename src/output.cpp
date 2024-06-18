/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "output.hpp"

void output_state(
    const std::string &prefix,
    const AeroData &aero_data,
    const AeroState &aero_state,
    const GasData &gas_data,
    const GasState &gas_state,
    const EnvState &env_state
){
    int index;
    double time;
    double del_t;
    int i_repeat;
    bool record_removals;
    bool record_optical;
    const int prefix_size = prefix.size();

    index = 1;
    time = 1.0;
    del_t = 60.0;
    i_repeat = 1;
    record_removals = false;
    record_optical = false;

    f_output_state(prefix.c_str(), &prefix_size, aero_data.ptr.f_arg(),
       aero_state.ptr.f_arg(), gas_state.gas_data->ptr.f_arg(),
       gas_state.ptr.f_arg(), env_state.ptr.f_arg(), &index, &time, &del_t,
       &i_repeat, &record_removals, &record_optical);
}

std::tuple<AeroData*, GasData*> input_state(
    const std::string &name,
    const AeroData &aero_data,
    const AeroState &aero_state,
    const GasData &gas_data,
    const GasState &gas_state,
    const EnvState &env_state
){
    int index;
    double time;
    double del_t;
    int i_repeat;
    const int name_size = name.size();

    f_input_state(name.c_str(), &name_size, &index, &time, &del_t, &i_repeat,
       aero_data.ptr.f_arg(), aero_state.ptr.f_arg(),
       gas_state.gas_data->ptr.f_arg(), gas_state.ptr.f_arg(), env_state.ptr.f_arg());


    // Made in C++ rather than passed in
    AeroData *aero_data_new = new AeroData();
    AeroState *aero_state_new = new AeroState(std::shared_ptr<AeroData>(aero_data_new));
    GasData *gas_data_new = new GasData();
    GasState *gas_state_new = new GasState(std::shared_ptr<GasData>(gas_data_new));
    f_input_state(name.c_str(), &name_size, &index, &time, &del_t, &i_repeat,
       aero_data_new->ptr.f_arg(), aero_state_new->ptr.f_arg(),
       gas_data_new->ptr.f_arg(), gas_state_new->ptr.f_arg(), env_state.ptr.f_arg());

   int n_source;
   int len_gas_data;
   int len_gas_state;
   int len_aero_state;

   // Quick tests to see if everything works
   f_aero_data_n_source(aero_data_new->ptr.f_arg(), &n_source);
   f_gas_data_len(gas_data_new->ptr.f_arg(), &len_gas_data);
   f_gas_state_len(gas_state_new->ptr.f_arg(), &len_gas_state);
   f_aero_state_len(aero_state_new->ptr.f_arg(), &len_aero_state);

   return std::make_tuple(aero_data_new, gas_data_new);
}
