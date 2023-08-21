import json
import csv


with open('../sample.json', 'r') as json_file:
    data = json.load(json_file)

csv_file_name = 'data.csv'

with open(csv_file_name, 'w', newline='') as csv_file:

    csv_writer = csv.writer(csv_file)

    header = data[0].keys()
    csv_writer.writerow(header)

    for row in data:
        csv_writer.writerow(row.values())

print(f'JSON data has been successfully converted to {csv_file_name}.')



   

