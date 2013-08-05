#!/usr/bin/python
# -*- coding: UTF-8 -*-
import prompt
from prompt import out

def main(name):
    out.send_message(name + ":  WAZAAAAAAAAAP")
    while True:
        msg = prompt.input.receive_message()
        print "message received = [%s]" % (msg)
        if msg in ["adios", "adieu", "arrivederci", "quit", "goodbye", "flw", "falou", "té mais", "té", "bye", "hasta"]:
            break
        elif msg == "hip hip":
            prompt.out.send_multimessage(name+": HOORAH!", 3)
        elif msg in ["vegeta", "scouter", "nappa"]:
            prompt.out.send_message(name+": what was Goku's power level reported from Vegeta's scouter?")
            pl = prompt.input.receive_number()
            if pl > 9000.0:
                prompt.out.send_message(name+": correct.")
            else:
                prompt.out.send_message(name+": dafuq dude?")
        elif msg == "ask me":
            prompt.out.send_message(name+": vc é bobo?")
            bobo = prompt.input.receive_confirmation()
            prompt.out.send_message(name+": "+str(bobo)+"? Hmpf.")
        
    return True
