import prompt

prompt_token = "&"
skip_lines = 1

def respond(line, n=0):
    if line == "vc é bobo?":
        prompt.print("Claro! ZUEI =D")
    else:
        numP = len(line.split())
        prompt.print("plóc "*numP)
    return True

