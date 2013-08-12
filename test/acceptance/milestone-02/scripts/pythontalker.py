#!/usr/bin/python
# -*- coding: UTF-8 -*-    

from prompt.input import receive_message, receive_number, receive_confirmation
from prompt.out.constants import TALKER_NAME
from prompt import out

def main():
    out.send_message(TALKER_NAME() + ":  WAZAAAAAAAAAP")
    while True:
        msg = receive_message()
        print "message received = [%s]" % (msg)
        if msg in ["adios", "adieu", "arrivederci", "quit", "goodbye", "flw", "falou", "té mais", "té", "bye", "hasta"]:
            break
        elif msg == "hip hip":
            out.send_multimessage(TALKER_NAME()+": HOORAH!", 3)
        elif msg in ["vegeta", "scouter", "nappa"]:
            out.send_message(TALKER_NAME()+": what was Goku's power level reported from Vegeta's scouter?")
            pl = receive_number()
            if pl > 9000.0:
                out.send_message(TALKER_NAME()+": correct.")
            else:
                out.send_message(TALKER_NAME()+": dafuq dude?")
        elif msg == "ask me":
            out.send_message(TALKER_NAME()+": vc é bobo?")
            bobo = receive_confirmation()
            out.send_message(TALKER_NAME()+": "+str(bobo)+"? Hmpf.")
        elif msg == "break":
            out.BREAK_LINE(not out.BREAK_LINE())
            out.send_message(TALKER_NAME()+": toggled break line - "+str(out.BREAK_LINE()))
        elif msg in ["help", "h"]:
            out.send_message(TALKER_NAME()+": 'hip hip', 'scouter', 'ask me', 'break', 'quit'")
        
    return True
