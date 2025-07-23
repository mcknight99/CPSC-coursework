import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import pint

# Initialize unit registry
ureg = pint.UnitRegistry()
Q_ = ureg.Quantity

# Constants
R_UNIV = 8314.5  # J/(kmol·K)
M_N2O = 44.013  # Molar mass of N2O, g/mol
R_N2O = R_UNIV / M_N2O * 1000  # J/(kg·K)

# Load inputs
def load_inputs(csv_file="input_parameters.csv"):
    df = pd.read_csv(csv_file)
    inputs = {}

    # Define expected units for all known parameters
    expected_units = {
        "injector_diameter": "meter",
        "tank_inner_diameter": "meter",
        "tank_length": "meter",
        "chamber_diameter": "meter",
        "chamber_length": "meter",
        "nozzle_throat_diameter": "meter",
        "nozzle_exit_diameter": "meter",
        "ambient_pressure": "pascal",
        "tank_temperature_initial": "kelvin",
        "fuel_density": "kilogram / meter ** 3",
        "oxidizer_mass_initial": "kilogram",
        "fuel_regression_a": "meter / second", #mm/s
        "fuel_regression_n": "dimensionless",
        "injector_cd": "dimensionless",
        "time_step": "second",
        "burn_time": "second",
        "fuel_port_diameter_initial": "meter",
        # Add more expected params here as needed
    }

    for _, row in df.iterrows():
        param = row['parameter']
        val = float(row['value'])
        unit = row['unit']

        try:
            if pd.isna(unit) or unit.strip() in ["-", ""]:
                quantity = Q_(val, "dimensionless")
            else:
                quantity = Q_(val, unit)

            # Convert to expected unit if defined
            if param in expected_units:
                quantity = quantity.to(expected_units[param])
                print(f"✅ Parsed '{param}' with unit '{unit}': {quantity:.5f}")

            inputs[param] = quantity.magnitude  # Store as float (unitless now)
            #inputs[param] = quantity
        except Exception as e:
            print(f"❌ Error parsing '{param}' with unit '{unit}': {e}")
            raise

    return inputs


# Load CEA lookup table
def load_cea_lookup(file="cea_lookup.csv"):
    df = pd.read_csv(file)
    return df

# Interpolate combustion temperature from CEA data
def get_combustion_temperature(of_ratio, cea_df):
    # Linear interpolation from CEA table
    if of_ratio < cea_df["O/F"].min() or of_ratio > cea_df["O/F"].max():
        return 3000  # fallback value
    return np.interp(of_ratio, cea_df["O/F"], cea_df["Temperature (K)"])

# Compute oxidizer flow with pressure drop
def compute_oxidizer_flow(P_tank, P_cc, rho_ox, Cd, A_inj):
    delta_P = max(P_tank - P_cc, 0)
    return Cd * A_inj * np.sqrt(2 * rho_ox * delta_P)

# Fuel regression
def compute_fuel_flow(G_ox, a, n, rho_fuel, L, D_port):
    r_dot = a * G_ox**n
    A_burn = np.pi * D_port * L
    m_dot_fuel = rho_fuel * A_burn * r_dot
    return m_dot_fuel, r_dot

# Blowdown model: ideal gas approximation
def update_tank_state(m_ox, V_tank, T_tank):
    if m_ox <= 0:
        return 0, 0
    rho_ox = m_ox / V_tank
    P_tank = rho_ox * R_N2O * T_tank
    return P_tank, rho_ox

