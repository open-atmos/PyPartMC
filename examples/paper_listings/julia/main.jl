using Pkg
Pkg.add("PyCall")

using PyCall
ppmc = pyimport("PyPartMC")

si = ppmc["si"]

aero_data = ppmc.AeroData((
  #       (density, ions in solution, molecular weight, kappa)
  Dict("OC" => (1000 * si.kg/si.m^3, 0, 1e-3 * si.kg/si.mol, 0.001)),
  Dict("BC" => (1800 * si.kg/si.m^3, 0, 1e-3 * si.kg/si.mol, 0))
))

aero_dist = ppmc.AeroDist(aero_data, (
  Dict( 
    "cooking" => Dict(
      "mass_frac" => (Dict("OC" => (1,)),),
      "diam_type" => "geometric",
      "mode_type" => "log_normal",
      "num_conc" => 3200 / si.cm^3,
      "geom_mean_diam" => 8.64 * si.nm,
      "log10_geom_std_dev" => .28,
    )
  ),
  Dict( 
    "diesel" => Dict(
      "mass_frac" => (Dict("OC" => (.3,)), Dict("BC" => (.7,))),
      "diam_type" => "geometric",
      "mode_type" => "log_normal",
      "num_conc" => 2900 / si.m^3,
      "geom_mean_diam" => 50 * si.nm,
      "log10_geom_std_dev" => .24,
    )
  )
))

n_part = 100 
seed = 44
offset = 0 

aero_state = ppmc.AeroState(n_part, aero_data)

ppmc.rand_init(seed, offset)
aero_state.dist_sample(aero_dist)

print(sum(aero_state.masses))
