
require 'prompt'

local out = prompt.out
local input = prompt.input

function main (name)
  out.send_message(name..": S'up.");
  while true do
    local msg = input.reeive_message()
    if msg == "bye" then break end
    if msg == "dude" then
      out.send_multimessage(name..": Say what?", 5)
    end
  end
  return true
end

