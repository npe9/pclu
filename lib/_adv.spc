_adv = cluster [t: type] is create, equal,
			    get_low, set_low,
			    get_size, set_size,
			    get_start, set_start,
			    get_predict, set_predict,
			    get_vector, set_vector, _gcd

rep = any

create = proc (low: int, pred: int, v: _vec[t]) returns (cvt) signals (bounds)
	% low bound, predicted size, initial elements
	end create

equal = proc (adv1, adv2: cvt) returns (bool)
	% object identity
	end equal

get_low = proc (adv: cvt) returns (int)
	% get low bound
	end get_low

set_low = proc (adv: cvt, low: int) signals (bounds)
	% set low bound
	end set_low

get_size = proc (adv: cvt) returns (int)
	% get size
	end get_size

set_size = proc (adv: cvt, size: int) signals (bounds)
	% set size
	end set_size

get_start = proc (adv: cvt) returns (int)
	% get index in real part of first element
	end get_start

set_start = proc (adv: cvt, strt: int) signals (bounds)
	% set index in real part of first element (recomputes size)
	end set_start

get_predict = proc (adv: cvt) returns (int)
	% get predicted size
	end get_predict

set_predict = proc (adv: cvt, pred: int)
	% set predicted size
	end set_predict

get_vector = proc (adv: cvt) returns (_vec[t])
	% get real part
	end get_vector

set_vector = proc (adv: cvt, v: _vec[t]) signals (bounds)
	% set real part (start := 1, size := size(v))
	end set_vector

_gcd = proc (adv: cvt, tab: gcd_tab) returns (int)
			where t has _gcd: proctype (t, gcd_tab) returns (int)
	end _gcd

end _adv
