#!/usr/bin/python
# -*- coding: UTF-8 -*-    

from prompt.input import Receiver
import prompt

def main():
    rec = Receiver()
    out = prompt.out.Sender()

    out.send_message("WAZAAAAAAAAAP")
    while True:
        msg = rec.receive_message()
        print "message received = [%s]" % (msg)
        if msg in ["adios", "adieu", "arrivederci", "quit", "goodbye", "flw", "falou", "té mais", "té", "bye", "hasta"]:
            break
        elif msg == "hip hip":
            out.send_multimessage("HOORAH!", 3)
        elif msg in ["vegeta", "scouter", "nappa"]:
            out.send_message("what was Goku's power level reported from Vegeta's scouter?")
            pl = rec.receive_number()
            if pl > 9000.0:
                out.send_message("correct.")
            else:
                out.send_message("dafuq dude?")
        elif msg == "ask me":
            out.send_message("vc é bobo?")
            bobo = rec.receive_confirmation()
            out.send_message(str(bobo)+"? Hmpf.")
        elif msg == "break":
            out.break_line = not out.break_line
            out.send_message("toggled break line - "+str(out.break_line))
        elif msg in ["help", "h"]:
            out.send_message(" 'hip hip', 'scouter', 'ask me', 'break', 'quit'")
        
    return True
