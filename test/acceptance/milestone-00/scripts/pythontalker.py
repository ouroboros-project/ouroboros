#!/usr/bin/python
# -*- coding: UTF-8 -*-
import prompt

prompt_token = "&"
skip_lines = 1

def respond(line, n=0):
    if line == "vc é bobo?":
        prompt.message("Claro! ZUEI =D")
    else:
        numP = len(line.split())
        prompt.message("plóc "*numP)
    return True

