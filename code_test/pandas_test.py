# import pandas as pd

# # Sample data
# data = {
#     'time': ['2024-06-30 15:24:54', '2024-06-30 16:00:00', '2024-07-01 10:30:00', '2024-07-01 12:00:00'],
#     'cumulative_sum': [10, 20, 30, 40]
# }

# # Create the DataFrame
# aaa_df = pd.DataFrame(data)

# # print(aaa_df)

# # Convert the 'time' column to datetime
# aaa_df['time'] = pd.to_datetime(aaa_df['time'])

# # Extract the date part
# aaa_df['date'] = aaa_df['time'].dt.date

# print(aaa_df)

# # Group by the date and get the last value of the cumulative sum for each date
# aaa_df = aaa_df.groupby('date')['cumulative_sum'].sum().reset_index()


# # # Rename columns if necessary
# # new_df.columns = ['date', 'cumcu']

# # Print the new DataFrame
# print(aaa_df)


import pandas as pd

# Sample data
data = {
    'date': ['2024-06-30', '2024-07-02', '2024-07-05'],
    'value': [10, 20, 30]
}

# Create the DataFrame
df = pd.DataFrame(data)

# Convert the 'date' column to datetime
df['date'] = pd.to_datetime(df['date'])

# Set the 'date' column as the index
df.set_index('date', inplace=True)

# Create a complete date range
date_range = pd.date_range(start=df.index.min(), end=df.index.max(), freq='D')

# Reindex the DataFrame to this date range
df_reindexed = df.reindex(date_range)

# Optional: Fill NaN values with 0 or any other value
df_reindexed.fillna(0, inplace=True)

# Reset the index to turn the 'date' index back into a column
df_reindexed.reset_index(inplace=True)

# Rename the index column to 'date'
df_reindexed.rename(columns={'index': 'date'}, inplace=True)

# Print the new DataFrame
print(df_reindexed)
