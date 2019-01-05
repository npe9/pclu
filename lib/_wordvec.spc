_wordvec = cluster is create, fetch, store, fetch2, store2, xstore,
		      bfetch, bstore, wfetch, wstore, lfetch, lstore,
		      move_lr, move_rl,
		      get_byte, set_byte, word_size, byte_size, bytes_per_word,
		      move_b2w, move_w2b, size, equal, copy, _gcd

rep = any

create = proc (n: int) returns (cvt) signals (toobig)
	% number of words
	end create

fetch = proc (wv: cvt, idx: int) returns (int) signals (bounds, overflow)
	% start index is 1
	end fetch

store = proc (wv: cvt, idx, i: int) signals (bounds)
	% start index is 1
	end store

fetch2 = proc (wv: cvt, idx: int) returns (int, int) signals (bounds)
	% returns left and right halves sign extended
	end fetch2

store2 = proc (wv: cvt, idx, l, r: int) signals (bounds)
	% stores left and right halves
	end store2

xstore = proc (wv: cvt, idx, b, l: int) signals (bounds)
	% stores high byte and low 24-bit values
	end xstore

bfetch = proc (wv: cvt, idx: int) returns (int) signals (bounds)
	% start byte index is 1
	end bfetch

bstore = proc (wv: cvt, idx, i: int) signals (bounds)
	% start byte index is 1
	end bstore

wfetch = proc (wv: cvt, idx: int) returns (int) signals (bounds)
	% start byte index is 1
	end wfetch

wstore = proc (wv: cvt, idx, i: int) signals (bounds)
	% start byte index is 1
	end wstore

lfetch = proc (wv: cvt, idx: int) returns (int) signals (bounds, overflow)
	% start byte index is 1
	end lfetch

lstore = proc (wv: cvt, idx, i: int) signals (bounds)
	% start byte index is 1
	end lstore

move_lr = proc (w1: cvt, s1: int, w2: cvt, s2: int, len: int)
						signals (negative_size, bounds)
	% moves len words from w1 starting at s1 to w2 starting at s2
	% first word is moved first
	% 1 <= s1 <= size(w1) + 1
	% 1 <= s2 <= size(w2) + 1
	% len >= 0
	end move_lr

move_rl = proc (w1: cvt, s1: int, w2: cvt, s2: int, len: int)
						signals (negative_size, bounds)
	% moves len words from w1 starting at s1 to w2 starting at s2
	% last word is moved first
	% 1 <= s1 <= size(w1) + 1
	% 1 <= s2 <= size(w2) + 1
	% len >= 0
	end move_rl

get_byte = proc (wv: cvt, wrd, bit, len: int) returns (int)
				signals (bounds, overflow, illegal_size)
	% get len bits ending at bit bit at index wrd
	% most significant bit is 1
	end get_byte

set_byte = proc (wv: cvt, byte, wrd, bit, len: int)
						signals (bounds, illegal_size)
	% set len bits ending at bit bit at index wrd
	% most significant bit is 1
	end set_byte

word_size = proc () returns (int)
	% returns 32 on VAX
	end word_size

byte_size = proc () returns (int)
	% returns 8 on VAX
	end byte_size

bytes_per_word = proc () returns (int)
	% returns 4 on VAX
	end bytes_per_word

move_b2w = proc (b: _bytevec, bpos: int, w: cvt, wpos: int, n: int)
							signals (bounds)
	% moves n bytes from b starting at bpos to w starting at wpos
	end move_b2w

move_w2b = proc (w: cvt, wpos: int, b: _bytevec, bpos: int, n: int)
							signals (bounds)
	% moves n bytes from w starting at wpos to b starting at bpos
	end move_w2b

size = proc (w: cvt) returns (int)
	% number of words
	end size

equal = proc (w1, w2: cvt) returns (bool)
	% object identity
	end equal

copy = proc (w: cvt) returns (cvt)
	% really copies
	end copy

_gcd = proc (w: cvt, tab: gcd_tab) returns (int)
	end _gcd

end _wordvec
