_tagcell = cluster [t: type] is create, get, set, set_value, copy, equal, _gcd

rep = any

create = proc (tg: int, val: t) returns (cvt) signals (toobig)
	% create tagged cell
	end create

copy = proc (tt: cvt) returns (cvt)
	% actually copy1
	end copy

get = proc (tt: cvt) returns (int, t)
	% return tag and value
	end get

set = proc (tt: cvt, tg: int, val: t) signals (toobig)
	% set tag and value
	end set

set_value = proc (tt: cvt, val: t)
	% set value
	end set_value

equal = proc (t1, t2: cvt) returns (bool)
	% object identity
	end equal

_gcd = proc (tt: cvt, tab: gcd_tab) returns (int)
			where t has _gcd: proctype (t, gcd_tab) returns (int)
	end _gcd

end _tagcell
