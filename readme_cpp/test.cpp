#include "PyPartMC.hpp"
#include "PyPartMC/si.hpp"

int main() {
    auto aero_data = std::shared_ptr<AeroData>(new AeroData({
        { {"OC", {1000.0 * si::kg / (si::m * si::m * si::m), 0.0, 1e-3 * si::kg / si::mol, 0.001, 0.0, 0.0}} },
        { {"BC", {1800.0 * si::kg / (si::m * si::m * si::m), 0.0, 1e-3 * si::kg / si::mol, 0.0,   0.0, 0.0}} }
    }));

    auto aero_dist = AeroDist(
        aero_data, 
        nlohmann::ordered_json::array({
            nlohmann::ordered_json::object({
                {"cooking", {
                    {"mass_frac", nlohmann::ordered_json::array({ nlohmann::ordered_json::object({{"OC", {1.0}}}) }) }, 
                    {"diam_type", "geometric"},
                    {"mode_type", "log_normal"},
                    {"num_conc", 3200.0 / (si::cm * si::cm * si::cm)},
                    {"geom_mean_diam", 8.64 * si::nm},
                    {"log10_geom_std_dev", 0.28}
                }},
                // {"diesel", {
                //     {"mass_frac", nlohmann::ordered_json::array({ nlohmann::ordered_json::object({{"OC", {0.3}}}), nlohmann::ordered_json::object({{"BC", {0.7}}}) }) }, 
                //     {"diam_type", "geometric"},
                //     {"mode_type", "log_normal"},
                //     {"num_conc", 2900.0 / (si::cm * si::cm * si::cm)},
                //     {"geom_mean_diam", 50.0 * si::nm},
                //     {"log10_geom_std_dev", 0.24}
                // }}
            })
        })
    );

    int n_part = 100;
    auto aero_state = AeroState(aero_data, n_part, "nummass_source");
    
    AeroState::dist_sample(aero_state, aero_dist, 1.0, 0.0, true, true);

    auto masses = AeroState::masses(aero_state, {}, {});
    auto num_concs = AeroState::num_concs(aero_state);

    double total_mass = std::inner_product(
        std::begin(num_concs), std::end(num_concs), 
        std::begin(masses), 
        0.0
    );
    
    std::cout.precision(15);
    std::cout << std::scientific << total_mass << " # kg/m3\n";

    return 0;
}