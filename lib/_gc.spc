_gc = cluster is count, gc

rep = any

count = proc () returns (int)
	% return number of garbage collections so far
	end count

% control = proc (add, mask: int) signals (illegal)
%	% This operation has been replaced by _gc_control;
%	% see the file etc.spc.

gc = proc ()
	% force a garbage collect
	end gc

end _gc
