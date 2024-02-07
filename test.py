#!/usr/bin/env python3
import sqlite3
from datetime import datetime, timedelta

# Function to convert string time to datetime object
def convert_str_to_datetime(time_str):
    return datetime.strptime(time_str, '%Y-%m-%d%H%M%S')

# Function to retrieve data from the last 10 minutes
def get_data_last_10_minutes():
    # Connect to the SQLite database
    conn = sqlite3.connect('WeatherData.db')
    cursor = conn.cursor()

    # Calculate the time 10 minutes ago from the current time
    ten_minutes_ago = datetime.now() - timedelta(minutes=10)

    # Execute the query to retrieve data from the last 10 minutes
    query = "SELECT id, node, time, temperature, humidity, pressure FROM Weatherdata"
    cursor.execute(query)

    # Fetch all the rows and filter based on time
    result = [row for row in cursor.fetchall() if convert_str_to_datetime(row[2]) >= ten_minutes_ago]

    # Close the connection
    conn.close()

    return result

# Function to retrieve distinct node numbers from the database
def get_distinct_nodes():
    # Connect to the SQLite database
    conn = sqlite3.connect('WeatherData.db')
    cursor = conn.cursor()

    # Execute the query to retrieve distinct node numbers
    query = "SELECT DISTINCT node FROM Weatherdata"
    cursor.execute(query)

    # Fetch all the distinct nodes
    nodes = [row[0] for row in cursor.fetchall()]

    # Close the connection
    conn.close()

    return nodes

# Function to calculate the average temperature for a specific node
def calculate_average_temperature_for_node(data, node_number):
    if not data:
        return None

    # Filter data based on node number
    filtered_data = [row[3] for row in data if row[1] == node_number]

    if not filtered_data:
        return None

    # Calculate the average temperature
    average_temperature = sum(filtered_data) / len(filtered_data)

    return average_temperature

# Function to calculate the average pressure for a specific node
def calculate_average_pressure_for_node(data, node_number):
    if not data:
        return None

    # Filter data based on node number
    filtered_data = [row[5] for row in data if row[1] == node_number]

    if not filtered_data:
        return None

    # Calculate the average pressure
    average_pressure = sum(filtered_data) / len(filtered_data)

    return average_pressure

# Function to calculate the average humidity for a specific node
def calculate_average_humidity_for_node(data, node_number):
    if not data:
        return None

    # Filter data based on node number
    filtered_data = [row[4] for row in data if row[1] == node_number]

    if not filtered_data:
        return None

    # Calculate the average humidity
    average_humidity = sum(filtered_data) / len(filtered_data)

    return average_humidity

# Example usage for distinct node numbers
distinct_nodes = get_distinct_nodes()

for node_number_to_filter in distinct_nodes:
    data_last_10_minutes = get_data_last_10_minutes()  # Call the function to get data

    # Calculate average temperature, pressure, and humidity for the current node
    average_temperature_node = calculate_average_temperature_for_node(data_last_10_minutes, node_number_to_filter)
    average_pressure_node = calculate_average_pressure_for_node(data_last_10_minutes, node_number_to_filter)
    average_humidity_node = calculate_average_humidity_for_node(data_last_10_minutes, node_number_to_filter)

    # Print the results for the current node
    print(f"Node {node_number_to_filter} - "
          f"{'Average Temperature: {:.2f} °C, '.format(average_temperature_node) if average_temperature_node is not None else ''}"
          f"{'Average Pressure: {:.2f} hPa, '.format(average_pressure_node) if average_pressure_node is not None else ''}"
          f"{'Average Humidity: {:.2f}%'.format(average_humidity_node) if average_humidity_node is not None else ''}")
          

# Example usage for all nodes
data_last_10_minutes = get_data_last_10_minutes()  # Call the function to get data

# Initialize lists to store non-null values
temperature_values = []
pressure_values = []
humidity_values = []

for node_number_to_filter in distinct_nodes:
    # Calculate average temperature, pressure, and humidity for the current node
    average_temperature_node = calculate_average_temperature_for_node(data_last_10_minutes, node_number_to_filter)
    average_pressure_node = calculate_average_pressure_for_node(data_last_10_minutes, node_number_to_filter)
    average_humidity_node = calculate_average_humidity_for_node(data_last_10_minutes, node_number_to_filter)

    # Append non-null values to the respective lists
    if average_temperature_node is not None:
        temperature_values.append(average_temperature_node)
    if average_pressure_node is not None:
        pressure_values.append(average_pressure_node)
    if average_humidity_node is not None:
        humidity_values.append(average_humidity_node)

# Calculate overall averages
overall_average_temperature = sum(temperature_values) / len(temperature_values) if temperature_values else None
overall_average_pressure = sum(pressure_values) / len(pressure_values) if pressure_values else None
overall_average_humidity = sum(humidity_values) / len(humidity_values) if humidity_values else None

# Print the overall averages
print(f"Overall Average Temperature: {overall_average_temperature:.2f} °C" if overall_average_temperature is not None else "No data available.")
print(f"Overall Average Pressure: {overall_average_pressure:.2f} hPa" if overall_average_pressure is not None else "No data available.")
print(f"Overall Average Humidity: {overall_average_humidity:.2f}%" if overall_average_humidity is not None else "No data available.")
