_signal = cluster is set, get, unset, set_message, get_message

rep = any

set = proc (sig: int, hold: bool) signals (bad_code)
	% set to count occurrences of given signal if hold = FALSE
	% place signal on hold if hold = TRUE
	% NOTE: signal 8 (SIGFPE) should be avoided if real arithmetic is used
	% NOTE: signals 2 (SIGINT), 3 (SIGQUIT), 4 (SIGILL), 10 (SIGBUS),
	% 11 (SIGSEGV), and 18 (SIGTSTP) should not be set until after a
	% call to stream/_chan primary_input/output
	end set

unset = proc (sig: int) signals (bad_code)
	% restore signal to its orignal setting
	end unset

get = proc (sig: int) returns (int) signals (bad_code)
	% get number of occurrences of signal since last get call
	end get

set_message = proc (sig: int, msg: string) signals (bad_code)
	% set message to be output (on primary output) on every
	% occurrence of signal
	end set_message

get_message = proc (sig: int) returns (string) signals (bad_code)
	% get message to be output for signal
	end get_message

end _signal
