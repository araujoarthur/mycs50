ALPHABET = [chr(i) for i in range(65, 91)]


def main():
    text = input("Text: ")
    if (grade := colemanliau(text)) > 16:
        print("Grade: 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print('Grade: ' + str(grade))


def colemanliau(text):
    # Sum the occurence of each !, ? and . It should be equal to the number of sentences
    sentences = text.count("!") + text.count("?") + text.count(".")
    # (Inner to outer) Filter every character for alphabetical characters, then creates a list of values and gets its length
    letters = len(list(filter(lambda x: x.upper() in ALPHABET, [a for a in text])))
    # Just gather the length of the text splitted by spaces.
    words = len(text.split(" "))

    L = letters * 100 / words  # the average number of letters per 100 words in the text
    S = sentences * 100 / words  # the average number of sentences per 100 words in the text
    # 0.0588 * letters_per - 0.296 * sentences_per - 15.8;
    return round(0.0588 * L - 0.296 * S - 15.8)


if __name__ == "__main__":
    main()
