import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Define constants
g = 9.81  # Acceleration due to gravity (m/s^2)
m1, m2 = 1.0, 1.0  # Masses of the pendulum arms (kg)
l1, l2 = 1.0, 1.0  # Lengths of the pendulum arms (m)
dt = 0.01  # Time step (s)

# Equations of motion for the double pendulum
def derivatives(state):
    theta1, omega1, theta2, omega2 = state

    delta = theta2 - theta1
    denom1 = m1 + m2 * np.sin(delta) ** 2

    domega1 = (m2 * g * np.sin(theta2) * np.cos(delta) - m2 * np.sin(delta) * (l1 * omega1 ** 2 * np.cos(delta) + l2 * omega2 ** 2) - (m1 + m2) * g * np.sin(theta1)) / (l1 * denom1)
    domega2 = ((m1 + m2) * (l1 * omega1 ** 2 * np.sin(delta) - g * np.sin(theta2) + g * np.sin(theta1) * np.cos(delta)) + m2 * l2 * omega2 ** 2 * np.sin(delta) * np.cos(delta)) / (l2 * denom1)

    return np.array([omega1, domega1, omega2, domega2])

# Initialize the pendulums
num_pendulums = 50
states = []
initial_angles = np.linspace(-np.pi / 2, np.pi / 2, num_pendulums)
for angle in initial_angles:
    states.append([angle, 0.0, angle / 2, 0.0])

states = np.array(states)

# Prepare for visualization
fig, ax = plt.subplots()
ax.set_xlim(-2 * (l1 + l2), 2 * (l1 + l2))
ax.set_ylim(-2 * (l1 + l2), 2 * (l1 + l2))
ax.set_aspect('equal')

lines = [ax.plot([], [], 'o-', lw=2)[0] for _ in range(num_pendulums)]

# Update function for animation
def update(frame):
    global states
    
    for i in range(len(states)):
        k1 = derivatives(states[i])
        k2 = derivatives(states[i] + dt * k1 / 2)
        k3 = derivatives(states[i] + dt * k2 / 2)
        k4 = derivatives(states[i] + dt * k3)
        states[i] += dt * (k1 + 2 * k2 + 2 * k3 + k4) / 6

    for i, line in enumerate(lines):
        theta1, _, theta2, _ = states[i]
        x1 = l1 * np.sin(theta1)
        y1 = -l1 * np.cos(theta1)
        x2 = x1 + l2 * np.sin(theta2)
        y2 = y1 - l2 * np.cos(theta2)

        line.set_data([0, x1, x2], [0, y1, y2])
    
    return lines

# Animate
ani = FuncAnimation(fig, update, frames=1000, blit=True, interval=dt * 1000)
plt.show()
