_bytevec = cluster is create, fetch, store, size, equal,
		      indexc, indexv, nc_indexv, comp, copy,
		      move_lr, move_rl, move_v, move_b, build, _gcd

rep = any

create = proc (sz: int) returns (cvt) signals (toobig)
	% number of chars  (all initially '\000')
	end create

copy = proc (v: cvt) returns (cvt)
	% really copies
	end copy

fetch = proc (v: cvt, i: int) returns (char) signals (bounds)
	% start index is 1
	end fetch

store = proc (v: cvt, i: int, c: char) signals (bounds)
	% start index is 1
	end store

size = proc (v: cvt) returns (int)
	% number of chars
	end size

equal = proc (v1, v2: cvt) returns (bool)
	% object identity
	end equal

indexc = proc (c: char, v: cvt, first: int) returns (int) signals (bounds)
	% MIN{i | i >= first & v[i] = c}
	% returns 0 if no match
	end indexc

indexv = proc (v1: cvt, v2: cvt, first: int) returns (int) signals (bounds)
	% MIN{i | i >= first & v1[j] = v2[i - 1 + j] for all j}
	% returns 0 if no match
	end indexv

nc_indexv = proc (v1: cvt, v2: cvt, first: int) returns (int) signals (bounds)
	% same as indexv but case is ignored
	end nc_indexv

comp = proc (v1: cvt, v2: cvt) returns (int)
	% returns -1 if v1 < v2
	%	   0 if v1 = v2		(char comparison, not object identity)
	%	  +1 if v1 > v2
	end comp

move_lr = proc (v1: cvt, s1: int, v2: cvt, s2: int, len: int)
						signals (negative_size, bounds)
	% moves len chars from v1 starting at s1 to v2 starting at s2
	% first char is moved first
	% 1 <= s1 <= size(v1) + 1
	% 1 <= s2 <= size(v2) + 1
	% len >= 0
	end move_lr

move_rl = proc (v1: cvt, s1: int, v2: cvt, s2: int, len: int)
						signals (negative_size, bounds)
	% moves len chars from v1 starting at s1 to v2 starting at s2
	% last char is moved first
	% 1 <= s1 <= size(v1) + 1
	% 1 <= s2 <= size(v2) + 1
	% len >= 0
	end move_rl

move_v = proc (v1: _vec[char], s1: int, v2: cvt, s2: int, len: int)
						signals (negative_size, bounds)
	% moves len chars from v1 starting at s1 to v2 starting at s2
	% 1 <= s1 <= size(v1) + 1
	% 1 <= s2 <= size(v2) + 1
	% len >= 0
	end move_v

move_b = proc (v1: cvt, s1: int, v2: _vec[char], s2: int, len: int)
						signals (negative_size, bounds)
	% moves len chars from v1 starting at s1 to v2 starting at s2
	% 1 <= s1 <= size(v1) + 1
	% 1 <= s2 <= size(v2) + 1
	% len >= 0
	end move_b

build = proc (v: cvt) returns (cvt)
	% builds new _bytevec from ASCIZ string, converts to lower case
	end build

_gcd = proc (v: cvt, tab: gcd_tab) returns (int)
	end _gcd

end _bytevec
