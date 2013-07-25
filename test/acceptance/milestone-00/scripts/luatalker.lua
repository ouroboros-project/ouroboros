
require 'prompt'

prompt_token  = ">"
skip_lines    = 2

function respond (line, n)
  if line == "oi" then
    prompt.print("vc é bobo?")
  --else
  --  prompt.print("diga oi")
  end
  return true
end

