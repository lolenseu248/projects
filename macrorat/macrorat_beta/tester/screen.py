import cv2
import numpy

from mss import mss


with mss() as sct:
    monitor1={'top':320,'left':900,'width':70,'height':20}
    monitor2={'top':440,'left':828,'width':249,'height':265}
    #monitor2={'top':290,'left':190,'width':285,'height':290}

    while True:
        screen1=numpy.asarray(sct.grab(monitor1))
        screen2=numpy.asarray(sct.grab(monitor2))


        cv2.imshow('screen1',screen1)
        cv2.imshow('screen2',screen2)
                   
        if cv2.waitKey(1) & 0xFF == ord("q"):
            cv2.destroyAllWindows()
            break