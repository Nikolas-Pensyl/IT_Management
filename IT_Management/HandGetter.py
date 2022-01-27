import random

suits = ["spades", "hearts", "diamonds", "clubs"]
count  = input("Input the number of cards you want dealt to yourself: ")
cards = []

def getCard(num):
    card = ""
    number = num%13
    card = number
    if(number==0):
        card = "King"
    elif(number==12):
        card = "Queen"
    elif(number==11):
        card = "Jack"
    card += " of " + suits[(num%52)/4]
    return card
    

def getDup(num):
    for j in cards:
            if num is j:
                return True
    return False

while not count.isnumeric:
    print("Invalid intput try agian\n")
    count  = input("Input the number of cards you want dealt to yourself: ")

decks = (count/52).floor() + 1
print("{0} decks will be used to deal".format(decks))
for i in range(count):
    num = random.random()*52*decks
    while getDup(num):
        num = random.random()*52*decks
    cards.append(num)

k = 0
for i in cards:
    k +=1
    print("Card #{0} is the: {1}" .format(k, getCard(i)))