# generate data like this
# time,roll,pitch,yaw,altitude,battery,load
# 0.0,1.0,2.5,0.3
# 1.0,2.0,3.1,0.2
# 2.0,3.0,3.7,0.1
import csv
import time
from datetime import datetime
import random
n = 1000
for i in range(n):
    with open('data/test.csv', 'a', newline='') as csvfile:
        writer = csv.writer(csvfile)
        if i == 0:
            writer.writerow(['time', 'roll', 'pitch', 'yaw', 'altitude', 'battery', 'load'])
        t = time.time()
        roll = i * 1.0
        pitch = i * 2.5
        yaw = i * 0.3
        if i <n/2:
            altitude = i * 100 
            load = 2
        elif i >n-n*0.1:
            pitch = pitch + random.uniform(-100, 100)  # add some noise to pitch
        else:
            altitude = n/2 *100 
            load = 0
        battery = 100 - i * n/100
        writer.writerow([t, roll, pitch, yaw, altitude, battery, load])
    
    time.sleep(0.01)  # simulate time delay for each entry