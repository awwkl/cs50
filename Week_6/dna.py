import sys
import csv

""" ### FROM databases/large.csv, DATA STRUCTURES WILL BE CREATED THAT LOOK LIKE THESE:
counts_dictionary = {
    "15,49,38,5,14,44,14,12": "Albus",
    "31,21,41,28,30,9,36,44": "Cedric"
}

str_list = ["AGATC", "TTTTTTCT", "AATG", "TCTAG", "GATA", "TATC"]

FROM sequences/1.txt,
      dna_sequence = "AGAAAGTGATGAGGGAGATAGTTAGGAAATTAAGAAAAATTAT..."

CALL get_dna_str_counts(dna_sequence, str_list)
    RETURN "31,21,41,28,30,9,36,44"
GET counts_dictionary[dna_str_counts] (dictionary[KEY] => VALUE)
PRINT it
"""

# global variables - need to be accessed across all functions
db_file_name = sys.argv[1]      # e.g. databases/large.csv
seq_file_name = sys.argv[2]     # e.g. sequences/1.txt

counts_dictionary = {}      # e.g. {"44,14,12": "Alice", "9,36,44": "Bob"}
str_list = []               # list of STRs e.g. ["TATA", "ATGC", "GGGCC"]


def main():
    with open(db_file_name) as csv_file:    # open databases/large.csv
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:      # for each line
            if line_count == 0:     # name,AGATC,AATG,TATC
                str_list = row[1:]  # e.g. ["AGATC", "AATG", "TATC"]
                line_count += 1
            else:                   # Alice,2,8,3
                str_counts_list = row[1:]                       # ["2", "8", "3"]
                str_counts_string = ",".join(str_counts_list)   # "2,8,3"
                counts_dictionary[str_counts_string] = row[0]   # "Alice"
                line_count += 1

    sequence_file = open(seq_file_name, "r")        # open DNA sequence file
    dna_sequence = sequence_file.read().strip()     # get DNA sequence, remove \n
    dna_str_counts = get_dna_str_counts(dna_sequence, str_list)     # "2,8,3"

    try:
        print(counts_dictionary[dna_str_counts])    # counts_dictionary["2,8,3"] => "Alice"
    except:
        print("No match")   # KeyError if access a key nott in dictionary


def get_dna_str_counts(dna_sequence, str_list):
    dna_str_counts = ""         # eventually "2,8,3"
    for str_name in str_list:
        dna_str_counts += str(longest_repeat(str_name, dna_sequence))   # "2"
        dna_str_counts += ","

    return dna_str_counts[:-1]  # remove last comma from "2,8,3,"


def longest_repeat(str, dna_sequence):
    longest_run = 0                         # returns 0 if no match at all

    for count in range(1, 100):             # likely max 100 longest
        if (str * count) in dna_sequence:   # if "ATGC" * 2 in "TTTATGCATGC"
            longest_run = count
        else:
            break

    return longest_run


main()