import csv
import sys

from cs50 import SQL
db = SQL("sqlite:///students.db")


def main():
    if len(sys.argv) != 2:
        print("Usage: python import.py characters.csv")
        sys.exit(1)

    csv_filename = sys.argv[1]
    with open(csv_filename) as csv_file:
        csv_file.readline()

        csv_reader = csv.reader(csv_file, delimiter=',')
        for row in csv_reader:
            name_array = row[0].split()     # ["Adelaide", "Murton"]
            house = row[1]
            birth = row[2]

            first = name_array[0]
            middle = None

            if len(name_array) == 2:
                last = name_array[1]
                sql = "INSERT INTO students (first, last, house, birth) VALUES (%s, %s, %s, %s)"
                val = (first, last, house, birth)
                db.execute(sql, val)

            else:
                middle = name_array[1]
                last = name_array[2]

                sql = "INSERT INTO students (first, middle, last, house, birth) VALUES (%s, %s, %s, %s, %s)"
                val = (first, middle, last, house, birth)
                db.execute(sql, val)

            # if middle:
            #     print(f"{first} {middle} {last},{house},{birth}")
            # else:
            #     print(f"{first} {last},{house},{birth}")


main()