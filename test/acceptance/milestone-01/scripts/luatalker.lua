
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
    end
  end
end

