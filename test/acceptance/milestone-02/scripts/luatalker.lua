
--require 'prompt'

local out = require 'prompt.out'
local input = require 'prompt.input'

local function wrongnumtest ()
  local check, err = pcall(function () out.send_message() end)
  if check == false and err then
    print(err)
  else
    return false
  end
  return true
end

local function wrongtypetest ()
  local check, err = pcall(function () out.send_multimessage('derp', function() end) end)
  if check == false and err then
    print(err)
  else
    return false
  end
  return true
end

function main ()
  local name = out.constants.TALKER_NAME
  print(name..": Let's check some things first...")
  if not wrongnumtest() or not wrongtypetest() then
    return false
  end
  out.send_message(name..": Everything ok so far. S'up bro.", 'unused');
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
      out.BREAK_LINE = false
      out.send_message(name..": You sure? ")
      local answer = input.receive_confirmation()
      out.BREAK_LINE = true
      if answer then
        out.send_message(name..": Lame.")
      else
        out.send_message(name..": Indecisive fella.")
      end
    end
  end
end

