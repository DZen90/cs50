import cs50

# input number
while True:
    print("Number: ", end = "")
    cc_number = cs50.get_float()
    if cc_number > 0:
        break

length = 0
order = 10
max_order = 10
check_sum = 0
digit = 0
even = False

while cc_number % (order // 10) != cc_number:
    max_order = order
    if even:
        digit = ((cc_number % order) - (cc_number % (order // 10))) / (order // 10) * 2
        if digit >= 10:
            digit = (digit % 10) + digit // 10
    else:
        digit = ((cc_number % order) - (cc_number % (order // 10))) / (order // 10)
    check_sum += digit
    even = not even
    order *= 10
    length += 1

if check_sum % 10 != 0:
    print("INVALID")
    quit()

if length == 16:
    first_two = ((cc_number % max_order) - (cc_number % (max_order // 100))) / (max_order // 100)
    if 51 <= first_two <= 55:
        print("MASTERCARD")
        quit()
    else:
        first = ((cc_number % max_order) - (cc_number % (max_order // 10))) / (max_order // 10)
        if first == 4:
            print("VISA")
            quit()
        else:
            print("INVALID")
            quit()
elif length == 15:
    first_two = ((cc_number % max_order) - (cc_number % (max_order // 100))) / (max_order // 100)
    if first_two == 34 or first_two == 37:
        print("AMEX")
        quit()
    else:
        print("INVALID")
        quit()
elif length == 13 or length == 16:
    first = ((cc_number % max_order) - (cc_number % (max_order // 10))) / (max_order // 10)
    if first == 4:
        print("VISA")
        quit()
    else:
        print("INVALID")
        quit()
else:
    print("INVALID")
    quit()


    
        

