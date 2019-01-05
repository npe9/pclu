_job = cluster is cons, cons1, cone, cone1, exec, exece, fork, run, sig, sig_group, dead,
		  wait, wait0, any_wait, any_wait0,
		  self, get_pid, get_group, set_group, control_tty,
		  get_group_mode, set_group_mode,
		  equal, similar, copy

rep = any

cons = proc (fn: file_name, jcl: string,
		pri, pro, ero: _chan, save: sequence[_chan])
				returns (cvt) signals (not_possible(string))
	% fork process to execute FN with given JCL
	% If JCL does not contain nuls, spaces and tabs will be converted
	% PRI, PRO, and ERO become primary input, output, and error output
	% SAVE is a list of _chans that should not be closed in the new process
	end cons

cons1 = proc (fn: file_name, jcl: string, save: sequence[_chan]) returns (cvt)
						signals (not_possible(string))
	% fork process to execute FN with given JCL
	% If JCL does not contain nuls, spaces and tabs will be converted
	% current primary input, output, and error output are retained
	% SAVE is a list of _chans that should not be closed in the new process
	end cons1

cone = proc (fn: file_name, jcl: string, envs: array[string],
		pri, pro, ero: _chan, save: sequence[_chan])
				returns (cvt) signals (not_possible(string))
	% fork process to execute FN with given JCL
	% If JCL does not contain nuls, spaces and tabs will be converted
	% PRI, PRO, and ERO become primary input, output, and error output
	% envs is the environment for the new program.
	% SAVE is a list of _chans that should not be closed in the new process
	end cone

cone1 = proc (fn: file_name, jcl: string, envs: array[string], save: sequence[_chan]) returns (cvt)
						signals (not_possible(string))
	% fork process to execute FN with given JCL
	% If JCL does not contain nuls, spaces and tabs will be converted
	% current primary input, output, and error output are retained
	% envs is the environment for the new program.
	% SAVE is a list of _chans that should not be closed in the new process
	end cone1

exec = proc (fn: file_name, jcl: string, save: sequence[_chan])
						signals (not_possible(string))
	% execute FN with given JCL in current process
	% If JCL does not contain nuls, spaces and tabs will be converted
	% current primary input, output, and error output are retained
	% SAVE is a list of _chans that should not be closed
	end exec

exece = proc (fn: file_name, jcl: string, envs: array[string], save: sequence[_chan])
						signals (not_possible(string))
	% execute FN with given JCL in current process
	% If JCL does not contain nuls, spaces and tabs will be converted
	% current primary input, output, and error output are retained
	% envs is the environment for the new program.
	% SAVE is a list of _chans that should not be closed
	end exece

fork = proc () returns (cvt, bool) signals (not_possible(string))
	% fork copy of current process
	% returns (child, false) in parent process
	% returns (parent, true) in child process
	end fork

run = proc (j: cvt) returns (int) signals (not_possible(string))
	% wait for termination of given process
	% restart process whenever it stops
	% discard stop or termination of any intervening process
	% return termination status
	end run

sig = proc (j: cvt, signum: int) signals (not_possible(string))
	% send signal to given process
	end sig

sig_group = proc (j: cvt, signum: int) signals (not_possible(string))
	% send signal to all processes in given process's process group
	end sig_group

wait = proc (j: cvt) returns (int) signals (not_possible(string))
	% wait for stop or termination of given process
	% discard termination of any intervening process
	% return stop or termination status
	end wait

wait0 = proc (j: cvt) returns (int) signals (not_possible(string))
	% test for stop or termination of given process without hanging
	% discard termination of any intervening process
	% return stop or termination status if any, else -1
	end wait0

any_wait = proc () returns (cvt, int) signals (not_possible(string))
	% wait for stop or termination of next process
	% return stop or termination status
	end any_wait

any_wait0 = proc () returns (cvt, int) signals (not_possible(string))
	% test for stop or termination of next process without hanging
	% return _job (0 if none terminated) and stop or termination status
	end any_wait0

self = proc () returns (cvt)
	% return current process
	end self

get_pid = proc (j: cvt) returns (int)
	% get process id
	end get_pid

get_group = proc (j: cvt) returns (int) signals (not_possible(string))
	% get process group
	end get_group

set_group = proc (j: cvt, g: int) signals (not_possible(string))
	% set process group
	end set_group

control_tty = proc (j: cvt) signals (not_possible(string))
	% make this process' group the controlling terminals' group
	end control_tty

get_group_mode = proc () returns (int)
	% get process group mode
	end get_group_mode

set_group_mode = proc (mode: int)
	% set process group mode
	% 0 => do nothing
	% ~= 0 => cons and cons1 will set process group to be process id
	% < 0 => cons and cons1 will also change control of file desc 0
	%	 to new process group
	end set_group_mode

dead = proc () returns (int)
	% return count of processes that have died
	end dead

equal = proc (j1, j2: cvt) returns (bool)
	end equal

similar = proc (j1, j2: cvt) returns (bool)
	end similar

copy = proc (j: cvt) returns (cvt)
	end copy

end _job
