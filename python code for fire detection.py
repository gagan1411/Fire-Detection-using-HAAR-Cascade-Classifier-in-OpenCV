# -*- coding: utf-8 -*-
"""
Created on Fri Sep  4 23:02:17 2020

@author: HP USER
"""



import numpy as np
import cv2
import serial
import time


fire_cascade = cv2.CascadeClassifier('cascade.xml')
#cascade.xml is the classifier file that contains the parameters of classifier
#checks for fire detection

ser1 = serial.Serial('COM5',9600) #select the COM port number on which arduino is connected


cap = cv2.VideoCapture(0) #start video capturing
count = 0
while cap.isOpened():
    ret, img = cap.read() #capture a frame
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY) #convert image to grayscale
    fire = fire_cascade.detectMultiScale(img, 12, 5) #test for fire detection
    for (x,y,w,h) in fire:
        cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2) #highlight the area of image with fire
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = img[y:y+h, x:x+w]
        print( 'Fire is detected..!' + str(count)) 
        count = count + 1
        ser1.write(str.encode('p')) #write 'p' on serial COM port to arduino
        time.sleep(0.2) #wait
        
    cv2.imshow('img', img)
    ser1.write(str.encode('s')) #write 's' if there is no fire
    k = cv2.waitKey(100) & 0xff
    if k == 27:
       break

ser1.close()
cap.release()
cv2.destroyAllWindows()

