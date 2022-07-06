
def main():
    card_number = input("Card number: ")
    lengths = [13, 15, 16]
    if len(card_number) not in lengths:
        print("INVALID")
        exit()
    is_valid = luhns(card_number)
    print(cardProvider(card_number))


def cardProvider(cardNumber):
    providers = {"AMEX": [34, 37],
                 "MASTERCARD": [51, 52, 53, 54, 55],
                 "VISA": [4]}

    cardNumber = cardNumber[0:2]
    for key in providers.keys():
        if key != "VISA":
            if int(cardNumber) in providers[key]:
                return key
        # Just to be sure
        elif key == "VISA":
            if int(cardNumber[0]) in providers[key]:
                return key
    return "INVALID"


def luhns(cardNumber):
    numblist = list(cardNumber)
    for i in range(0, len(numblist)):
        numblist[i] = int(numblist[i])
    sum_dg = 0
    # Using an alternator to change behavior inside the loop after each interaction
    # It allows me to use the same loop to make two operations alternating on the same array without
    # Having to loop twice through the structure
    alternator = False

    # Iterating throught the range form 0 to length of numblist, but reversed (so from length of numblist to 0)
    for k in reversed(range(0, len(numblist))):
        # Checks for alternator status. If true, realizes the product check, else just sum digt
        if(alternator):
            if (prod := numblist[k] * 2) >= 10:
                digit_list = [int(a) for a in str(prod)]
                sum_dg += sum(digit_list)
            else:
                sum_dg += prod
        else:
            sum_dg += numblist[k]
        # Changes the state of the alternator
        alternator = not alternator
    # Will consider a card valid if the sum ends with 0 (i.e the sum modulo 10 is 0)
    if sum_dg % 10 == 0:
        return True
    else:
        return False


if __name__ == "__main__":
    main()

