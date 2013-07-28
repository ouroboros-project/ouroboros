
require 'prompt'

prompt_token  = ">"
skip_lines    = 2

function respond (line, n)
  if line == "oi" then
    return prompt.message("vc é bobo?")
  else
    return prompt.message("diga oi")
  end
end

