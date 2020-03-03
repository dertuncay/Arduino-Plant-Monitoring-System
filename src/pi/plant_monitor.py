import serial
import time
import sys

sensors = [0, 1, 2, 3, 4, 5, 6]

delay = 60

ar = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
print('Connected on: {}'.format(ar.name), file=sys.stderr)
print('Monitoring every {} seconds'.format(delay), file=sys.stderr)
    
try:
    while True:
        readings = []
        for sensor in sensors:
            response = ''
            while response == '':
                ar.write(str(sensor).encode())
                time.sleep(0.2)
                response = ar.readline().decode().strip()
            readings.append(response)
        with open('log.txt', 'a+') as f:
            print(int(time.time()), *readings, file=f)
        time.sleep(delay)
except KeyboardInterrupt:
    print('Quitting', file=sys.stderr)
