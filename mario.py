import cs50
while True:
    print("Height: ", end = "")
    height = cs50.get_int()
    if height > 0 and height < 23:
        break

for i in range(1, height + 1):
    j = height
    while j > i:
        print(" ", end = "")
        j -= 1
    while j > 0:
        print("#", end = "")
        j -= 1
    print("  ", end = "")
    while j < i:
        print("#", end = "")
        j += 1
    print()
