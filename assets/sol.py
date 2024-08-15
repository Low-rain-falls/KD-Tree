import pandas as pd

# Read the data from the file
filename = './worldcities.csv'  # Replace with the name of your file
df = pd.read_csv(filename)

# Select only the necessary columns
df_selected = df[['city_ascii', 'lat', 'lng']]

# Save the selected data to a new file
output_filename = 'filtered_cities.csv'
df_selected.to_csv(output_filename, index=False)

print(f"Filtered data has been saved to {output_filename}")
