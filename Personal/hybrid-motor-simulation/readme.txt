getting started:
cd hybrid-motor-simulation
python -m venv .venv
.\.venv\Scripts\activate
pip install -r requirements.txt



Wishlist To Do:
- ambient temperature to flow through walls, tank and chamber (which also means add walls)
- flag system to detect failure modes
    - combustion chamber pressure exceeding a certain pressure or temperature
    - oxidizer tank pressure falls below combustion chamber pressure
    - negative or force-clamped values
    - oxidizer tank pressure exceeding a certain pressure (critical w/ ambient temperature + loaded oxidizer mass)