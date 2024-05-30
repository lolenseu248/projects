import time
import cv2 as cv
import numpy as np
from mss import mss
from pynput.mouse import Button, Controller as MouseController
from pynput.keyboard import Key, Controller as KeyboardController


#var 
loop_counter=0
click_counter=0

monitor1={'top':320,'left':900,'width':70,'height':20}
monitor2={'top':420,'left':828,'width':249,'height':285}

sct = mss()

lootof_img=cv.imread('img/trigger/lootof.png',cv.IMREAD_UNCHANGED)
template_paths=['img/dump/t4/t4_1.png','img/dump/t4/t4_2.png','img/dump/t4/t4_3.png','img/dump/t4/t4_4.png','img/dump/t4/t4_5.png',
                'img/dump/t5/t5_1.png','img/dump/t5/t5_2.png','img/dump/t5/t5_3.png','img/dump/t5/t5_4.png','img/dump/t5/t5_5.png',
                'img/dump/t6/t6_1.png','img/dump/t6/t6_2.png','img/dump/t6/t6_3.png','img/dump/t6/t6_4.png','img/dump/t6/t6_5.png',
                'img/dump/t7/t7_1.png','img/dump/t7/t7_2.png','img/dump/t7/t7_3.png','img/dump/t7/t7_4.png','img/dump/t7/t7_5.png',
                'img/dump/t8/t8_1.png','img/dump/t8/t8_2.png','img/dump/t8/t8_3.png','img/dump/t8/t8_4.png','img/dump/t8/t8_5.png']

templates=[cv.imread(path) for path in template_paths]
template_grays=[cv.cvtColor(template,cv.COLOR_BGR2GRAY) for template in templates]

match_threshold=0.8

mouse=MouseController()
keyboard=KeyboardController()


#func
def clicker(x,y):
    mouse.position=(x,y)
    time.sleep(.02)
    mouse.press(Button.left)
    time.sleep(.08)
    mouse.release(Button.left)
    time.sleep(.08)

def lootof():
    lootof_screen=np.asarray(sct.grab(monitor1))
    lootof_gray=cv.matchTemplate(lootof_screen,lootof_img,cv.TM_CCOEFF_NORMED)

    # debug
    #cv.imshow('lootofscreen',lootof_screen)
    #cv.imshow('lootofgray',lootof_gray)

    _,max_val,_,max_loc=cv.minMaxLoc(lootof_gray)
    return max_val>=match_threshold

def lootbox():
    global click_counter
    lootbox_screen=np.asarray(sct.grab(monitor2))
    lootbox_gray=cv.cvtColor(lootbox_screen,cv.COLOR_BGR2GRAY)

    # debug
    #cv.imshow('lootboxscreen',lootbox_screen)
    #cv.imshow('lootboxgray',lootbos_gray)

    for template_gray in template_grays:
        result=cv.matchTemplate(lootbox_gray,template_gray,cv.TM_CCOEFF_NORMED)
        point=np.where(result>=match_threshold)

        if point[0].size>0:
            clicker(monitor2['left']+point[1][0]+20,monitor2['top']+point[0][0]+20)
            click_counter+=1
            break


#loop
#time.sleep(5)
while True:
    lootpixelresult=lootof()

    if lootpixelresult:
        keyboard.press(Key.shift)

        while lootpixelresult:
            lootbox()
            lootpixelresult=lootof()
    
        keyboard.release(Key.shift)

    print(f'loop: {loop_counter}, items clicked: {click_counter}', end='\r')
    loop_counter += 1

    if loop_counter == 9:
        loop_counter = 0

    time.sleep(.1)
    if cv.waitKey(1) & 0xFF == ord("q"):
        cv.destroyAllWindows()
        break
