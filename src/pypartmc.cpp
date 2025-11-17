/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022-2025 University of Illinois Urbana-Champaign                                  #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#include "nanobind/nanobind.h"
#include "nanobind/stl/complex.h"
#include "nanobind/stl/vector.h"
#include "nanobind/stl/string.h"
#include "nanobind/stl/shared_ptr.h"
#include "nanobind/stl/tuple.h"
#include "nanobind/stl/detail/nb_optional.h"
#include "nanobind/ndarray.h"
#undef snprintf // required to fix an issue with std::snprintf in nlohmann::json
#include "nlohmann/json.hpp"
#include "nanobind_json/nanobind_json.h"
#include "sundials/sundials_config.h"
#include "camp/version.h"
#include "tl/optional.hpp"

#include "util.hpp"
#include "rand.hpp"
#include "run_part.hpp"
#include "run_part_opt.hpp"
#include "run_sect.hpp"
#include "run_sect_opt.hpp"
#include "run_exact.hpp"
#include "run_exact_opt.hpp"
#include "aero_binned.hpp"
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
#include "output_parameters.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace nb = nanobind;
namespace nl = nlohmann;

NAMESPACE_BEGIN(nanobind)
NAMESPACE_BEGIN(detail)

template <typename Type> struct type_caster<std::valarray<Type>> {
    NB_TYPE_CASTER(std::valarray<Type>, const_name("[") + const_name("std::valarray") + const_name("]"))

    using Caster = make_caster<Type>;

    bool from_python(handle src, uint8_t flags, cleanup_list *cleanup) noexcept {
        if (nb::isinstance<nb::list>(src)) {
            try {
                auto py_array = nb::cast<nb::list>(src);
                size_t size = py_array.size();

                value.resize(size);

                for (size_t i = 0; i < size; i++) {
                    value[i] = nb::cast<Type>(py_array[i]);
                }

                return true;
            }
            catch (...) {
                PyErr_Clear();
                return false;
            }
        }
        else if (nb::isinstance<nb::ndarray<Type, nb::ndim<1>>>(src)) {
            try {
                auto py_array = nb::cast<nb::ndarray<Type, nb::ndim<1>>>(src);
                size_t size = py_array.size();
                auto *data = py_array.data();

                value.resize(size);

                for (size_t i = 0; i < size; i++) {
                    value[i] = data[i];
                }

                return true;
            }
            catch (...) {
                PyErr_Clear();
                return false;
            }
        }

        return false;
    }

    template <typename T>
    static handle from_cpp(T &&src, rv_policy policy, cleanup_list *cleanup) {
        nb::list obj;
        for (const auto& elem : src) {
            obj.append(elem);
        }

        return obj.release();
    }
};

template <typename Type> struct type_caster<tl::optional<Type>> : optional_caster<tl::optional<Type>> {};

NAMESPACE_END(detail)
NAMESPACE_END(nanobind)

