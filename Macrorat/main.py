# https://github.com/lolenseu
# lolen's dev
# MacroRat 1.5

import os
import time
import random
import cv2 as cv
import numpy as np

from mss import mss
from pynput.mouse import Button, Controller as MouseController, Listener as MouseListener
from pynput.keyboard import KeyCode, Key, Controller as KeyboardController, Listener as KeyboardListener


# Functions
def resource_path(relative_path):
    """ Get absolute path to resource, works for dev and for PyInstaller """
    try:
        # PyInstaller creates a temp folder and stores path in _MEIPASS
        base_path = getattr(sys, '_MEIPASS', os.path.dirname(os.path.abspath(__file__)))
    except Exception:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)

def clicker(x, y, i, o):
    mouse.position = (x + 24 + random.randint(1, 24), y + 24 + random.randint(1, 24))
    mouse_x, mouse_y = mouse.position

    if mouse_x < monitor2['left'] or mouse_x >= monitor2['left'] + i or mouse_y < monitor2['top'] or mouse_y >= monitor2['top'] + o:
       pass

    else:
        mouse.press(Button.left)
        time.sleep(.08)
        mouse.release(Button.left)
        time.sleep(.04) # .04 stable / .2 trial

def on_press(key):
    global stop_loot
    if key == Key.esc:
        stop_loot = True

def on_release(key):
    global stop_loot
    if key == Key.esc:
        stop_loot = False

def on_mouse(x,y):
    global mouse_x, mouse_y
    mouse_x, mouse_y = x, y

def lootof():
    lootof_screen = np.asarray(sct.grab(monitor1))
    _lootof = cv.matchTemplate(lootof_screen, lootof_img, cv.TM_CCOEFF_NORMED)

    # debug
    #cv.imshow('lootofscreen', lootof_screen)
    #cv.imshow('lootof', _lootof)

    _, max_val, _, max_loc = cv.minMaxLoc(_lootof)
    return max_val >= match_threshold

def lootbox():
    lootbox_screen = np.asarray(sct.grab(monitor2))
    _lootbox = cv.cvtColor(lootbox_screen, cv.COLOR_BGR2GRAY)

    # debug
    #cv.imshow('lootboxscreen', lootbox_screen)
    #cv.imshow('lootbox', _lootbox)

    for item in item_template:
        template_result = cv.matchTemplate(_lootbox, item, cv.TM_CCOEFF_NORMED)
        min_val, max_val, min_loc, max_loc = cv.minMaxLoc(template_result)
        
        if stop_loot:
            break

        if max_val >= match_threshold:
            item_x, item_y = max_loc[0], max_loc[1]
            item_left, item_top = item_x, item_y
            item_right = min(item_x + 64, _lootbox.shape[1])
            item_bottom = min(item_y + 64, _lootbox.shape[0])
            
            if item_right >= _lootbox.shape[1] or item_bottom >= _lootbox.shape[0]:
                break
            
            else:
                exclude_match_found = False
                item_region = _lootbox[item_top:item_bottom, item_left:item_right]

                for exclude in exclude_template:
                    exclude_result = cv.matchTemplate(item_region, exclude, cv.TM_CCOEFF_NORMED)
                    exclude_min_val, exclude_max_val, _, _ = cv.minMaxLoc(exclude_result)

                    if exclude_max_val >= match_threshold:
                        exclude_match_found = True
                        break

                if not exclude_match_found:
                    x, y = monitor2['left'] + item_x, monitor2['top'] + item_y
                    clicker(x, y, _lootbox.shape[1], _lootbox.shape[0])
                    break


# Variables
mouse_x,mouse_y = 0, 0
loop_counter = 0

match_threshold = 0.8

stop_loot = False

monitor1 = {'top':320, 'left':900, 'width':70, 'height':20}
monitor2 = {'top':405, 'left':825, 'width':260, 'height':310}

# Load images
lootof_img = cv.imread(resource_path('img/trigger/lootof.png'), cv.IMREAD_UNCHANGED)

item_templates = [
    'img/dump/t4/t4_1.png', 'img/dump/t4/t4_2.png', 'img/dump/t4/t4_3.png', 'img/dump/t4/t4_4.png', 'img/dump/t4/t4_5.png',
    'img/dump/t5/t5_1.png', 'img/dump/t5/t5_2.png', 'img/dump/t5/t5_3.png', 'img/dump/t5/t5_4.png', 'img/dump/t5/t5_5.png',
    'img/dump/t6/t6_1.png', 'img/dump/t6/t6_2.png', 'img/dump/t6/t6_3.png', 'img/dump/t6/t6_4.png', 'img/dump/t6/t6_5.png',
    'img/dump/t7/t7_1.png', 'img/dump/t7/t7_2.png', 'img/dump/t7/t7_3.png', 'img/dump/t7/t7_4.png', 'img/dump/t7/t7_5.png',
    'img/dump/t8/t8_1.png', 'img/dump/t8/t8_2.png', 'img/dump/t8/t8_3.png', 'img/dump/t8/t8_4.png', 'img/dump/t8/t8_5.png'
]

exclude_templates = [
    'img/exclude/t3_horse.png', 'img/exclude/t4_stag.png', 'img/exclude/t5_armored_horse.png', 'img/exclude/t5_graywolf.png', 
    'img/exclude/t5_swiftclaw.png', 'img/exclude/t6_wolf.png'
]

trash_templates = [
    'img/trash/empty.png', 'img/trash/t1_trash.png', 'img/trash/t2_trash.png', 'img/trash/t3_trash.png', 'img/trash/t4_trash.png',
    'img/trash/t5_trash.png', 'img/trash/t6_trash.png', 'img/trash/t7_trash.png', 'img/trash/t8_trash.png'
]

exclude_templates.extend(trash_templates)

item_template = [cv.imread(resource_path(path), cv.IMREAD_GRAYSCALE) for path in item_templates]

exclude_template = [cv.imread(resource_path(path), cv.IMREAD_GRAYSCALE) for path in exclude_templates]

sct = mss()

mouse = MouseController()
keyboard = KeyboardController()

keyboard_listener = KeyboardListener(on_press=on_press, on_release = on_release)
mouse_listener = MouseListener(on_mouse = on_mouse)
keyboard_listener.start()
mouse_listener.start()


# Loop
while True:
    try:
        lootpixelresult = lootof()

        if lootpixelresult == True:
            keyboard.press(Key.shift)
            time.sleep(.1)
            
            while lootpixelresult:
                if stop_loot:
                    break
                else:
                    lootbox()
                    lootpixelresult = lootof()
        
            keyboard.release(Key.shift)

        print(f'Press Hold ESC to Pause Looting - Loop: {loop_counter}, Running!', end = '\r')
        if loop_counter == 9:
            loop_counter = 0
        else:
            loop_counter += 1

        time.sleep(.1)
        if cv.waitKey(1) & 0xFF == ord("q"):
            cv.destroyAllWindows()
            keyboard_listener.stop()
            mouse_listener.stop()
            break

    except Exception as e:
        print(f"error occurred: {e}")
        pass
