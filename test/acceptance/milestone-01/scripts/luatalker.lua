
require 'prompt'

function main (name)
  prompt.out.send_message(name..": S'up.");
  while true do
    local msg = prompt.input.reeive_message()
    if msg == "bye" then break end
    if msg == "dude" then
      prompt.send_multimessage(name..": Say what?", 5)
    end
  end
  return true
end

