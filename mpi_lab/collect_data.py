import re
import csv
from pathlib import Path

if __name__ == '__main__':
    BASE_DIR = Path(__file__).resolve(strict=True).parent.parent
    data_path = BASE_DIR / 'data'
    full_data = list()
    csv_filename = "full_data.csv"
    for path in data_path.iterdir():
        if not path.is_file() or path.suffix != ".txt":
            continue
        with open(str(path.absolute()), 'r', encoding="utf-8") as file:
            file_data = file.read().split("\n")
            for line in file_data:
                if line == '':
                    continue
                data_list = line.split("|")
                parsed_data = dict()
                parsed_data["processor_number"] = int(re.sub("\D", "", data_list[0]))
                parsed_data["block_size"] = int(re.sub("\D", "", data_list[1]))
                parsed_data["processors_count"] = int(re.sub("\D", "", data_list[2]))
                parsed_data["time"] = float(data_list[-1].split(":")[-1])
                full_data.append(parsed_data)
    with open(csv_filename, 'w', newline='') as csv_file:
        fieldnames = ["processor_number", "block_size", "processors_count",
                      "time"]
        csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        csv_writer.writeheader()
        csv_writer.writerows(full_data)
