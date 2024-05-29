import pyautogui

import time

time.sleep(3)


screen=pyautogui.screenshot(region=(828,440,249,265))
screen.save(r"me.png")
