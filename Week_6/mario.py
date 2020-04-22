def main():     # main function. called at end of file
    while True:
        try:    # if no errors (from empty input "", "foo")
            height = int(input("Height: "))
            if (height >= 1 and height <= 8):   # breaks loop if 1-8
                break
        except:         # catches exceptions
            continue    # re-enters while loop

    for i in range(1, height + 1):      # prints pyramid 1 line at a time
        print_line(i, height)   # print_line(hash_count, total_height)


def print_spaces(n):
    for i in range(n):
        print(" ", end="")


def print_hashes(n):
    for i in range(n):
        print("#", end="")


def print_line(hash_count, total_height):
    print_spaces(total_height - hash_count)
    print_hashes(hash_count)
    print_spaces(2)
    print_hashes(hash_count)
    print()


main()