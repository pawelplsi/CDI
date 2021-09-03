import serial
from datetime import datetime

def log(s):
    print(f"{datetime.now().strftime('%H:%M:%S:%f')} {s}")

with serial.Serial('/dev/ttyUSB1', 38400, timeout = None) as s:
    while True:
        b = s.read()[0]
        if chr(b) == 'A':
            log("Restart")
        elif chr(b) == 'B':

            log("B")
        elif chr(b) == 'G':
            log("G")
        elif chr(b) == 'H':
            log("timeout")

        elif chr(b) == 'T':
            b0 = s.read()[0]
            b1 = s.read()[0]
            b2 = s.read()[0]
            b3 = s.read()[0]
            t = b0*256+b1
            ch = b2*256+b3
            if t!=0:
                rpm = 8437500 // t
            else:
                rpm = 0
            log(f"T: {t}, RPM: {rpm} CH: {ch}")

        elif chr(b) == 'O':
            b = s.read()[0]
            log(f"O: {b}")
        # # if chr(b)=='M':
        #     # print('dupa')
        #     # continue
        # # print(chr(b), end="");
        # pressed = not bool( b & B )
        # b = b&127
        # pitch = b+48
        # note_on = [0x90, pitch, 112]
        # note_off = [0x80, pitch, 0]
        # print(str(b)+': '+str(pressed))
        # if pressed:
        #     midiout.send_message(note_on)
        # else:
        #     midiout.send_message(note_off)