NB_MODULE(_PyPartMC, m) {
    m.doc() = R"pbdoc(
        PyPartMC is a Python interface to PartMC.
    )pbdoc";

    m.def("run_part", &run_part, "Do a particle-resolved Monte Carlo simulation.");
    m.def("run_part_timestep", &run_part_timestep, "Do a single time step.");
    m.def("run_part_timeblock", &run_part_timeblock, "Do multiple time steps over a block of time.");
    m.def("condense_equilib_particles", &condense_equilib_particles, R"pbdoc(
      Call condense_equilib_particle() on each particle in the aerosol
      to ensure that every particle has its water content in
      equilibrium.
    )pbdoc");
    m.def("condense_equilib_particle", &condense_equilib_particle, R"pbdoc(
        Determine the water equilibrium state of a single particle.
    )pbdoc");

    m.def("run_sect", &run_sect, "Do a 1D sectional simulation (Bott 1998 scheme).");
    m.def("run_exact", &run_exact, "Do an exact solution simulation.");

    nb::class_<AeroBinned>(m, "AeroBinned",
        R"pbdoc(
             Aerosol number and volume distributions stored per size bin.
             These quantities are densities both in volume (per m^3) and in radius
             (per log_width).
        )pbdoc"
    )
        .def(nb::init<std::shared_ptr<AeroData>>())
        .def(nb::init<std::shared_ptr<AeroData>, const BinGrid&>())
        .def_prop_ro("num_conc", AeroBinned::num_conc,
            "Returns the number concentration of each bin (#/m^3/log_width)")
        .def_prop_ro("vol_conc", AeroBinned::vol_conc,
            "Returns the volume concentration per bin per species (m^3/m^3/log_width)")
        .def("add_aero_dist", AeroBinned::add_aero_dist,
            "Add an AeroDist to an AeroBinned.")
    ;

    nb::class_<AeroData>(m, "AeroData",
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
        .def(nb::init<const CampCore&>())
        .def(nb::init<const nlohmann::ordered_json&>())
        .def("spec_by_name", AeroData::spec_by_name,
             "Return the number of the species in AeroData with the given name.")
        .def("__len__", AeroData::__len__, "Return number of aerosol species.")
        .def_prop_ro("n_source", AeroData::n_source,
             "Number of aerosol sources.")
        .def_prop_ro("sources", AeroData::sources, "Return list of source names.")
        .def_prop_rw("frac_dim", &AeroData::get_frac_dim, &AeroData::set_frac_dim,
             "Volume fractal dimension (1).")
        .def_prop_rw("vol_fill_factor", &AeroData::get_vol_fill_factor,
             &AeroData::set_vol_fill_factor, "Volume filling factor (1).")
        .def_prop_rw("prime_radius", &AeroData::get_prime_radius, &AeroData::set_prime_radius,
            "Radius of primary particles (m).")
        .def_prop_ro("densities", &AeroData::densities,
            "Return array of aerosol species densities.")
        .def_prop_ro("kappa", &AeroData::kappa,
            "Returns array of aerosol species hygroscopicity parameter kappa.")
        .def_prop_ro("molecular_weights", &AeroData::molecular_weights,
            "Return array of aerosol species molecular weights.")
        .def("density", &AeroData::density, "Return density of an aerosol species.")
        .def("rad2vol", AeroData::rad2vol,
            "Convert geometric radius (m) to mass-equivalent volume (m^3).")
        .def("vol2rad", AeroData::vol2rad,
            "Convert mass-equivalent volume (m^3) to geometric radius (m).")
        .def("diam2vol", AeroData::diam2vol,
            "Convert geometric diameter (m) to mass-equivalent volume (m^3).")
        .def("vol2diam", AeroData::vol2diam,
            "Convert mass-equivalent volume (m^3) to geometric diameter (m).")
        .def_prop_ro("species", AeroData::names,
            "Return list of aerosol species names.")
    ;

    nb::class_<AeroParticle>(m, "AeroParticle",
        R"pbdoc(
             Single aerosol particle data structure.

             The \c vol array stores the total volumes of the different
             species that make up the particle. This array must have length
             equal to aero_data%%n_spec, so that \c vol(i) is the volume (in
             m^3) of the i'th aerosol species.
        )pbdoc"
    )
        .def(nb::init<std::shared_ptr<AeroData>, const std::valarray<double>&>())
        .def_prop_ro("volumes", AeroParticle::volumes,
            "Constituent species volumes (m^3)")
        .def_prop_ro("volume", AeroParticle::volume,
            "Total volume of the particle (m^3).")
        .def("species_volume",
            nb::overload_cast<const AeroParticle &, const int &>(&AeroParticle::species_volume),
            "Volume of a single species in the particle (m^3).")
        .def("species_volume",
            nb::overload_cast<const AeroParticle &, const std::string &>(&AeroParticle::species_volume_by_name),
            "Volume of a single species in the particle (m^3).")
        .def_prop_ro("dry_volume", AeroParticle::dry_volume,
            "Total dry volume of the particle (m^3).")
        .def_prop_ro("radius", AeroParticle::radius,
            "Total radius of the particle (m).")
        .def_prop_ro("dry_radius", AeroParticle::dry_radius,
            "Total dry radius of the particle (m).")
        .def_prop_ro("diameter", AeroParticle::diameter,
            "Total diameter of the particle (m).")
        .def_prop_ro("dry_diameter", AeroParticle::dry_diameter,
            "Total dry diameter of the particle (m).")
        .def_prop_ro("mass", AeroParticle::mass,
            "Total mass of the particle (kg).")
        .def("species_mass", nb::overload_cast<const AeroParticle &, const int &>(&AeroParticle::species_mass),
            "Mass of a single species in the particle (kg).")
        .def("species_mass", nb::overload_cast<const AeroParticle &, const std::string &>(&AeroParticle::species_mass_by_name),
            "Mass of a single species in the particle (kg).")
        .def_prop_ro("species_masses", AeroParticle::species_masses,
            "Mass of all species in the particle (kg).")
        .def_prop_ro("solute_kappa", AeroParticle::solute_kappa,
            "Return the average of the solute kappas (1).")
        .def_prop_ro("moles", AeroParticle::moles,
            "Total moles in the particle (1).")
        .def_prop_ro("absorb_cross_sect", AeroParticle::absorb_cross_sect,
            "Absorption cross-section (m^-2).")
        .def_prop_ro("scatter_cross_sect", AeroParticle::scatter_cross_sect,
            "Scattering cross-section (m^-2).")
        .def_prop_ro("asymmetry", AeroParticle::asymmetry,
            "Asymmetry parameter (1).")
        .def_prop_ro("refract_shell", AeroParticle::refract_shell,
            "Refractive index of the shell (1).")
        .def_prop_ro("refract_core", AeroParticle::refract_core,
            "Refractive index of the core (1).")
        .def_prop_ro("sources", AeroParticle::sources,
            "Number of original particles from each source that coagulated to form particle.")
        .def_prop_ro("least_create_time", AeroParticle::least_create_time,
            "First time a constituent was created (s).")
        .def_prop_ro("greatest_create_time", AeroParticle::greatest_create_time,
            "Last time a constituent was created (s).")
        .def_prop_ro("id", AeroParticle::id, "Unique ID number.")
        .def_prop_ro("is_frozen", AeroParticle::is_frozen, "Frozen status - particle is ice if 1.")
        .def("mobility_diameter", AeroParticle::mobility_diameter,
            "Mobility diameter of the particle (m).")
        .def_prop_ro("density", AeroParticle::density,
            "Average density of the particle (kg/m^3)")
        .def("approx_crit_rel_humid", AeroParticle::approx_crit_rel_humid,
            "Return the approximate critical relative humidity (1).")
        .def("crit_rel_humid", AeroParticle::crit_rel_humid,
            "Return the critical relative humidity (1).")
        .def("crit_diameter", AeroParticle::crit_diameter,
            "Return the critical diameter (m).")
        .def("coagulate", AeroParticle::coagulate,
            "Coagulate two particles together to make a new one. The new particle will not have its ID set.")
        .def("zero", AeroParticle::zero,
            "Reset an aero_particle to be zero.")
        .def("set_vols", AeroParticle::set_vols,
            "Set the aerosol particle volumes.")
        .def_prop_rw("weight_group", AeroParticle::get_weight_group, AeroParticle::set_weight_group,
            "Weighting function group number.")
        .def_prop_rw("weight_class", AeroParticle::get_weight_class, AeroParticle::set_weight_class,
            "Weighting function class number.")
        .def("new_id", AeroParticle::new_id, "Assigns a new unique particle ID")
    ;

    nb::class_<AeroState>(m, "AeroState",
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
        .def(nb::init<std::shared_ptr<AeroData>, const double, const std::string>())
        .def(nb::init<std::shared_ptr<AeroData>, const double, const std::string, const CampCore&>())
        .def("__len__", AeroState::__len__,
            "Return current number of particles in the AeroState.")
        .def_prop_ro("total_num_conc", AeroState::total_num_conc,
            "Return the total number concentration of the population.")
        .def_prop_ro("total_mass_conc", AeroState::total_mass_conc,
            "Return the total mass concentration of the population.")
        .def_prop_ro("frozen_fraction", AeroState::frozen_fraction,
            "Return the fraction of ice particles in the population.")
        .def_prop_ro("num_concs", AeroState::num_concs,
            "Return the number concentration of each particle in the population.")
        .def("masses", AeroState::masses,
            "Return the total mass of each particle in the population.",
            nb::arg("include") = nb::none(), nb::arg("exclude") = nb::none()
        )
        .def("volumes", AeroState::volumes,
            "Return the volume of each particle in the population.",
            nb::arg("include") = nb::none(), nb::arg("exclude") = nb::none())
        .def_prop_ro("dry_diameters", AeroState::dry_diameters,
            "Return the dry diameter of each particle in the population.")
        .def("mobility_diameters", AeroState::mobility_diameters,
            "Return the mobility diameter of each particle in the population.")
        .def("diameters", AeroState::diameters,
            "Return the diameter of each particle in the population.",
            nb::arg("include") = nb::none(), nb::arg("exclude") = nb::none())
        .def("crit_rel_humids", AeroState::crit_rel_humids,
            "Return the critical relative humidity of each particle in the population.")
        .def("make_dry", AeroState::make_dry,
            "Make all particles dry (water set to zero).")
        .def_prop_ro("ids", AeroState::ids,
            "Return the IDs of all particles.")
        .def("mixing_state", AeroState::mixing_state,
            "returns the mixing state parameters (d_alpha, d_gamma, chi) of the population",
            nb::arg("include") = nb::none(), nb::arg("exclude") = nb::none(),
            nb::arg("group") = nb::none())
        .def("bin_average_comp", AeroState::bin_average_comp,
            "Composition-averages population using BinGrid.")
        .def("particle", AeroState::get_particle,
            "Return the particle of a given index.")
        .def("rand_particle", AeroState::get_random_particle,
            "Return a random particle from the population.")
        .def("dist_sample", AeroState::dist_sample,
            "sample particles for AeroState from an AeroDist",
            nb::arg("AeroDist"), nb::arg("sample_prop") = 1.0, nb::arg("create_time") = 0.0,
            nb::arg("allow_doubling") = true, nb::arg("allow_halving") = true)
        .def("add_particle", AeroState::add_particle, "Add a particle to an AeroState.")
        .def("add", AeroState::add,
            R"pbdoc(aero_state += aero_state_delta, including combining the
            weights, so the new concentration is the weighted average of the
            two concentrations.)pbdoc")
        .def("add_particles", AeroState::add_particles,
            R"pbdoc(aero_state += aero_state_delta, with the weight left unchanged
             so the new concentration is the sum of the two concentrations.)pbdoc")
        .def("sample", AeroState::sample,
             R"pbdoc(Generates a random sample by removing particles from
             aero_state_from and adding them to aero_state_to, transfering
             weight as well. This is the equivalent of aero_state_add().)pbdoc")
        .def("sample_particles", AeroState::sample_particles,
             R"pbdoc(  !> Generates a random sample by removing particles from
             aero_state_from and adding them to aero_state_to, which must be
             already allocated (and should have its weight set).

             None of the weights are altered by this sampling, making this the
             equivalent of aero_state_add_particles().)pbdoc")
        .def("copy_weight", AeroState::copy_weight,
             "Copy weighting from another AeroState.")
        .def("remove_particle", AeroState::remove_particle,
            "Remove particle of a given index.")
        .def("zero", AeroState::zero, "Remove all particles from an AeroState.")
    ;

    nb::class_<GasData>(m, "GasData",
        R"pbdoc(
            Constant gas data.

            Each gas species is identified by an integer \c i between 1 and
            \c gas_data_n_spec(gas_data). Species \c i has name \c gas_data%%name(i).
            The variable gas data describing the current mixing ratios is stored
            in the gas_state_t structure, so the mixing ratio of species \c i
            is gas_state%%mix_rat(i).
        )pbdoc"
    )
        .def(nb::init<const CampCore&>())
        .def(nb::init<const nb::tuple&>())
        .def("__len__", GasData::__len__,
            "Return the number of gas species.")
        .def_prop_ro("n_spec", GasData::__len__)
        .def("__str__", GasData::__str__,
            "Return a string with JSON representation of the object.")
        .def("spec_by_name", GasData::spec_by_name,
            "Return the number of the species in gas with the given name.")
        .def_prop_ro("species", GasData::names, "Return list of gas species names.")
    ;

    nb::class_<EnvState>(m, "EnvState",
        R"pbdoc(
            Current environment state.

            All quantities are instantaneous, describing the state at a
            particular instant of time. Constant data and other data not
            associated with the current environment state is stored in
            scenario_t.
        )pbdoc"
    )
        .def(nb::init<const nlohmann::ordered_json&>())
        .def("set_temperature", EnvState::set_temperature,
            "Set the temperature of the environment state.")
        .def_prop_ro("temp", EnvState::temp,
            "Return the current temperature of the environment state.")
        .def_prop_ro("rh", EnvState::rh,
            "Return the current relative humidity of the environment state.")
        .def_prop_ro("elapsed_time", EnvState::get_elapsed_time,
            "Return time since start_time (s).")
        .def_prop_ro("start_time", EnvState::get_start_time,
            "Return the simulation start time (s since 00:00 UTC on start_day).")
        .def_prop_rw("height", &EnvState::get_height, &EnvState::set_height,
            "Box height (m).")
        .def_prop_rw("pressure", &EnvState::get_pressure, &EnvState::set_pressure,
            "Ambient pressure (Pa).")
        .def_prop_rw("latitude", &EnvState::get_latitude, &EnvState::set_latitude,
            "Latitude (degrees).")
        .def_prop_rw("longitude", &EnvState::get_longitude, &EnvState::set_longitude,
            "Longitude (degrees).")
        .def_prop_rw("altitude", &EnvState::get_altitude, &EnvState::set_altitude,
            "Altitude (m).")
        .def_prop_ro("air_density", &EnvState::air_density,
            "Air density (kg m^{-3}).")
        .def_prop_ro("air_molar_density", &EnvState::air_molar_density,
            "Air molar density (mol m^{-3}).")
        .def_prop_rw("additive_kernel_coefficient", &EnvState::get_additive_kernel_coefficient, &EnvState::set_additive_kernel_coefficient,
            "Scaling coefficient for additive coagulation kernel.")
        .def("ppb_to_conc", &EnvState::ppb_to_conc, "Convert (ppb) to (molecules m^{-3}).")
        .def("conc_to_ppb", &EnvState::conc_to_ppb, "Convert (molecules m^{-3}) to (ppb).")
        .def_prop_ro("sat_vapor_pressure", &EnvState::sat_vapor_pressure,
            "Returns the current saturation vapor pressure (Pa).")
    ;

    nb::class_<Photolysis>(m,
        "Photolysis",
        R"pbdoc(
            PartMC interface to a photolysis module
        )pbdoc"
    )
        .def(nb::init<>())
        .def(nb::init<const CampCore&>())
    ;

    nb::class_<CampCore>(m,
        "CampCore",
        R"pbdoc(
            An interface between PartMC and the CAMP
        )pbdoc"
    )
        .def(nb::init<>())
        .def(nb::init<const std::string &>())
    ;

    nb::class_<Scenario>(m,
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
            nb::init<
                const GasData&,
                const AeroData&,
                const nlohmann::ordered_json&
            >(),
            "instantiates and initializes from a JSON object"
        )
        .def("__str__", Scenario::__str__,
            "Return a string with JSON representation of the object.")
        .def("init_env_state", Scenario::init_env_state,
            "Initialize the EnvState.")
        .def("aero_emissions", Scenario::get_aero_emission_dist,
            "Return aero_emissions AeroDists at a given index.")
        .def_prop_ro("aero_emissions_n_times", Scenario::get_emissions_n_times,
            "Return the number of times specified for emissions.")
        .def_prop_ro("aero_emissions_rate_scale", Scenario::emission_rate_scale,
            "Aerosol emission rate scales at set-points (1).")
        .def_prop_ro("aero_emissions_time", Scenario::emission_time)
        .def("aero_background", Scenario::get_aero_background_dist,
            "Return aero_background AeroDists at a given index.")
        .def_prop_ro("aero_dilution_n_times", Scenario::get_aero_dilution_n_times,
            "Return the number of times specified for dilution.")
        .def_prop_ro("aero_dilution_rate", Scenario::aero_dilution_rate,
            "Aerosol-background dilution rates at set-points (s^{-1}).")
        .def_prop_ro("aero_dilution_time", Scenario::aero_dilution_time,
            "Aerosol-background dilution set-point times (s).")
    ;

    nb::class_<GasState>(m,
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
        .def(nb::init<std::shared_ptr<GasData>>(),
            "Instantiate and initialize based on GasData.")
        .def("__setitem__", GasState::set_item)
        //.def("__setitem__", GasState::set_items)
        .def("__getitem__", GasState::get_item)
        //.def("__getitem__", GasState::get_items)
        .def("__len__", GasState::__len__, "Return number of gas species.")
        .def_prop_ro("n_spec", GasState::__len__,
            "Return number of gas species.")
        .def("__str__", GasState::__str__,
            "Return a string with JSON representation of the object.")
        .def("set_size", GasState::set_size,
            "Set the GasState to the size of GasData.")
        .def("mix_rat", GasState::mix_rat,
            "Return the mixing ratio of a gas species.")
        .def_prop_rw("mix_rats", &GasState::mix_rats, &GasState::set_mix_rats,
            "Provide access (read or write) to the array of mixing ratios.")
    ;

    nb::class_<RunPartOpt>(m,
        "RunPartOpt",
        "Options controlling the execution of run_part()."
    )
        .def(nb::init<const nlohmann::ordered_json&>())
        .def_prop_ro("t_max", RunPartOpt::t_max, "Total simulation time.")
        .def_prop_ro("del_t", RunPartOpt::del_t, "Time step.")
    ;

    nb::class_<RunSectOpt>(m,
        "RunSectOpt",
        "Options controlling the execution of run_sect()."
    )
        .def(nb::init<const nlohmann::ordered_json&, EnvState&>())
        .def_prop_ro("t_max", RunSectOpt::t_max, "Total simulation time.")
        .def_prop_ro("del_t", RunSectOpt::del_t, "Time step.")
    ;

    nb::class_<RunExactOpt>(m,
        "RunExactOpt",
        "Options controlling the execution of run_exact()."
    )
        .def(nb::init<const nlohmann::ordered_json&, EnvState&>())
        .def_prop_ro("t_max", RunExactOpt::t_max, "Total simulation time.")
    ;

    nb::class_<BinGrid>(m,"BinGrid",
        "1D grid, either logarithmic or linear.")
        .def(nb::init<const int, const nb::str, const double, const double>())
        .def("__len__", BinGrid::__len__, "Return number of bins in the BinGrid.")
        .def_prop_ro("edges", BinGrid::edges, "Return bin edges.")
        .def_prop_ro("centers", BinGrid::centers, "Return bin centers.")
        .def_prop_ro("widths", BinGrid::widths, "Return bin widths.")
    ;

    nb::class_<AeroMode>(m,"AeroMode")
        .def(nb::init<AeroData&, const nlohmann::ordered_json&>())
        .def_prop_rw("num_conc", &AeroMode::get_num_conc, &AeroMode::set_num_conc,
             "Provide access (read or write) to the total number concentration of a mode.")
        .def("num_dist", &AeroMode::num_dist,
             "Return the binned number concenration of a mode.")
        .def_prop_rw("vol_frac", &AeroMode::get_vol_frac,
             &AeroMode::set_vol_frac, "Species fractions by volume.")
        .def_prop_rw("vol_frac_std", &AeroMode::get_vol_frac_std,
             &AeroMode::set_vol_frac_std, "Species fraction standard deviation.")
        .def_prop_rw("char_radius", &AeroMode::get_char_radius,
             &AeroMode::set_char_radius,
             "Characteristic radius, with meaning dependent on mode type (m).")
        .def_prop_rw("gsd", &AeroMode::get_gsd,
             &AeroMode::set_gsd, "Geometric standard deviation.")
        .def("set_sample", &AeroMode::set_sampled)
        .def_prop_ro("sample_num_conc", &AeroMode::get_sample_num_conc,
             "Sample bin number concentrations (m^{-3}).")
        .def_prop_ro("sample_radius", &AeroMode::get_sample_radius,
             "Sample bin radii (m).")
        .def_prop_rw("type", &AeroMode::get_type, &AeroMode::set_type,
             "Mode type (given by module constants).")
        .def_prop_rw("name", &AeroMode::get_name, &AeroMode::set_name,
             "Mode name, used to track particle sources.")
    ;

    nb::class_<AeroDist>(m,"AeroDist")
        .def(nb::init<std::shared_ptr<AeroData>, const nlohmann::ordered_json&>())
        .def_prop_ro("n_mode", &AeroDist::get_n_mode,
            "Number of aerosol modes in the distribution.")
        .def_prop_ro("num_conc", &AeroDist::get_total_num_conc,
            "Total number concentration of a distribution (#/m^3).")
        .def("mode", AeroDist::get_mode,
            "Return the mode of a given index.")
    ;

    m.def(
        "histogram_1d", &histogram_1d, nb::rv_policy::copy,
        "Return a 1D histogram with of the given weighted data, scaled by the bin sizes."
    );

    m.def(
        "histogram_2d", &histogram_2d, nb::rv_policy::copy,
        "Return a 2D histogram with of the given weighted data, scaled by the bin sizes."
    );

    //  TODO #120: auto util = m.def_submodule("util", "...");
    m.def(
        "pow2_above", &pow2_above, nb::rv_policy::copy,
        "Return the least power-of-2 that is at least equal to n."
    );

    m.def(
        "sphere_vol2rad", &sphere_vol2rad, nb::rv_policy::copy,
        "Convert mass-equivalent volume (m^3) to geometric radius (m) for spherical particles."
    );

    m.def(
        "rad2diam", &rad2diam, nb::rv_policy::copy,
        "Convert radius (m) to diameter (m)."
    );

    m.def(
        "sphere_rad2vol", &sphere_rad2vol, nb::rv_policy::copy,
        "Convert geometric radius (m) to mass-equivalent volume for spherical particles."
    );

    m.def(
        "diam2rad", &diam2rad, nb::rv_policy::copy,
        "Convert diameter (m) to radius (m)."
    );

    m.def(
        "loss_rate_dry_dep", &loss_rate_dry_dep, nb::rv_policy::copy,
        "Compute and return the dry deposition rate for a given particle."
    );

    m.def(
        "loss_rate", &loss_rate, nb::rv_policy::copy,
        "Evaluate a loss rate function."
    );

    m.def(
        "output_state", &output_state, "Output current state to netCDF file."
    );

    m.def(
        "input_state", &input_state, "Read current state from netCDF output file."
    );

    m.def(
        "input_sectional", &input_sectional, "Read current state from run_sect netCDF output file."
    );

    m.def(
        "input_exact", &input_exact, "Read current state from run_exact netCDF output file."
    );

    m.def(
        "rand_init", &rand_init, "Initialize the random number generator to the state defined by the given seed. If the seed is 0 then a seed is auto-generated from the current time."
    );

    m.def(
        "rand_normal", &rand_normal, "Generate a normally distributed random number with the given mean and standard deviation."
    );

    auto vobtd = nb::dict();
    vobtd["nanobind"] = MACRO_STRINGIFY(NB_VERSION_MAJOR) "." MACRO_STRINGIFY(NB_VERSION_MINOR) "." MACRO_STRINGIFY(NB_VERSION_PATCH);
    vobtd["PartMC"] = PARTMC_VERSION;
    vobtd["SUNDIALS"] = SUNDIALS_VERSION;
    vobtd["CAMP"] = CAMP_VERSION;
    // TODO #164
    // - expose git hashes?
    // - more submodules (netCDF, ...)
    m.attr("__versions_of_build_time_dependencies__") = vobtd;
}
