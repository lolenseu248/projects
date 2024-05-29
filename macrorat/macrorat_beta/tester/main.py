import sys
import cv2
import time
import numpy
import pyautogui
import keyboard

from PIL import ImageGrab
from mss import mss

#time.sleep(5)
#func



with mss() as sct:
    #var
    loopcounter=0

    monitor1={'top':320,'left':900,'width':70,'height':20}
    monitor2={'top':440,'left':828,'width':249,'height':265}
    #monitor2={'top':290,'left':190,'width':285,'height':290}


    lootofimg=cv2.imread('imgdump/lootof.png',cv2.IMREAD_UNCHANGED)

    itemf=cv2.imread('imgdump/t4.png',cv2.IMREAD_UNCHANGED)
    itemu=cv2.imread('imgdump/t5.png',cv2.IMREAD_UNCHANGED)
    itemlist=[itemf,itemu,]

    #loop
    while True:import sys
import cv2
import time
import numpy
import pyautogui
import keyboard

from PIL import ImageGrab
from mss import mss

#time.sleep(5)
#func



with mss() as sct:
    #var
    loopcounter=0

    monitor1={'top':320,'left':900,'width':70,'height':20}
    monitor2={'top':440,'left':828,'width':249,'height':265}
    #monitor2={'top':290,'left':190,'width':285,'height':290}


    lootofimg=cv2.imread('imgdump/lootof.png',cv2.IMREAD_UNCHANGED)

    t4item=cv2.imread('imgdump/t4.png',cv2.IMREAD_UNCHANGED)
    t5item=cv2.imread('imgdump/t5.png',cv2.IMREAD_UNCHANGED)
    t6item=cv2.imread('imgdump/t6.png',cv2.IMREAD_UNCHANGED)
    t7item=cv2.imread('imgdump/t7.png',cv2.IMREAD_UNCHANGED)
    t8item=cv2.imread('imgdump/t8.png',cv2.IMREAD_UNCHANGED)
    itemlist=[t4item,t5item,t6item,t7item,t8item]

    #loop
    while True:
        lootpixelresult=True

        lootofscreen=numpy.asarray(sct.grab(monitor1))
        lootofresult=cv2.matchTemplate(lootofscreen,lootofimg,cv2.TM_CCOEFF_NORMED)

        # debug
        #cv2.imshow('lootofscreen',lootofscreen)
        #cv2.imshow('lootofresult',lootofresult)

        for x in range(0,lootofresult.shape[0]):
            for y in range(0,lootofresult.shape[1]):
                lootpixel=int(lootofresult[x,y])

                if lootpixel==True:
                    lootpixelresult=True
                    
        if lootpixelresult==True:
            items=False
            itemscreen=numpy.asarray(sct.grab(monitor2))

            itemscreens=[]
            for i in range(len(itemlist)):
                itemscreens.append(cv2.matchTemplate(itemscreen,itemlist[i],cv2.TM_CCOEFF_NORMED))

            for i in range(len(itemscreens)):
                #cv2.imshow(f'itemscreen{i}',itemscreen) #debug
                cv2.imshow(f'itemscreens{i}',itemscreens[i]) #debug

                if items==False:
                    for x in range(0,itemscreen[i].shape[0]):
                        for y in range(0,itemscreen[i].shape[1]):
                            itempixel=int(itemscreen[i][x,y])

                            if itempixel==True:
                                items=True
                                print(itempixel)
                                #pyautogui.keyDown('shift')
                                #pyautogui.click(monitor2['left']+x,monitor2['top']+y)
                                #pyautogui.keyUp('shift')
                                
                            else:
                                pass

                else:
                    pass

        loopcounter+=1
        sys.stdout.write('\r')
        sys.stdout.write(f'clock {loopcounter}')
        sys.stdout.flush()

        if loopcounter==60:
            loopcounter=0


        #time.sleep(1) #debug
        if cv2.waitKey(1)&0xFF==ord("q"):
            cv2.destroyAllWindows()
            break
        lootpixelresult=True

        lootofscreen=numpy.asarray(sct.grab(monitor1))
        lootofresult=cv2.matchTemplate(lootofscreen,lootofimg,cv2.TM_CCOEFF_NORMED)

        # debug
        #cv2.imshow('lootofscreen',lootofscreen)
        #cv2.imshow('lootofresult',lootofresult)

        for x in range(0,lootofresult.shape[0]):
            for y in range(0,lootofresult.shape[1]):
                lootpixel=int(lootofresult[x,y])

                if lootpixel==True:
                    lootpixelresult=True
                    
        if lootpixelresult==True:
            items=False
            itemscreen=numpy.asarray(sct.grab(monitor2))

            itemscreens=[]
            for i in range(len(itemlist)):
                itemscreens.append(cv2.matchTemplate(itemscreen,itemlist[i],cv2.TM_CCOEFF_NORMED))

            for i in range(len(itemscreens)):
                #cv2.imshow(f'itemscreen{i}',itemscreen) #debug
                cv2.imshow(f'itemscreens{i}',itemscreens[i]) #debug

                if items==False:
                    for x in range(0,itemscreen[i].shape[0]):
                        for y in range(0,itemscreen[i].shape[1]):
                            itempixel=int(itemscreen[i][x,y])

                            if itempixel==True:
                                items=True
                                print(itempixel)
                                #pyautogui.keyDown('shift')
                                #pyautogui.click(monitor2['left']+x,monitor2['top']+y)
                                #pyautogui.keyUp('shift')
                                
                            else:
                                pass

                else:
                    pass

        loopcounter+=1
        sys.stdout.write('\r')
        sys.stdout.write(f'clock {loopcounter}')
        sys.stdout.flush()

        if loopcounter==60:
            loopcounter=0


        #time.sleep(1) #debug
        if cv2.waitKey(1)&0xFF==ord("q"):
            cv2.destroyAllWindows()
            break