_event = cluster is defer, undefer

rep = any

defer = proc ()
	% defer quit interrupt, stop array-trimming during garbage collection
	% must be paired with undefer call!
	end defer

undefer = proc ()
	% allow quit interrupt, allow array-trimming during garbage collection
	% must be paired with defer call!
	end undefer

end _event
