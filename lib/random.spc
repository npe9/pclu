random = cluster is reset, seed, next

rep = null

reset = proc ()
	% restore initial seed
	end reset

seed = proc (x: int)
	% modify current seed
	end seed

next = proc (x: int) returns (int)
	% 0 <= res < x
	end next

end random
