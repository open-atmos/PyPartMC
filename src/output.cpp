/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022-2025 University of Illinois Urbana-Champaign                                  #
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

std::tuple<std::shared_ptr<AeroData>, AeroState*, std::shared_ptr<GasData>,
     GasState*, EnvState*> input_state(
    const std::string &name
){
    int index;
    double time;
    double del_t;
    int i_repeat;
    const int name_size = name.size();

    AeroState *aero_state = new AeroState(std::shared_ptr<AeroData>(new AeroData()));
    GasState *gas_state = new GasState(std::shared_ptr<GasData>(new GasData()));
    EnvState *env_state = new EnvState();
    f_input_state(name.c_str(), &name_size, &index, &time, &del_t, &i_repeat,
       aero_state->aero_data->ptr.f_arg_non_const(), aero_state->ptr.f_arg_non_const(),
       gas_state->gas_data->ptr.f_arg_non_const(), gas_state->ptr.f_arg_non_const(),
       env_state->ptr.f_arg_non_const());

    return std::make_tuple(aero_state->aero_data, aero_state, gas_state->gas_data,
       gas_state, env_state);
}


std::tuple<std::shared_ptr<AeroData>, BinGrid*, AeroBinned*, std::shared_ptr<GasData>,
     GasState*, EnvState*> input_sectional(
    const std::string &name
){
    int index;
    double time;
    double del_t;
    const int name_size = name.size();

    AeroBinned *aero_binned = new AeroBinned(std::shared_ptr<AeroData>(new AeroData()));
    BinGrid *bin_grid = new BinGrid();
    GasState *gas_state = new GasState(std::shared_ptr<GasData>(new GasData()));
    EnvState *env_state = new EnvState();
    f_input_sectional(name.c_str(), &name_size, &index, &time, &del_t, bin_grid->ptr.f_arg_non_const(),
       aero_binned->aero_data->ptr.f_arg_non_const(), aero_binned->ptr.f_arg_non_const(),
       gas_state->gas_data->ptr.f_arg_non_const(), gas_state->ptr.f_arg_non_const(),
       env_state->ptr.f_arg_non_const());

    return std::make_tuple(aero_binned->aero_data, bin_grid, aero_binned, gas_state->gas_data,
       gas_state, env_state);
}

std::tuple<std::shared_ptr<AeroData>, BinGrid*, AeroBinned*, std::shared_ptr<GasData>,
     GasState*, EnvState*> input_exact(
    const std::string &name
){
    int index;
    double time;
    double del_t;
    const int name_size = name.size();

    AeroBinned *aero_binned = new AeroBinned(std::shared_ptr<AeroData>(new AeroData()));
    BinGrid *bin_grid = new BinGrid();
    GasState *gas_state = new GasState(std::shared_ptr<GasData>(new GasData()));
    EnvState *env_state = new EnvState();
    f_input_exact(name.c_str(), &name_size, &index, &time, &del_t, bin_grid->ptr.f_arg_non_const(),
       aero_binned->aero_data->ptr.f_arg_non_const(), aero_binned->ptr.f_arg_non_const(),
       gas_state->gas_data->ptr.f_arg_non_const(), gas_state->ptr.f_arg_non_const(),
       env_state->ptr.f_arg_non_const());

    return std::make_tuple(aero_binned->aero_data, bin_grid, aero_binned, gas_state->gas_data,
       gas_state, env_state);
}
