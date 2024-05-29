import sys
import cv2
import time
import numpy
import pyautogui

from PIL import ImageGrab
from mss import mss

#time.sleep(5)
#func
def click(t,l,x,y):
    #pyautogui.keyDown('shift')
    pyautogui.click(t+x,l+y)
    #pyautogui.keyUp('shift')


with mss() as sct:
    #var
    loopcounter=0

    lootofmonitor={'top':320,'left':900,'width':70,'height':20}
    itemtargetscreen=(828,440,249,265)

    lootofimg=cv2.imread('imgdump/lootof.png',cv2.IMREAD_UNCHANGED)

    T4item=(77,113,136)
    t5item=(134,61,54)
    t6item=(212,123,56)
    t7item=(214,185,90)
    t8item=(239,239,239)

    #loop
    while True:
        lootpixelresult=False

        lootofscreen=numpy.asarray(sct.grab(lootofmonitor))
        lootofresult=cv2.matchTemplate(lootofscreen,lootofimg,cv2.TM_CCOEFF_NORMED)

        # debug
        #cv2.imshow('lootofscreen',lootofscreen)
        #scv2.imshow('lootofresult',lootofresult)

        for x in range(0,lootofresult.shape[0]):
            for y in range(0,lootofresult.shape[1]):
                lootpixel=int(lootofresult[x,y])

                if lootpixel==True:
                    lootpixelresult=True
        
        if lootpixelresult==True:
            itemscreen=pyautogui.screenshot(region=itemtargetscreen)
            width,height=itemscreen.size

            for x in range(0,width,1):
                for y in range(0,height,1):
                    r,g,b=itemscreen.getpixel((x,y))

                    if(g in range(105,115)):
                        click(itemtargetscreen[0],itemtargetscreen[1],x,y)    

                    elif(g in range(55,65)):
                        click(itemtargetscreen[0],itemtargetscreen[1],x,y)

                    elif(g in range(120,130)):
                        click(itemtargetscreen[0],itemtargetscreen[1],x,y)
                        
                    elif(g in range(175,190)):
                        click(itemtargetscreen[0],itemtargetscreen[1],x,y)

                    elif(g in range(235,245)):
                        click(itemtargetscreen[0],itemtargetscreen[1],x,y)
                
        else:
            pass
            
        loopcounter+=1
        sys.stdout.write('\r')
        sys.stdout.write(f'clock {loopcounter} lootp {lootpixel}')
        sys.stdout.flush()

        if loopcounter==60:
            loopcounter=0


        #time.sleep(1) #debug
        if cv2.waitKey(1)&0xFF==ord("q"):
            cv2.destroyAllWindows()
            break
