import time
import cv2 as cv
import numpy as np
from mss import mss
from pynput.mouse import Button, Controller as MouseController
from pynput.keyboard import Key, Controller as KeyboardController


#var 
loop_counter=0
click_counter=0
clicked=False

monitor1={'top':320,'left':900,'width':70,'height':20}
monitor2={'top':420,'left':828,'width':249,'height':285}

sct = mss()

lootof_img=cv.imread('img/trigger/lootof.png',cv.IMREAD_UNCHANGED)
item_templates=['img/dump/t4/t4_1.png','img/dump/t4/t4_2.png','img/dump/t4/t4_3.png','img/dump/t4/t4_4.png','img/dump/t4/t4_5.png',
                'img/dump/t5/t5_1.png','img/dump/t5/t5_2.png','img/dump/t5/t5_3.png','img/dump/t5/t5_4.png','img/dump/t5/t5_5.png',
                'img/dump/t6/t6_1.png','img/dump/t6/t6_2.png','img/dump/t6/t6_3.png','img/dump/t6/t6_4.png','img/dump/t6/t6_5.png',
                'img/dump/t7/t7_1.png','img/dump/t7/t7_2.png','img/dump/t7/t7_3.png','img/dump/t7/t7_4.png','img/dump/t7/t7_5.png',
                'img/dump/t8/t8_1.png','img/dump/t8/t8_2.png','img/dump/t8/t8_3.png','img/dump/t8/t8_4.png','img/dump/t8/t8_5.png']

exclude_templates=['img/exclude/t3_horse.png','img/exclude/t4_stag.png','img/exclude/t5_armored_horse.png','img/exclude/t5_graywolf.png','img/exclude/t5_swiftclaw.png','img/exclude/t6_wolf.png']
trash_templates=['img/trash/t1_trash.png','img/trash/t2_trash.png','img/trash/t3_trash.png','img/trash/t4_trash.png','img/trash/t5_trash.png','img/trash/t6_trash.png','img/trash/t7_trash.png','img/trash/t8_trash.png',]

exclude_templates.extend(trash_templates)

item_template=[cv.imread(path) for path in item_templates]
gray_item_template=[cv.cvtColor(template,cv.COLOR_BGR2GRAY) for template in item_template]

exclude_template=[cv.imread(path) for path in exclude_templates]
gray_exclude_template=[cv.cvtColor(template,cv.COLOR_BGR2GRAY) for template in exclude_template]

match_threshold=0.8

mouse=MouseController()
keyboard=KeyboardController()


#func
def clicker(x,y):
    mouse.position=(x,y)
    time.sleep(.05)
    mouse.press(Button.left)
    time.sleep(.1)
    mouse.release(Button.left)
    time.sleep(.1)

def lootof():
    lootof_screen=np.asarray(sct.grab(monitor1))
    lootof_gray=cv.matchTemplate(lootof_screen,lootof_img,cv.TM_CCOEFF_NORMED)

    # debug
    #cv.imshow('lootofscreen',lootof_screen)
    #cv.imshow('lootofgray',lootof_gray)

    _,max_val,_,max_loc=cv.minMaxLoc(lootof_gray)
    return max_val>=match_threshold

def lootbox():
    global click_counter,clicked
    lootbox_screen=np.asarray(sct.grab(monitor2))
    lootbox_gray=cv.cvtColor(lootbox_screen,cv.COLOR_BGR2GRAY)

    # debug
    #cv.imshow('lootboxscreen',lootbox_screen)
    #cv.imshow('lootboxgray',lootbox_gray)

    for gray_item in gray_item_template:
        template_result=cv.matchTemplate(lootbox_gray,gray_item,cv.TM_CCOEFF_NORMED)
        template_points=np.where(template_result>=match_threshold)

        if template_points[0].size>0:
            for point in zip(*template_points[::-1]):
                item_x,item_y=point
                item_left,item_top=item_x,item_y
                item_right=min(item_x+50,lootbox_gray.shape[1])
                item_bottom=min(item_y+50,lootbox_gray.shape[0])
                item_region=lootbox_gray[item_top:item_bottom,item_left:item_right]

                exclude_match_found=False
                for gray_exclude in gray_exclude_template:
                    exclude_result=cv.matchTemplate(item_region,gray_exclude,cv.TM_CCOEFF_NORMED)
                    exclude_points=np.where(exclude_result>=match_threshold)

                    if exclude_points[0].size>0:
                        exclude_match_found=True
                        break

                if not exclude_match_found:
                    clicker(monitor2['left']+item_x+25,monitor2['top']+item_y+25)
                    clicked=True
                    click_counter+=1
                    break

        if clicked:
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

            if clicked:
                clicked=False
    
        keyboard.release(Key.shift)

    print(f'loop: {loop_counter}, items clicked: {click_counter}',end='\r')
    if loop_counter==9:
        loop_counter=0

    else:
        loop_counter+=1

    time.sleep(.1)
    if cv.waitKey(1) & 0xFF == ord("q"):
        cv.destroyAllWindows()
        break
