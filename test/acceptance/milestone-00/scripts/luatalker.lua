
require 'prompt'

prompt_token  = ">"
skip_lines    = 2

function respond (line, n)
  if line == "oi" then
    return prompt.print("vc é bobo?")
  else
    return prompt.print("diga oi")
  end
end

