require 'lpeg'
local P, C, V, Cs = lpeg.P, lpeg.C, lpeg.V, lpeg.Cs

function DOSTRING (string)
    assert(loadstring(string))()
    return ''
end

function CALL (f, string)
    return assert(_G[f], 'undefined function "'..f..'"')(string)
end

G = {
    [1] = Cs((V'Require' + V'Call' + P(1))^0)
,
    Require = (('@[[' * C((P(1)-']]')^0) * ']]')
                    / DOSTRING)
,
    Call    = (('@' * C((P(1)-'[['-'\n')^0) * '[[' * C((P(1)-']]')^0) * ']]')
                    / CALL)
}

local FILE = assert(io.open(...)):read'*a'
print( P(G):match(FILE) )
