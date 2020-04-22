def main():
    text = input("Text: ")      # get input text

    index = get_index(text)     # calculate index
    print_grade(index)          # print "Grade X" based on index


def get_index(text):
    words = 1       # start at 1 because it counts by spaces e.g. "Dogs. Cats."
    letters = sentences = 0

    for char in text:   # check each character in text string
        if char == " ":
            words += 1
        if char in ("!", "?", "."):
            sentences += 1
        if char.isalpha():
            letters += 1

    L = (letters / words) * 100   # (letters / words) is number of letters per 1 word
    S = (sentences / words) * 100

    return (0.0588 * L - 0.296 * S - 15.8)


def print_grade(index):     # print "Grade X" based on index
    grade = round(index)    # round it to nearest int

    if grade >= 16:
        print("Grade 16+")
        return
    elif grade < 1:
        print("Before Grade 1")
        return
    else:
        print(f"Grade {grade}")


main()