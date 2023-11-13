/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "pybind11/pybind11.h"
#include "nlohmann/json.hpp"
#include "pybind11_json/pybind11_json.hpp"
#include "pybind11/complex.h"

#include "util.hpp"
#include "rand.hpp"
#include "run_part.hpp"
#include "run_part_opt.hpp"
#include "aero_data.hpp"
#include "aero_dist.hpp"
#include "aero_mode.hpp"
#include "aero_state.hpp"
#include "env_state.hpp"
#include "gas_data.hpp"
#include "gas_state.hpp"
#include "condense.hpp"
#include "bin_grid.hpp"
#include "camp_core.hpp"
#include "photolysis.hpp"
#include "output.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

namespace PYBIND11_NAMESPACE { namespace detail {
    template <typename T>
    struct type_caster<tl::optional<T>> : optional_caster<tl::optional<T>> {};
}}

PYBIND11_MODULE(_PyPartMC, m) {
    m.doc() = R"pbdoc(
        PyPartMC is a Python interface to PartMC.
    )pbdoc";

    m.def("run_part", &run_part, "Do a particle-resolved Monte Carlo simulation.");
    m.def("run_part_timestep", &run_part_timestep, "Do a single time step");
    m.def("run_part_timeblock", &run_part_timeblock, "Do a time block");
    m.def("condense_equilib_particles", &condense_equilib_particles, R"pbdoc(
      Call condense_equilib_particle() on each particle in the aerosol
      to ensure that every particle has its water content in
      equilibrium.
    )pbdoc");
    m.def("condense_equilib_particle", &condense_equilib_particle, R"pbdoc(
        Determine the water equilibrium state of a single particle.
    )pbdoc");

    // TODO #65
    //m.def("run_sect", &run_sect, "Do a 1D sectional simulation (Bott 1998 scheme).");
    //m.def("run_exact", &run_exact, "Do an exact solution simulation.");

    py::class_<AeroData, std::shared_ptr<AeroData>>(m, "AeroData",
        R"pbdoc(
             Aerosol material properties and associated data.

             The data in this structure is constant, as it represents physical
             quantities that cannot change over time.

             Each aerosol species is identified by an index <tt>i =
             1,...,aero_data_n_spec(aero_data)</tt>. Then \c name(i) is the name of
             that species, \c density(i) is its density, etc. The ordering of the
             species is arbitrary and should not be relied upon (currently it is the
             order in the species data file). The only exception is that it is
             possible to find out which species is water from the \c i_water
             variable.

             The names of the aerosol species are not important to PartMC, as
             only the material properties are used. The names are used for
             input and output, and also for communication with MOSAIC. For the
             MOSAIC interface to work correctly the species must be named the
             same, but without the \c _a suffix.
        )pbdoc"
    )
        .def(py::init<const nlohmann::json&>())
        .def("spec_by_name", AeroData::spec_by_name)
        .def("__len__", AeroData::__len__)
        .def_property_readonly("n_source", AeroData::n_source)
        .def_property("frac_dim", &AeroData::get_frac_dim, &AeroData::set_frac_dim)
        .def_property("vol_fill_factor", &AeroData::get_vol_fill_factor, &AeroData::set_vol_fill_factor)
        .def_property("prime_radius", &AeroData::get_prime_radius, &AeroData::set_prime_radius)
        .def_property_readonly("densities", &AeroData::densities,
            "Return array of aerosol species densities")
        .def("density", &AeroData::density, "Return density of an aerosol species")
        .def("rad2vol", AeroData::rad2vol,
            "Convert geometric radius (m) to mass-equivalent volume (m^3).")
        .def("vol2rad", AeroData::vol2rad,
            "Convert mass-equivalent volume (m^3) to geometric radius (m)")
        .def("diam2vol", AeroData::diam2vol,
            "Convert geometric diameter (m) to mass-equivalent volume (m^3).")
        .def("vol2diam", AeroData::vol2diam,
            "Convert mass-equivalent volume (m^3) to geometric diameter (m).")
    ;

    py::class_<AeroParticle>(m, "AeroParticle",
        R"pbdoc(
             Single aerosol particle data structure.

             The \c vol array stores the total volumes of the different
             species that make up the particle. This array must have length
             equal to aero_data%%n_spec, so that \c vol(i) is the volume (in
             m^3) of the i'th aerosol species.
        )pbdoc"
    )
        .def(py::init<std::shared_ptr<AeroData>, const std::valarray<double>&>())
        .def_property_readonly("volumes", AeroParticle::volumes)
        .def_property_readonly("volume", AeroParticle::volume,
            "Total volume of the particle (m^3).")
        .def("species_volume",
            py::overload_cast<const AeroParticle &, const int &>(&AeroParticle::species_volume),
            "Volume of a single species in the particle (m^3).")
        .def("species_volume",
            py::overload_cast<const AeroParticle &, const std::string &>(&AeroParticle::species_volume_by_name),
            "Volume of a single species in the particle (m^3).")
        .def_property_readonly("dry_volume", AeroParticle::dry_volume,
            "Total dry volume of the particle (m^3).")
        .def_property_readonly("radius", AeroParticle::radius,
            "Total radius of the particle (m).")
        .def_property_readonly("dry_radius", AeroParticle::dry_radius,
            "Total dry radius of the particle (m).")
        .def_property_readonly("diameter", AeroParticle::diameter,
            "Total diameter of the particle (m).")
        .def_property_readonly("dry_diameter", AeroParticle::dry_diameter,
            "Total dry diameter of the particle (m).")
        .def_property_readonly("mass", AeroParticle::mass,
            "Total mass of the particle (kg).")
        .def("species_mass", py::overload_cast<const AeroParticle &, const int &>(&AeroParticle::species_mass),
            "Mass of a single species in the particle (kg).")
        .def("species_mass", py::overload_cast<const AeroParticle &, const std::string &>(&AeroParticle::species_mass_by_name),
            "Mass of a single species in the particle (kg).")
        .def_property_readonly("species_masses", AeroParticle::species_masses,
            "Mass of all species in the particle (kg).")
        .def_property_readonly("solute_kappa", AeroParticle::solute_kappa,
            "Returns the average of the solute kappas (1).")
        .def_property_readonly("moles", AeroParticle::moles,
            "Total moles in the particle (1).")
        .def_property_readonly("absorb_cross_sect", AeroParticle::absorb_cross_sect,
            "Absorption cross-section (m^-2).")
        .def_property_readonly("scatter_cross_sect", AeroParticle::scatter_cross_sect,
            "Scattering cross-section (m^-2).")
        .def_property_readonly("asymmetry", AeroParticle::asymmetry,
            "Asymmetry parameter (1).")
        .def_property_readonly("refract_shell", AeroParticle::refract_shell,
            "Refractive index of the shell (1).")
        .def_property_readonly("refract_core", AeroParticle::refract_core,
            "Refractive index of the core (1).")
        .def_property_readonly("n_orig_part", AeroParticle::n_orig_part,
            "Number of original particles from each source that coagulated to form particle.")
        .def_property_readonly("least_create_time", AeroParticle::least_create_time,
            "First time a constituent was created (s).")
        .def_property_readonly("greatest_create_time", AeroParticle::greatest_create_time,
            "Last time a constituent was created (s).")
        .def_property_readonly("id", AeroParticle::id, "Unique ID number.")
        .def("mobility_diameter", AeroParticle::mobility_diameter,
            "Mobility diameter of the particle (m).")
        .def_property_readonly("density", AeroParticle::density,
            "Average density of the particle (kg/m^3)")
        .def("approx_crit_rel_humid", AeroParticle::approx_crit_rel_humid,
            "Returns the approximate critical relative humidity (1).")
        .def("crit_rel_humid", AeroParticle::crit_rel_humid,
            "Returns the critical relative humidity (1).")
        .def("crit_diameter", AeroParticle::crit_diameter,
            "Returns the critical diameter (m).")
        .def("coagulate", AeroParticle::coagulate,
            "Coagulate two particles together to make a new one. The new particle will not have its ID set.")
        .def("zero", AeroParticle::zero,
            "Resets an aero_particle to be zero.")
        .def("set_vols", AeroParticle::set_vols,
            "Sets the aerosol particle volumes.")
    ;

    py::class_<AeroState>(m, "AeroState",
        R"pbdoc(
             The current collection of aerosol particles.

             The particles in \c aero_state_t are stored in a single flat
             array (the \c apa data member), with a sorting into size bins and
             weight groups/classes possibly stored in the \c aero_sorted data
             member (if \c valid_sort is true).

             Every time we remove particles we keep track of the particle ID
             and the action performed in the aero_info_array_t structure. This
             is typically cleared each time we output data to disk.
        )pbdoc"
    )
        .def(py::init<std::shared_ptr<AeroData>, const double, const std::string>())
        .def("__len__", AeroState::__len__)
        .def_property_readonly("total_num_conc", AeroState::total_num_conc,
            "returns the total number concentration of the population")
        .def_property_readonly("total_mass_conc", AeroState::total_mass_conc,
            "returns the total mass concentration of the population")
        .def_property_readonly("num_concs", AeroState::num_concs,
            "returns the number concentration of each particle in the population")
        .def("masses", AeroState::masses,
            "returns the total mass of each particle in the population",
            py::arg("include") = py::none(), py::arg("exclude") = py::none())
        .def("volumes", AeroState::volumes,
            "returns the volume of each particle in the population",
             py::arg("include") = py::none(), py::arg("exclude") = py::none())
        .def_property_readonly("dry_diameters", AeroState::dry_diameters,
            "returns the dry diameter of each particle in the population")
        .def("diameters", AeroState::diameters,
            "returns the diameter of each particle in the population",
            py::arg("include") = py::none(), py::arg("exclude") = py::none())
        .def("crit_rel_humids", AeroState::crit_rel_humids,
            "returns the critical relative humidity of each particle in the population")
        .def("mixing_state", AeroState::mixing_state,
            "returns the mixing state parameters (chi,d_alpha,d_gamma) of the population",
            py::arg("include") = py::none(), py::arg("exclude") = py::none(),
            py::arg("group") = py::none())
        .def("bin_average_comp", AeroState::bin_average_comp,
            "composition-averages population using BinGrid")
        .def("particle", AeroState::get_particle,
            "returns the particle of a given index")
        .def("rand_particle", AeroState::get_random_particle,
            "returns a random particle from the population")
        .def("dist_sample", AeroState::dist_sample,
            "sample particles for AeroState from an AeroDist",
            py::arg("AeroDist"), py::arg("sample_prop") = 1.0, py::arg("create_time") = 0.0,
            py::arg("allow_doubling") = true, py::arg("allow_halving") = true)
    ;

    py::class_<GasData, std::shared_ptr<GasData>>(m, "GasData",
        R"pbdoc(
            Constant gas data.

            Each gas species is identified by an integer \c i between 1 and
            \c gas_data_n_spec(gas_data). Species \c i has name \c gas_data%%name(i).
            The variable gas data describing the current mixing ratios is stored
            in the gas_state_t structure, so the mixing ratio of species \c i
            is gas_state%%mix_rat(i).
        )pbdoc"
    )
        .def(py::init<const py::tuple&>())
        .def("__len__", GasData::__len__)
        .def_property_readonly("n_spec", GasData::__len__)
        .def("__str__", GasData::__str__,
            "returns a string with JSON representation of the object")
        .def("spec_by_name", GasData::spec_by_name,
            "returns the number of the species in gas with the given name")
    ;

    py::class_<EnvState>(m,
        "EnvState",
        R"pbdoc(
            Current environment state.

            All quantities are instantaneous, describing the state at a
            particular instant of time. Constant data and other data not
            associated with the current environment state is stored in
            scenario_t.
        )pbdoc"
    )
        .def(py::init<const nlohmann::json&>())
        .def("set_temperature", EnvState::set_temperature,
            "sets the temperature of the environment state")
        .def_property_readonly("temp", EnvState::temp,
            "returns the current temperature of the environment state")
        .def_property_readonly("rh", EnvState::rh,
            "returns the current relative humidity of the environment state")
        .def_property_readonly("elapsed_time", EnvState::get_elapsed_time,
            "returns time since start_time (s).")
        .def_property_readonly("start_time", EnvState::get_start_time,
            "returns start time (s since 00:00 UTC on start_day)")
        .def_property("height", &EnvState::get_height, &EnvState::set_height)
        .def_property("pressure", &EnvState::get_pressure, &EnvState::set_pressure)
    ;

    py::class_<Photolysis>(m,
        "Photolysis",
        R"pbdoc(
            PartMC interface to a photolysis module
        )pbdoc"
    )
        .def(py::init<>())
    ;

    py::class_<CampCore>(m,
        "CampCore",
        R"pbdoc(
            An interface between PartMC and the CAMP
        )pbdoc"
    )
        .def(py::init<>())
    ;

    py::class_<Scenario>(m,
        "Scenario",
        R"pbdoc(
            This is everything needed to drive the scenario being simulated.

            The temperature, pressure, emissions and background states are profiles
            prescribed as functions of time by giving a number of times and
            the corresponding data. Simple data such as temperature and pressure is
            linearly interpolated between times, with constant interpolation
            outside of the range of times. Gases and aerosols are
            interpolated with gas_state_interp_1d() and
            aero_dist_interp_1d(), respectively.
        )pbdoc"
    )
        .def(
            py::init<
                const GasData&,
                const AeroData&,
                const nlohmann::json&
            >(),
            "instantiates and initializes from a JSON object"
        )
        .def("__str__", Scenario::__str__,
            "returns a string with JSON representation of the object")
        .def("init_env_state", Scenario::init_env_state,
            "initializes the EnvState")
        .def("aero_emissions", Scenario::get_dist,
            "returns aero_emissions AeroDists at a given index")
        .def_property_readonly("aero_emissions_n_times", Scenario::get_emissions_n_times,
            "returns the number of times specified for emissions")
        .def_property_readonly("aero_emissions_rate_scale", Scenario::emission_rate_scale)
        .def_property_readonly("aero_emissions_time", Scenario::emission_time)
        .def("aero_background", Scenario::get_aero_background_dist,
            "returns aero_background AeroDists at a given index")
        .def_property_readonly("aero_dilution_n_times", Scenario::get_aero_dilution_n_times,
            "returns the number of times specified for dilution")
        .def_property_readonly("aero_dilution_rate", Scenario::aero_dilution_rate)
        .def_property_readonly("aero_dilution_time", Scenario::aero_dilution_time)

    ;

    py::class_<GasState>(m,
        "GasState",
        R"pbdoc(
            Current state of the gas mixing ratios in the system.

            The gas species are defined by the gas_data_t structure, so that
            \c gas_state%%mix_rat(i) is the current mixing ratio of the gas
            with name \c gas_data%%name(i), etc.

            By convention, if gas_state_is_allocated() return \c .false.,
            then the gas_state is treated as zero for all operations on
            it. This will be the case for new \c gas_state_t structures.
        )pbdoc"
    )
        .def(py::init<std::shared_ptr<GasData>>(),
            "instantiates and initializes based on GasData")
        .def("__setitem__", GasState::set_item)
        //.def("__setitem__", GasState::set_items)
        .def("__getitem__", GasState::get_item)
        //.def("__getitem__", GasState::get_items)
        .def("__len__", GasState::__len__)
        .def_property_readonly("n_spec", GasState::__len__)
        .def("__str__", GasState::__str__,
            "returns a string with JSON representation of the object")
        .def("set_size", GasState::set_size,
            "sets the GasState to the size of GasData")
        .def("mix_rat", GasState::mix_rat,
            "returns the mixing ratio of a gas species")
        .def_property("mix_rats", &GasState::mix_rats, &GasState::set_mix_rats,
            "provides access (read of write) to the array of mixing ratios")
    ;

    py::class_<RunPartOpt>(m,
        "RunPartOpt",
        "Options controlling the execution of run_part()."
    )
        .def(py::init<const nlohmann::json&>())
        .def_property_readonly("t_max", RunPartOpt::t_max, "total simulation time")
        .def_property_readonly("del_t", RunPartOpt::del_t, "time step")
    ;

    py::class_<BinGrid>(m,"BinGrid")
        .def(py::init<const double, const py::str, const double, const double>())
        .def("__len__", BinGrid::__len__)
        .def_property_readonly("edges", BinGrid::edges, "Bin edges")
        .def_property_readonly("centers", BinGrid::centers, "Bin centers")
    ;

    py::class_<AeroMode>(m,"AeroMode")
        .def(py::init<AeroData&, const nlohmann::json&>())
        .def_property("num_conc", &AeroMode::get_num_conc, &AeroMode::set_num_conc,
             "provides access (read or write) to the total number concentration of a mode")
        .def("num_dist", &AeroMode::num_dist,
             "returns the binned number concenration of a mode")
        .def_property("vol_frac", &AeroMode::get_vol_frac,
             &AeroMode::set_vol_frac)
        .def_property("vol_frac_std", &AeroMode::get_vol_frac_std,
             &AeroMode::set_vol_frac_std)
        .def_property("char_radius", &AeroMode::get_char_radius,
             &AeroMode::set_char_radius)
        .def_property("gsd", &AeroMode::get_gsd,
             &AeroMode::set_gsd)
        .def("set_sample", &AeroMode::set_sampled)
        .def_property("type", &AeroMode::get_type, &AeroMode::set_type)
        .def_property("name", &AeroMode::get_name, &AeroMode::set_name)
    ;

    py::class_<AeroDist>(m,"AeroDist")
        .def(py::init<std::shared_ptr<AeroData>, const nlohmann::json&>())
        .def_property_readonly("n_mode", &AeroDist::get_n_mode)
        .def_property_readonly("num_conc", &AeroDist::get_total_num_conc)
        .def("mode", AeroDist::get_mode,
            "returns the mode of a given index")
    ;

    m.def(
        "histogram_1d", &histogram_1d, py::return_value_policy::copy,
        "Return a 1D histogram with of the given weighted data, scaled by the bin sizes."
    );

    m.def(
        "histogram_2d", &histogram_2d, py::return_value_policy::copy,
        "Return a 2D histogram with of the given weighted data, scaled by the bin sizes."
    );

    //  TODO #120: auto util = m.def_submodule("util", "...");
    m.def(
        "pow2_above", &pow2_above, py::return_value_policy::copy,
        "Return the least power-of-2 that is at least equal to n."
    );

    m.def(
        "sphere_vol2rad", &sphere_vol2rad, py::return_value_policy::copy,
        "Convert mass-equivalent volume (m^3) to geometric radius (m) for spherical particles."
    );

    m.def(
        "rad2diam", &rad2diam, py::return_value_policy::copy,
        "Convert radius (m) to diameter (m)."
    );

    m.def(
        "sphere_rad2vol", &sphere_rad2vol, py::return_value_policy::copy,
        "Convert geometric radius (m) to mass-equivalent volume for spherical particles."
    );

    m.def(
        "diam2rad", &diam2rad, py::return_value_policy::copy,
        "Convert diameter (m) to radius (m)."
    );

    m.def(
        "loss_rate_dry_dep", &loss_rate_dry_dep, py::return_value_policy::copy,
        "Compute and return the dry deposition rate for a given particle."
    );

    m.def(
        "loss_rate", &loss_rate, py::return_value_policy::copy,
        "Evaluate a loss rate function."
    );

    m.def(
        "output_state", &output_state, "Output current state to netCDF file."
    );

    m.def(
        "input_state", &input_state, "Read current state from netCDF output file."
    );

    m.def(
        "rand_init", &rand_init, "Initializes the random number generator to the state defined by the given seed. If the seed is 0 then a seed is auto-generated from the current time"
    );

    m.def(
        "rand_normal", &rand_normal, "Generates a normally distributed random number with the given mean and standard deviation"
    );

    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);

    m.attr("__all__") = py::make_tuple(
        "__version__",
        "AeroData",
        "AeroDist",
        "AeroMode",
        "AeroState",
        "AeroParticle",
        "BinGrid",
        "CampCore",
        "EnvState",
        "GasData",
        "GasState",
        "Photolysis",
        "RunPartOpt",
        "Scenario",
        "condense_equilib_particles",
        "run_part",
        "run_part_timeblock",
        "run_part_timestep",
        "pow2_above",
        "condense_equilib_particle",
        "histogram_1d",
        "histogram_2d",
        "sphere_vol2rad",
        "rad2diam",
        "sphere_rad2vol",
        "diam2rad",
        "loss_rate_dry_dep",
        "loss_rate",
        "output_state",
        "input_state",
        "rand_init",
        "rand_normal"
    );
}
