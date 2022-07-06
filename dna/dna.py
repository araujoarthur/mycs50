import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py SOURCE SEQUENCE")

    # Read database file into a variable
    with open(sys.argv[1], "r") as dbFile:
        db = list(csv.DictReader(dbFile))
        sequences = list(db[0].keys())[1:]
        # Read DNA sequence file into a variable
        with open(sys.argv[2], "r") as seqFile:
            sequence = seqFile.read()

    # TODO: Find longest match of each STR in DNA sequence
    match_count = dict()
    for seq in sequences:
        match_count[seq] = longest_match(sequence, seq)

    # TODO: Check database for matching profiles
    for individual in db:
        for key, value in match_count.items():
            match = False
            if int(individual[key]) != value:
                break
            else:
                match = True
        if match == True:
            print(individual['name'])
            break
    if not match:
        print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in sequence, return longest run found
    return longest_run


main()
