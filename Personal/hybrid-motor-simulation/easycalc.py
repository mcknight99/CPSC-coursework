import sympy as sp

# constants/params

OF_ratio = 7              # O:F ratio by mass (5:1)
H_tot = 80.88376          # Total usable height (cm)
ID = 4.92252              # Inner diameter (cm)
p_abs = 1.055             # Density of ABS (g/cm^3)
p_n2o = 0.7265975         # Density of N2O (g/cm^3) at 850 psi / 80F
r = 2.286                 # ABS grain core diameter (cm)
F_ullage = 0.1            # Ullage fraction (10%)

IR = ID/2                 # Inner radius (cm)
d = r/2                   # Core radius (cm)

# unknowns to solve for
H_abs, H_n2o = sp.symbols('H_abs H_n2o')

# ABS is outer circle minus core
# Area_abs = π(IR^2 - r^2)
Area_abs = sp.pi * (IR**2 - d**2)

# N2O is a full cylinder
# Area_n2o = π(IR^2)
Area_n2o = sp.pi * (IR**2)

# Mass equations 
M_abs = p_abs * Area_abs * H_abs
M_n2o = p_n2o * Area_n2o * H_n2o

# Equation 1:
# Oxidizer mass must equal OF_ratio × fuel mass
# M_n2o = OF_ratio × M_abs
eq1 = sp.Eq(M_n2o, OF_ratio * M_abs)

# Equation 2:
# Total height constraint with ullage
#
# The ullage reduces available oxidizer height.
# Given:
# (1 + F_ullage) * H_abs + H_n2o = H_tot
#
# This accounts for additional empty volume above oxidizer.
eq2 = sp.Eq((1 + F_ullage) * H_abs + H_n2o, H_tot)

# Solve the system
solution = sp.solve((eq1, eq2), (H_abs, H_n2o))
H_abs_val = float(solution[H_abs])
H_n2o_val = float(solution[H_n2o])

# Print results
print(f"H_abs  = {H_abs_val:.4f} cm")
print(f"H_n2o  = {H_n2o_val:.4f} cm")

# compute masses
M_abs_val = float(M_abs.subs(H_abs, H_abs_val))
M_n2o_val = float(M_n2o.subs(H_n2o, H_n2o_val))

print(f"M_abs  = {M_abs_val:.4f} g")
print(f"M_n2o  = {M_n2o_val:.4f} g")
# print ullage height for reference
F_ullage_val = F_ullage * H_abs_val
print(f"F_ullage = {F_ullage_val:.4f} cm")
# double check total height
total_height_check = (1 + F_ullage) * H_abs_val + H_n2o_val
print(f"Total height check: {total_height_check:.4f} cm (should be {H_tot:.4f} cm)")


# sanity check values
# volume:
V_abs = Area_abs * H_abs_val
V_n2o = Area_n2o * H_n2o_val
print(f"V_abs = {V_abs:.4f} cm^3")  
print(f"V_n2o = {V_n2o:.4f} cm^3")

#density check
p_abs_check = M_abs_val / V_abs
p_n2o_check = M_n2o_val / V_n2o
print(f"p_abs check = {p_abs_check:.4f} g/cm^3 (should match p_abs)")
print(f"p_n2o check = {p_n2o_check:.4f} g/cm^3 (should match p_n2o)")

