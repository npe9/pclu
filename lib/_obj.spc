_obj = cluster is is_ref, is_int_, is_real_, is_uninit, is_proc_, is_iter_,
		  is_routine, is_vec, is_bvec, is_cell, is_anyize, is_adv,
		  is_entry_vector,
		  get_addr, get_left, get_right,
		  lt, le, ge, gt, equal, similar, copy, print

rep = any

is_ref = proc (x: cvt) returns (bool)
	end is_ref

is_int_ = proc (x: cvt) returns (bool)
	end is_int_

is_real_ = proc (x: cvt) returns (bool)
	end is_real_

is_uninit = proc (x: cvt) returns (bool)
	end is_uninit

is_proc_ = proc (x: cvt) returns (bool)
	end is_proc_

is_iter_ = proc (x: cvt) returns (bool)
	end is_iter_

is_routine = proc (x: cvt) returns (bool)
	end is_routine

is_vec = proc (x: cvt) returns (bool)
	end is_vec

is_bvec = proc (x: cvt) returns (bool)
	end is_bvec

is_cell = proc (x: cvt) returns (bool)
	end is_cell

is_anyize = proc (x: cvt) returns (bool)
	end is_anyize

is_adv = proc (x: cvt) returns (bool)
	end is_adv

is_entry_vector = proc (x: cvt) returns (bool)
	end is_entry_vector

get_addr = proc (x: cvt) returns (int)
	end get_addr

get_left = proc (x: cvt) returns (int)
	end get_left

get_right = proc (x: cvt) returns (int)
	end get_right

lt = proc (x, y: cvt) returns (bool)
	end lt

le = proc (x, y: cvt) returns (bool)
	end le

ge = proc (x, y: cvt) returns (bool)
	end ge

gt = proc (x, y: cvt) returns (bool)
	end gt

equal = proc (x, y: cvt) returns (bool)
	end equal

similar = proc (x, y: cvt) returns (bool)
	end similar

copy = proc (x: cvt) returns (cvt)
	end copy

print = proc (x: cvt, ps: pstream)
	end print

end _obj
