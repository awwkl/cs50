import sys

from cs50 import SQL
db = SQL("sqlite:///students.db")   # connect using cs50's SQL


def main():
    if len(sys.argv) != 2:  # ensure 2 args
        print("Usage: python roster.py Gryffindor")
        sys.exit(1)

    house = sys.argv[1]

    # db.execute returns a list of rows
    # query the house, then sort by last, first names
    rows = db.execute(
        "SELECT * FROM students WHERE house=? ORDER BY last, first", (house)
    )

    for row in rows:
        first = row['first']
        middle = row['middle']
        last = row['last']
        birth = row['birth']
        if row['middle']:
            print(f"{first} {middle} {last}, born {birth}")
        else:
            print(f"{first} {last}, born {birth}")


main()