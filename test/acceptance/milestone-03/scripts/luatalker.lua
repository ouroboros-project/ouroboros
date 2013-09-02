
--require 'prompt'

local out = require 'prompt.out'
local input = require 'prompt.input'

--local function wrongnumtest ()
--  local check, err = pcall(function () out.send.message() end)
--  if check == false and err then
--    print(err)
--  else
--    return false
--  end
--  return true
--end
--
--local function wrongtypetest ()
--  local check, err = pcall(function () out.send_multimessage('derp', function() end) end)
--  if check == false and err then
--    print(err)
--  else
--    return false
--  end
--  return true
--end

function main ()
  local send = out.Sender()
  local receive = input.Receiver()
  send:send_message("Let's check some things first...")
  --if not wrongnumtest() or not wrongtypetest() then
  --  return false
  --end
  send:send_message("Everything ok so far. S'up bro.", 'unused');
  while true do
    local msg = receive:receive_message()
    print("[received '"..msg.."']")
    if msg == "bye" then return true end
    if msg == "dude" then
      send:send_multimessage("Say what?", 5)
    elseif msg == "sqrt" then
      send:send_message("Of...?")
      local arg = receive:number()
      send:send_message("'Tis "..math.sqrt(arg))
    elseif msg == "My favorite color is blue." then
      send.break_line = false
      send:send_message("You sure? ")
      send.break_line = true
      local answer = receive:confirmation()
      if answer then
        send:send_message("Lame.")
      else
        send:send_message("Indecisive fella.")
      end
    end
  end
end

