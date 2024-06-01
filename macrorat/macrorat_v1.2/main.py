import time
import random
import cv2 as cv
import numpy as np

from mss import mss
from pynput.mouse import Button,Controller as MouseController
from pynput.keyboard import Listener,KeyCode,Key,Controller as KeyboardController


#func
def clicker(x,y):
    global previous_x,previous_y
    if previous_x==x and previous_y==y:
        delay=.1
    else:
        delay=.05

    mouse.position=(x+24+random.randint(1,24),y+24+random.randint(1,24))
    time.sleep(.05)
    mouse.press(Button.left)
    time.sleep(delay)
    mouse.release(Button.left)
    time.sleep(.1)

    previous_x=x
    previous_y=y

def on_press(key):
    global stop_loot
    if key==KeyCode.from_char('x') or key==Key.esc:
        stop_loot=True

def on_release(key):
    global stop_loot
    if key==KeyCode.from_char('x') or key==Key.esc:
        stop_loot=False

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
    #cv.imshow('lootboxgray',lootbox_gray)

    for gray_item in gray_item_template:
        template_result=cv.matchTemplate(lootbox_gray,gray_item,cv.TM_CCOEFF_NORMED)
        min_val,max_val,min_loc,max_loc=cv.minMaxLoc(template_result)

        if max_val>=match_threshold:
            item_x,item_y=max_loc[0],max_loc[1]
            item_left,item_top=item_x,item_y
            item_right=min(item_x+64,lootbox_gray.shape[1])
            item_bottom=min(item_y+64,lootbox_gray.shape[0])

            if item_right>=lootbox_gray.shape[1] or item_bottom>=lootbox_gray.shape[0]:
                break

            item_region=lootbox_gray[item_top:item_bottom,item_left:item_right]
            exclude_match_found=False
            
            for gray_exclude in gray_exclude_template:
                exclude_result=cv.matchTemplate(item_region,gray_exclude,cv.TM_CCOEFF_NORMED)
                exclude_min_val,exclude_max_val,_,_=cv.minMaxLoc(exclude_result)

                if exclude_max_val>=match_threshold:
                    exclude_match_found=True
                    break

            if not exclude_match_found:
                clicker(monitor2['left']+item_x,monitor2['top']+item_y)
                click_counter+=1
                break


#var
previous_x=0
previous_y=0
click_counter=0
loop_counter=0

match_threshold=0.8

stop_loot=False

monitor1={'top':320,'left':900,'width':70,'height':20}
monitor2={'top':420,'left':828,'width':249,'height':285}

lootof_img=cv.imread('img/trigger/lootof.png',cv.IMREAD_UNCHANGED)

item_templates=['img/dump/t4/t4_1.png','img/dump/t4/t4_2.png','img/dump/t4/t4_3.png','img/dump/t4/t4_4.png','img/dump/t4/t4_5.png',
                'img/dump/t5/t5_1.png','img/dump/t5/t5_2.png','img/dump/t5/t5_3.png','img/dump/t5/t5_4.png','img/dump/t5/t5_5.png',
                'img/dump/t6/t6_1.png','img/dump/t6/t6_2.png','img/dump/t6/t6_3.png','img/dump/t6/t6_4.png','img/dump/t6/t6_5.png',
                'img/dump/t7/t7_1.png','img/dump/t7/t7_2.png','img/dump/t7/t7_3.png','img/dump/t7/t7_4.png','img/dump/t7/t7_5.png',
                'img/dump/t8/t8_1.png','img/dump/t8/t8_2.png','img/dump/t8/t8_3.png','img/dump/t8/t8_4.png','img/dump/t8/t8_5.png']

exclude_templates=['img/exclude/t3_horse.png','img/exclude/t4_stag.png','img/exclude/t5_armored_horse.png','img/exclude/t5_graywolf.png','img/exclude/t5_swiftclaw.png','img/exclude/t6_wolf.png']
trash_templates=['img/trash/empty.png','img/trash/t1_trash.png','img/trash/t2_trash.png','img/trash/t3_trash.png','img/trash/t4_trash.png','img/trash/t5_trash.png','img/trash/t6_trash.png','img/trash/t7_trash.png','img/trash/t8_trash.png']


exclude_templates.extend(trash_templates)

item_template=[cv.imread(path) for path in item_templates]
gray_item_template=[cv.cvtColor(template,cv.COLOR_BGR2GRAY) for template in item_template]

exclude_template=[cv.imread(path) for path in exclude_templates]
gray_exclude_template=[cv.cvtColor(template,cv.COLOR_BGR2GRAY) for template in exclude_template]

sct=mss()

mouse=MouseController()
keyboard=KeyboardController()

listener=Listener(on_press=on_press,on_release=on_release)
listener.start()


#loop
while True:
    try:
        lootpixelresult=lootof()

        if lootpixelresult:
            keyboard.press(Key.shift)

            while lootpixelresult and not stop_loot:
                lootbox()
                lootpixelresult=lootof()

            keyboard.release(Key.shift)

        print(f'loop: {loop_counter}, items clicked: {click_counter}',end='\r')
        if loop_counter==9:
            loop_counter=0

        else:
            loop_counter+=1

        time.sleep(.1)
        if cv.waitKey(1)&0xFF==ord("q"):
            cv.destroyAllWindows()
            listener.stop()
            break

    except Exception as e:
        print(f"error occurred: {e}")
        pass
