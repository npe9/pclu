i_and = proc (i, j: int) returns (int) signals (overflow)
	end i_and

i_or = proc (i, j: int) returns (int) signals (overflow)
	end i_or

i_xor = proc (i, j: int) returns (int) signals (overflow)
	end i_xor

i_not = proc (i: int) returns (int) signals (overflow)
	end i_not

i_shift = proc (i, cnt: int) returns (int) signals (overflow)
	end i_shift

i_rotate = proc (i, cnt: int) returns (int) signals (overflow)
	end i_rotate

i_get = proc (i, start, cnt: int) returns (int)
				  signals (bounds, illegal_size, overflow)
	% low order bit is numbered 0
	end i_get

i_set = proc (i, start, cnt, val: int) returns (int)
				       signals (bounds, illegal_size, overflow)
	% low order bit is numbered 0
	end i_set

i_get1 = proc (i, bit: int) returns (bool) signals (bounds)
	% low order bit is numbered 0
	end i_get1

i_set1 = proc (i, bit: int, val: bool) returns (int) signals (bounds, overflow)
	% low order bit is numbered 0
	end i_set1

i_first1 = proc (i: int) returns (int) signals (none)
	% return number of first 1 bit
	end i_first1

i_last1 = proc (i: int) returns (int) signals (none)
	% return number of last 1 bit
	end i_last1
