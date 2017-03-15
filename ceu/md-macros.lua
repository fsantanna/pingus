lpeg = require 'lpeg'
local P, C, V, S, Ct, Cs = lpeg.P, lpeg.C, lpeg.V, lpeg.S, lpeg.Ct, lpeg.Cs

function DOSTRING (string)
    assert(load(string))()
    return ''
end

function CALL (f, t)
    if type(t) == 'string' then
        t = { t }
    end
    return assert(_G[f], 'undefined function "'..f..'"')(table.unpack(t))
end

local SPC = S' \n'^0

G = {
    [1] = Cs((V'Require' + V'CallP' + V'CallB' + P(1))^0)
,
    Require = (('@[[' * C((P(1)-']]')^0) * ']]')
                    / DOSTRING)
,
    CallP   = (('@' * C((P(1)-'('-'[['-'\n')^0) * '(' * V'Args' * ')')
                    / CALL)
,
    CallB   = (('@' * C((P(1)-'[['-'('-'\n')^0) * '[[' * C((P(1)-']]')^0) * ']]')
                    / CALL)
,
    Args    = Ct( C((1-S',)')^0) * (','*SPC * C((1-S',)')^0))^0 )
}

local FILE = assert(io.open(...)):read'*a'
print( P(G):match(FILE) )
