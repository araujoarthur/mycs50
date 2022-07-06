from sys import exit

height = ''
invalid = True

while (invalid):
    if((not height.isnumeric()) or (int(height) > 8 or int(height) < 1)):
        height = input("Height: ")
    else:
        invalid = False

height = int(height)

# Iterate to form rows
for i in range(1, height+1):
    # It prints height - i blocks of whitespaces, then i blocks of #, then space then i blocks of # again
    print(' ' * (height-i) + "#" * i + '  ' + "#" * i)

