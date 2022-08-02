import random, time, os

        
def start():
    os.system('clear')
    print("Welcome to Guest the Hidden Number Mini Game!")

    time.sleep(2)
    print("Please Enter the Difficulty, Type \"e\" for Easy with 8 guest count, \"m\" for Medium with 7 guest count, \"h\" for Hard with 6 guest count, and \"a\" for Asia with 3 guest count!")
    
    a = input("> ").lower()
    if a == str('e'):
        print("Pooling Number is 1 to 20")
        b = random.randint(1, 20)
        guest_counter = 8
    elif a == str('m'):
        print("Pooling Number is 1 to 50")
        b = random.randint(1, 50)
        guest_counter = 7
    elif a == str('h'):
        print("Pooling Number is 1 to 100")
        b = random.randint(1, 100)
        guest_counter = 6
    elif a == str('a'):
        print("Pooling Number is 1 to 1000")
        b = random.randint(1, 1000)
        guest_counter = 3
    elif a == int():
        exit()
    else:
        start()

    guest = False
    while not guest:
        guest_counter -= 1
        if guest_counter == int(-1):
            if a == str('a'):
                print(f"Ops your Count is out, The Hidden Number is {b}, You Noob")
            else:
                print(f"Ops your Count is out, The Hidden Number is {b}")

            time.sleep(2)
            d = input('Please Enter to Restart the Game')
            if d == str():
                os.system('clear')
                start()
            else:
                exit()

        print("Guest the Number...")
        c = int(input("> "))
        if b == str():
            print("You Enter a Wrong Character your Game is Restarted")
            time.sleep(2)
            start()
        elif b == c:
            guest = True
        elif b < c:
            print(f"You are to High!, Guest Count {guest_counter}")
        elif b > c:
            print(f"You are to Low!, Guest Count {guest_counter}")
        else:
            exit()

    if a == str('e'):
        print(f"Congratulation You Guest the Number, The Hidden Number is {b}, Hey Try the Medium one!")
    elif a == str('m'):
        print(f"Congratulation You Guest the Number, The Hidden Number is {b}, Hey Try the Hard one!")
    elif a == str('h'):
        print(f"Congratulation You Guest the Number, The Hidden Number is {b}, Hey don't Forget to Beat the Asian one!")
    elif a == str('a'):
        print("So you are Asian! What the Hell!")

    time.sleep(5)
    d = input('Please Enter to Restart the Game')
    if d == str():
        os.system('clear')
        start()
    else:
        exit()
    

start()
