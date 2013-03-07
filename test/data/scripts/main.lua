
print "============ HEY FROM LUA"

--- Lists the contents of a table to the standard output.
-- 
function ls(t)
  for k,v in pairs(t) do
    print(k,v)
  end
end

print = print

str = "AMAGAD LUA STRING"
bool = true
integer = 42
number = math.pi
print "============ BYE FROM LUA"

