
--require 'prompt'

local out = require 'prompt.out'
local input = require 'prompt.input'

local function nogetter ()
  local test = input.Receiver()
  local check, err = pcall(function () assert(test.sbrubles) end)
  if check == false and err then
    print(err)
  else
    return false
  end
  return true
end

local function nosetter ()
  local test = out.Sender()
  local check, err = pcall(function () test.sbrubles = 42 end)
  if check == false and err then
    print(err)
  else
    return false
  end
  return true
end

function main ()
  local send = out.Sender()
  local receive = input.Receiver()
  send:send_message("Let's check some things first...")
  if not nogetter() or not nosetter() then
    return false
  end
  send:send_message("Everything ok so far. S'up bro.", 'unused');
  send:send_message("Line breakage status is "..tostring(send.break_line), 'unused');
  while true do
    local msg = receive:receive_message()
    print("[received '"..msg.."']")
    if msg == "bye" then return true end
    if msg == "dude" then
      send:send_multimessage("Say what?", 5)
    elseif msg == "sqrt" then
      send:send_message("Of...?")
      local arg = receive:receive_number()
      send:send_message("'Tis "..math.sqrt(arg))
    elseif msg == "My favorite color is blue." then
      send.break_line = false
      send:send_message("You sure? ")
      send.break_line = true
      local answer = receive:receive_confirmation()
      if answer then
        send:send_message("Lame.")
      else
        send:send_message("Indecisive fella.")
      end
    end
  end
end

