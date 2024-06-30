# Use the arm32v7 Python image to emulate Raspberry Pi environment
FROM arm32v7/python:3.9-slim

# Set the working directory in the container
WORKDIR /app

# Copy the requirements file into the container
COPY requirements.txt requirements.txt

# Install the dependencies
RUN pip install --no-cache-dir -r requirements.txt

# Copy the rest of the application code into the container
COPY . .

# Command to run the Python script
CMD ["python", "traffic_light_control.py"]
