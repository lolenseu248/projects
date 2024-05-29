import cv2 as cv
import numpy as np
from mss import mss




with mss() as sct:

    Winname = "Frame:"

    def nothing(x):
        pass

    cv.namedWindow('Frame:')
    # H, S,V are for Lower Boundaries
    #H2,S2,V2 are for Upper Boundaries
    cv.createTrackbar('H',Winname,0,255,nothing)
    cv.createTrackbar('S',Winname,0,255,nothing)
    cv.createTrackbar('V',Winname,0,255,nothing)
    cv.createTrackbar('H2',Winname,0,255,nothing)
    cv.createTrackbar('S2',Winname,0,255,nothing)
    cv.createTrackbar('V2',Winname,0,255,nothing)

    


    while True:
        monitor1={'top':50,'left':50,'width':500,'height':500}
        cap=np.asarray(sct.grab(monitor1))

        H = cv.getTrackbarPos('H', 'Frame:')
        S = cv.getTrackbarPos('S', 'Frame:')
        V = cv.getTrackbarPos('V', 'Frame:')
        H2 = cv.getTrackbarPos('H2', 'Frame:')
        S2 = cv.getTrackbarPos('S2', 'Frame:')
        V2 = cv.getTrackbarPos('V2', 'Frame:')
        hsv = cv.cvtColor(cap, cv.COLOR_BGR2HSV)
        lower_boundary = np.array([H, S, V])
        upper_boundary = np.array([H2,S2,V2])
        mask = cv.inRange(hsv, lower_boundary, upper_boundary)
        cv.imshow("screen:", cap)
        cv.imshow("mask:", mask)

        if cv.waitKey(1) == ord('q'): break

cv.destroyAllWindows()