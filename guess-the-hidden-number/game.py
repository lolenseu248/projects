import random, time, os

        
def start():
    os.system('clear')
    print("Welcome to Guess the Hidden Number Mini Game!")

    time.sleep(2)
    print("Please Enter the Difficulty, Type \"e\" for Easy with 8 guess count, \"m\" for Medium with 7 guess count, \"h\" for Hard with 6 guess count, and \"a\" for Asia with 3 guess count!")
    
    a = input("> ").lower()
    if a == str('e'):
        print("Pooling Number is 1 to 20")
        b = random.randint(1, 20)
        guess_counter = 8
    elif a == str('m'):
        print("Pooling Number is 1 to 50")
        b = random.randint(1, 50)
        guess_counter = 7
    elif a == str('h'):
        print("Pooling Number is 1 to 100")
        b = random.randint(1, 100)
        guess_counter = 6
    elif a == str('a'):
        print("Pooling Number is 1 to 1000")
        b = random.randint(1, 1000)
        guess_counter = 3
    elif a == int():
        exit()
    else:
        start()

    guess = False
    while not guess:
        guess_counter -= 1
        if guess_counter == int(-1):
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

        print("Guess the Number...")
        c = int(input("> "))
        if b == str():
            print("You Enter a Wrong Character your Game is Restarted")
            time.sleep(2)
            start()
        elif b == c:
            guess = True
        elif b < c:
            print(f"You are to High!, Guess Count {guess_counter}")
        elif b > c:
            print(f"You are to Low!, Guess Count {guess_counter}")
        else:
            exit()

    if a == str('e'):
        print(f"Congratulation You Guess the Number, The Hidden Number is {b}, Hey Try the Medium one!")
    elif a == str('m'):
        print(f"Congratulation You Guess the Number, The Hidden Number is {b}, Hey Try the Hard one!")
    elif a == str('h'):
        print(f"Congratulation You Guess the Number, The Hidden Number is {b}, Hey don't Forget to Beat the Asian one!")
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
