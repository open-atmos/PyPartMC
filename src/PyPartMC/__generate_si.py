from collections import namedtuple


def __generate_si():
    prefixes = {
        "T": 1e12,
        "G": 1e9,
        "M": 1e6,
        "k": 1e3,
        "h": 1e2,
        "da": 1e1,
        "": 1e0,
        "d": 1e-1,
        "c": 1e-2,
        "m": 1e-3,
        "u": 1e-6,
        "n": 1e-9,
        "p": 1e-12,
    }
    units = {
        "m": 1e0,
        "g": 1e-3,
        "s": 1e0,
        "K": 1e0,
        "Pa": 1e0,
        "mol": 1e0,
        "W": 1e0,
        "J": 1e0,
        "N": 1e0,
    }
    return namedtuple("SI", [prefix + unit for prefix in prefixes for unit in units])(
        **{
            prefix_k + unit_k: prefix_v * unit_v
            for prefix_k, prefix_v in prefixes.items()
            for unit_k, unit_v in units.items()
        }
    )