def run_simulation(params, cea_df):
    dt = params["time_step"]
    burn_time = params["burn_time"]
    steps = int(burn_time / dt)

    # Geometry
    A_inj = np.pi * (params["injector_diameter"] / 2) ** 2
    A_tank = np.pi * (params["tank_inner_diameter"] / 2) ** 2
    V_tank = A_tank * params["tank_length"]
    A_cc = np.pi * (params["chamber_diameter"] / 2) ** 2
    V_cc = A_cc * params["chamber_length"]

    A_throat = np.pi * (params["nozzle_throat_diameter"] / 2) ** 2
    D_exit = params["nozzle_exit_diameter"]
    A_exit = np.pi * (D_exit / 2) ** 2

    # Initial state
    m_ox = params["oxidizer_mass_initial"]
    D_port = params["fuel_port_diameter_initial"]
    T_tank = params["tank_temperature_initial"]
    T_combustion = 3000  # default
    P_cc = params["ambient_pressure"]  # init guess

    history = []

    for step in range(steps):
        t = step * dt
        print(f"Step {step + 1}/{steps}, Time: {t:.2f}s, Oxidizer Mass: {m_ox:.2f} kg, Fuel Port Diameter: {D_port:.2f} m -> ", end='')
        P_tank, rho_ox = update_tank_state(m_ox, V_tank, T_tank)
        if P_tank <= params["ambient_pressure"]:
            print("Tank pressure below ambient pressure, stopping simulation.")
            break

        m_dot_ox = compute_oxidizer_flow(P_tank, P_cc, rho_ox, params["injector_cd"], A_inj)
        A_port = np.pi * (D_port / 2) ** 2
        G_ox = m_dot_ox / A_port if A_port > 0 else 0

        m_dot_fuel, r_dot = compute_fuel_flow(
            G_ox, params["fuel_regression_a"], params["fuel_regression_n"],
            params["fuel_density"], params["fuel_grain_length"], D_port
        )

        m_dot_total = m_dot_ox + m_dot_fuel
        of_ratio = m_dot_ox / m_dot_fuel if m_dot_fuel > 0 else 0

        # Interpolate T_combustion
        T_combustion = get_combustion_temperature(of_ratio, cea_df)

        # Estimate chamber pressure and thrust
        gamma = 1.2  # assumed
        R_gas = R_UNIV / 29 * 1000  # crude estimate of molar mass
        P_cc = m_dot_total * R_gas * T_combustion / V_cc
        v_e = np.sqrt(2 * gamma / (gamma - 1) * R_gas * T_combustion)
        thrust = m_dot_total * v_e + (P_cc - params["ambient_pressure"]) * A_exit

        # Update oxidizer & fuel
        m_ox -= m_dot_ox * dt
        D_port += 2 * r_dot * dt

        # Save step
        history.append({
            "time": t,
            "m_dot_ox": m_dot_ox,
            "m_dot_fuel": m_dot_fuel,
            "m_dot_total": m_dot_total,
            "of_ratio": of_ratio,
            "P_cc": P_cc,
            "T_combustion": T_combustion,
            "thrust": thrust,
            "D_port": D_port,
            "m_ox_remaining": m_ox
        })
        print(f"Thrust: {thrust:.2f} N, Chamber Pressure: {P_cc:.2f} Pa, O/F Ratio: {of_ratio:.2f} ", end='')
        print(f"Oxidizer Remaining: {m_ox:.2f} kg")
        if m_ox <= 0:
            print("All oxidizer consumed, stopping simulation.")
            break

    return pd.DataFrame(history)

def plot_results(df):
    plt.figure(figsize=(10, 6))
    plt.subplot(3, 1, 1)
    plt.plot(df["time"], df["thrust"])
    plt.ylabel("Thrust (N)")

    plt.subplot(3, 1, 2)
    plt.plot(df["time"], df["P_cc"])
    plt.ylabel("Chamber Pressure (Pa)")

    plt.subplot(3, 1, 3)
    plt.plot(df["time"], df["of_ratio"])
    plt.ylabel("O/F Ratio")
    plt.xlabel("Time (s)")

    plt.tight_layout()
    #plt.show()

def main():
    params = load_inputs("input_parameters.csv")
    cea_df = load_cea_lookup("cea_lookup.csv")
    results_df = run_simulation(params, cea_df)
    results_df.to_csv("simulation_output.csv", index=False)
    plot_results(results_df)

if __name__ == "__main__":
    main()