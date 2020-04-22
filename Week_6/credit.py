def main():
    credit = int(input("Number: "))     # get input, convert to int

    if not is_valid(credit):    # if INVALID
        print("INVALID")
        return

    check_card_type(credit)     # print what type of card, or INVALID


def multiply_digit(n):
    n = n * 2                   # multiply by 2
    return int(n/10) + n % 10   # return sum of digits


def is_valid(credit):           # use Luhn’s algorithm
    sum = 0

    while (credit != 0):
        sum += credit % 10
        credit = int(credit / 10)

        sum += multiply_digit(credit % 10)
        credit = int(credit / 10)

    if (sum % 10 == 0):
        return True

    return False


def check_card_type(credit):
    credit = str(credit)            # convert to string
    length = len(credit)            # get length
    first_digit = credit[0:1]       # get first digit, in string form
    first_two_digits = credit[0:2]  # get first two digits, in string form

    if length == 15:
        if first_two_digits == "34" or first_two_digits == "37":
            print("AMEX")
            return

    if length == 16:
        if int(first_two_digits) >= 51 and int(first_two_digits) <= 55:
            print("MASTERCARD")
            return

    if (length == 13 or length == 16):
        if first_digit == "4":
            print("VISA")
            return

    print("INVALID")    # if none of those types, print INVALID


main()