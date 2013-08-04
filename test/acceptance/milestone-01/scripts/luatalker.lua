
require 'prompt'

local out = prompt.out
local input = prompt.input

function main (name)
  out.send_message(name..": S'up.");
  while true do
    local msg = input.receive_message()
    print("[received '"..msg.."']")
    if msg == "bye" then return true end
    if msg == "dude" then
      out.send_multimessage(name..": Say what?", 5)
    elseif msg == "sqrt" then
      out.send_message(name..": Of...?")
      local arg = input.receive_number()
      out.send_message(name..": 'Tis "..math.sqrt(arg))
    elseif msg == "My favorite color is blue." then
      out.send_message(name..": You sure?")
      local answer = input.receive_confirmation()
      if answer then
        out.send_message(name..": Lame.")
      else
        out.send_message(name..": Indecisive fella.")
      end
    end
  end
end

