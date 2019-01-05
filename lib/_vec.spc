_vec = cluster [t: type] is create, fetch, store, size, equal, copy,
			    move_lr, move_rl, _gcd

rep = any

create = proc (sz: int) returns (cvt) signals (toobig)
	% number of elements (initially all 0)
	end create

copy = proc (v: cvt) returns (cvt)
	% really copy1
	end copy

fetch = proc (v: cvt, i: int) returns (t) signals (bounds)
	% start index is 1
	end fetch

store = proc (v: cvt, i: int, val: t) signals (bounds)
	% start index is 1
	end store

size = proc (v: cvt) returns (int)
	% number of elements
	end size

equal = proc (v1, v2: cvt) returns (bool)
	% object identity
	end equal

move_lr = proc (v1: cvt, s1: int, v2: cvt, s2: int, len: int)
						signals (negative_size, bounds)
	% move len elements from v1 starting at s1 to v2 starting at s2
	% first element is moved first
	% 1 <= s1 <= size(v1) + 1
	% 1 <= s2 <= size(v2) + 1
	% len >= 0
	end move_lr

move_rl = proc (v1: cvt, s1: int, v2: cvt, s2: int, len: int)
						signals (negative_size, bounds)
	% move len elements from v1 starting at s1 to v2 starting at s2
	% last element is moved first
	% 1 <= s1 <= size(v1) + 1
	% 1 <= s2 <= size(v2) + 1
	% len >= 0
	end move_rl

_gcd = proc (v: cvt, tab: gcd_tab) returns (int)
			where t has _gcd: proctype (t, gcd_tab) returns (int)
	end _gcd

end _vec
