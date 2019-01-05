_byteptr = cluster is create, reset, next, peek, deposit, adjust, empty,
		      get_buf, get_index, get_count, equal, similar, copy,
		      _gcd

rep = any

create = proc (bv: _bytevec, i, cnt: int) returns (cvt)
					signals (bounds, negative_size)
	% start index i, cnt chars
	end create

reset = proc (bp: cvt, bv: _bytevec, i, cnt: int)
					signals (bounds, negative_size)
	% start index i, cnt chars
	end reset

next = proc (bp: cvt) returns (char) signals (bounds)
	% read char and increment index
	end next

peek = proc (bp: cvt) returns (char) signals (bounds)
	% peek at next char
	end peek

deposit = proc (bp: cvt, c: char) signals (bounds)
	% write char and increment index
	end deposit

adjust = proc (bp: cvt, disp: int) signals (bounds)
	% move disp chars relative to current position
	end adjust

empty = proc (bp: cvt) returns (bool)
	% test if next read/write will fail
	end empty

get_buf = proc (bp: cvt) returns (_bytevec)
	end get_buf

get_index = proc (bp: cvt) returns (int)
	% get next index to be read/written
	end get_index

get_count = proc (bp: cvt) returns (int)
	% get count of remaining chars to read/write
	end get_count

equal = proc (bp1, bp2: cvt) returns (bool)
	% object identity
	end equal

similar = proc (bp1, bp2: cvt) returns (bool)
	% same buf, index, and count
	end similar

copy = proc (bp: cvt) returns (cvt)
	% really copy1
	end copy

_gcd = proc (bp: cvt, tab: gcd_tab) returns (int)
	end _gcd

end _byteptr
