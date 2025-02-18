import pandas as pd

# Load the GTFS files
stops_df = pd.read_csv('stops.txt')
stop_times_df = pd.read_csv('stop_times.txt')
trips_df = pd.read_csv('trips.txt')
calendar_df = pd.read_csv('calendar.txt')  # Contains service days

# Step 1: Find the stop_id for 'Pelc Tyrolka'
pelc_tyrolka_stop = stops_df[stops_df['stop_name'] == 'Pelc Tyrolka']
if pelc_tyrolka_stop.empty:
    print("Stop 'Pelc Tyrolka' not found.")
    exit()
pelc_tyrolka_stop_id = pelc_tyrolka_stop.iloc[0]['stop_id']
print(f"Found 'Pelc Tyrolka' with stop_id: {pelc_tyrolka_stop_id}")

# Step 2: Extract all stop times at 'Pelc Tyrolka'
pelc_tyrolka_stop_times = stop_times_df[stop_times_df['stop_id'] == pelc_tyrolka_stop_id]

# Step 3: Retrieve the corresponding trip details
pelc_tyrolka_trip_details = pelc_tyrolka_stop_times.merge(trips_df[['trip_id', 'route_id', 'service_id']], on='trip_id')

# Step 4: Filter by active service days (including weekends)
active_days = ['monday', 'tuesday', 'wednesday', 'thursday', 'friday', 'saturday', 'sunday']  # Including weekends

# Filter the calendar to get services that operate on the desired days
active_services = calendar_df[
    (calendar_df[active_days[0]] == 1) |
    (calendar_df[active_days[1]] == 1) |
    (calendar_df[active_days[2]] == 1) |
    (calendar_df[active_days[3]] == 1) |
    (calendar_df[active_days[4]] == 1) |
    (calendar_df[active_days[5]] == 1) |
    (calendar_df[active_days[6]] == 1)
]

# Step 5: Filter trips based on active services
active_trip_ids = active_services['service_id'].unique()
pelc_tyrolka_trip_details = pelc_tyrolka_trip_details[pelc_tyrolka_trip_details['service_id'].isin(active_trip_ids)]

# Step 6: Extract only stop time and the days the service operates
# We will only include the relevant columns: stop_time and the active days columns
output_df = pelc_tyrolka_trip_details[['arrival_time', 'departure_time', 'service_id']]

# Step 7: Map the service days to the output
# Create a dictionary for active days
days_mapping = {
    'monday': 'Monday',
    'tuesday': 'Tuesday',
    'wednesday': 'Wednesday',
    'thursday': 'Thursday',
    'friday': 'Friday',
    'saturday': 'Saturday',
    'sunday': 'Sunday'
}

# Add a new column for the operating days
def map_operating_days(service_id):
    service_row = active_services[active_services['service_id'] == service_id]
    active_days_list = [days_mapping[day] for day in active_days if service_row[day].values[0] == 1]
    return ', '.join(active_days_list)

output_df['operating_days'] = output_df['service_id'].apply(map_operating_days)

# Step 8: Save the results to a CSV file
output_file = 'pelc_tyrolka_stop_times_operating_days.csv'
output_df.to_csv(output_file, index=False)

print(f"Saved the filtered results to {output_file}")

