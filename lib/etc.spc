% removed type_of 7/16/91 dcurtis
_anyize = proc [t: type] (x: t) returns (any)
    % create an ANY
    end _anyize

_change_mode = proc (fn: file_name, mode: int) signals (not_possible(string))
    % change file mode word
    end _change_mode

_change_owner = proc (fn: file_name, userid, groupid: int) signals (not_possible(string))
    end _change_owner

_change_signal = proc (sig: int, label: int) returns (int)
		   signals (not_possible(string))
    % change action for given signal (interrupt), return old action
    end _change_signal

_cvt = proc [from, to: type] (x: from) returns (to)
    % fake compiler out as to type of object (use with great care!)
    end _cvt

_dir_ent_size = proc () returns (int)
    % returns maximum size of a file name component
    end _dir_ent_size

_dtable_size = proc () returns (int)
    % returns maximum number of open files
    end _dtable_size

_environ = proc (name: string) returns (string) signals (not_found)
    % return environment variable
    end _environ

_environs = iter () yields (string)
    % yield the environment
    end _environs

_erstr = proc (num: int) returns (string)
    % return error string for given error code
    end _erstr

_ernum = proc (msg: string) returns (int) signals(not_found)
    % return error code for given error string
    end _ernum

_exit = proc (status: int)
    % _cleanup_() and terminate process with particular status
    end _exit

_file_access = proc (fs: string, mode: int) returns (bool)
    % determine file accessibility
    % OR of read = 4, write = 2, execute = 1, existence = 0
    end _file_access

_file_exists = proc (fs: string, ftype: int) returns (bool)
		 signals (not_possible(string))
    % test if file is of given type
    % directory = 4, char special = 2, block special = 6, regular = 8
    % multiplexed char special = 3, multiplexed block special = 7
    end _file_exists

_file_facts = proc (fn: file_name) returns (int, int)
		signals (not_possible(string))
    % return number of links, file mode of given file
    end _file_facts

_file_owner = proc (fn: file_name) returns (int, int)
		signals (not_possible(string))
    % return user id and group id of given file
    end _file_owner

_free_space = proc () returns (int)
    % number of free (long) words left in heap
    % For PCLU, this is bytes_to_words("max_heap") - words of allocated objects
    % (see comments below)
    end _free_space

_get_gid = proc () returns (int, int)
    % real group ID, effective group ID
    end _get_gid

_get_pid = proc () returns (int)
    % process ID
    end _get_pid

_get_pname = proc () returns (string)
    % return program name with directory, if any
    end _get_pname

_get_priority = proc (which, who: int) returns (int)
		  signals (not_possible(string))
    % do getpriority Unix system call
    end _get_priority

_get_runtime = proc () returns (int, int, int)
    % return seconds, milliseconds, microseconds of user+system time
    % for current process
    end _get_runtime

_get_termcap = proc () returns (string) signals (not_found)
    end _get_termcap

_get_uid = proc () returns (int, int)
    % real user ID, effective user ID
    end _get_uid

_get_version = proc () returns (int)
    % return program version number (set by #version option of LINK)
    end _get_version

_get_xjname = proc () returns (string)
    % return program name with directory removed
    end _get_xjname

_groups = proc () returns (sequence[int])
    % returns group access list
    end _groups

_heap_size = proc () returns (int)
    % current size of heap in (long) words
    % For PCLU, returns the number of machine words of allocated objects,
    % which is generally smaller than "max_heap".  (see comments below)
    end _heap_size

_home_dir = proc (user: string) returns (string) signals (not_found)
    end _home_dir

_host_id = proc () returns (int, int)
    % returns upper and lower halves of host id
    end _host_id

_host_name = proc () returns (string)
    % returns host name
    end _host_name

_ioctl = proc [t: type] (fdesc, reqh, reql: int, argp: t, disp: int)
	   signals (not_possible(string))
    % issue REQ ioctl on given file descriptor with given argument(s)
    % REQ is given in halves, high and low
    % if ARGP is not an int, DISP should generally be 4
    end _ioctl

_last_indexc = proc (c: char, s: string) returns (int)
    % Find the last occurence of c in the string s.
    end _last_indexc

_local_time = proc (l, r: int) returns (date)
    % Get local time. l and r form the number of seconds west of greenwich for the
    % time-zone required
    end _local_time

_lstat = proc (fn: file_name, buf: _wordvec)
	   signals (bounds, not_possible(string))
    % do lstat Unix system call placing result in buf
    end _lstat

_page_size = proc () returns (int)
    % returns system page size in bytes
    end _page_size

_pause = proc ()
    % wait for signal (interrupt)
    end _pause

_read_link = proc (fn: file_name, buf: _bytevec) returns (int)
	       signals (not_possible(string))
    % read value of symbolic link into buf
    % returns number of characters read
    end _read_link

_real_time = proc () returns (int)
    % return real time in milliseconds since unknown event in past
    end _real_time

_round_real = proc (digs: _bytevec, num: int) returns (int, int)
    % round DIGS to NUM digits
    % used by e_form, f_form, g_form
    end _round_real

_rusage = proc (who: int, buf: _wordvec) signals (bounds, not_possible(string))
    % do getrusage Unix system call, placing result in buf
    end _rusage

_select = proc (cnt, reads, writes, excepts, millis: int)
	    returns (int, int, int, int)
	    signals (not_possible(string))
    % do select Unix system call
    % returns (number found, read mask, write mask, except mask)
    end _select

_set_alarm = proc (secs: int) returns (bool)
    % set alarm interrupt to go off in SECS seconds real time
    % (0 => no alarm)
    % cancels any previous alarm
    % returns true if alarm has gone off since last call
    end _set_alarm

_set_limit = proc (resource, value: int) signals (not_possible(string))
    % set soft resource limit via setrlimit Unix system call.
    end _set_limit

_set_priority = proc (which, who: int) returns (int)
		  signals (not_possible(string))
    % do setpriority Unix system call
    end _set_priority

_sleep = proc (millis: int)
    % sleep for MILLIS milliseconds.
    end _sleep

_split_name_part = proc (s: string) returns (string, string, string)
    % Split file name up into name, suffix, rest (Separator = '.')
    end _split_name_part

_stat = proc (fn: file_name, buf: _wordvec)
	  signals (bounds, not_possible(string))
    % do stat Unix system call placing result in buf
    end _stat

_stop = proc ()
    % force SIGTSTP signal (interrupt) on this process
    % to stop in a continuable fashion
    end _stop

_system_root = proc () returns (string)
    % returns CLU system root directory name (no trailing slash)
    end _system_root

_termcap_entry = proc (fs, term: string) returns (string) signals (not_found)
    % Return the termcap entry for fs in terminal term.
    end _termcap_entry

_umask = proc (mask: int) returns (int) signals (not_possible(string))
    % set umask and return previous value
    end _umask

_unparse_real = proc (x: real) returns (bool, _bytevec, int)
    % returns sign, digits, exponent
    % used by e_form, f_form, g_form
    end _unparse_real

create_directory = proc (fn: file_name, mode: int)
		     signals (not_possible(string))
    end create_directory

connected_dir = proc () returns (string)
    % Get connected directory
    end connected_dir

delete_directory = proc (fn: file_name) signals (not_possible(string))
    end delete_directory

exp = proc (x: real) returns (real) signals (overflow, underflow)
    % e**X
    end exp

% file_date added 8/28/90 dcurtis
file_date = proc (fn: file_name, read: bool) returns (date)
                            signals (not_possible(string))
    end file_date

% file_exists added 8/28/90 dcurtis
file_exists = proc (fn: file_name) returns (bool)
		 signals (not_possible(string))
    end file_exists

gc = proc () returns (int)
    % returns number of (long) words reclaimed
    end gc

get_argv = proc () returns (sequence[string])
    % returns command line arguments, not including program name
    end get_argv

set_argv = proc (ss: sequence[string])
	% save argument for use as future command line arguments
        end set_argv

get_jcl = proc () returns (string)
    % returns command line, not including program name, first time called
    end get_jcl

host_address = proc (host: string) returns (int, int)
		 signals (not_found, bad_address)
    % returns upper and lower halves of host address
    end host_address

istream2chan = proc (ist: istream) returns (_chan) signals (not_possible(string))
    % get the _chan out of a istream
    end istream2chan

link_file = proc (ofn, nfn: file_name) signals (not_possible(string))
    % make NFN a link to OFN
    end link_file

log = proc (x: real) returns (real) signals (undefined)
    % natural logarithm
    end log

now = proc () returns (date)
    % Get date of now
    end now

% quit_ added 8/28/90 dcurtis
quit_ = proc()
	end quit_

set_connected_dir = proc (s: string) signals (bad_format, not_possible(string))
    % change connected directory
    end set_connected_dir


% sin added 8/28/90 dcurtis
sin = proc (x: real) returns (real)
	end sin

sleep = proc (secs: int)
    % sleep for SECS seconds
    end sleep

sqrt = proc (x: real) returns (real) signals (complex_result)
    % square root
    end sqrt

stream2chan = proc (st: stream) returns (_chan) signals (not_possible(string))
    % get the _chan out of a stream
    end stream2chan

symlink_file = proc (ofn, nfn: file_name) signals (not_possible(string))
    % make NFN a link to OFN
    end symlink_file

% type_of = proc (a: any) returns (type)
    % Get the type of an any.
%    end type_of

user_name = proc () returns (string)
    % What is the users name.
    end user_name

logit = proc(s: string)
	end logit
%
%	specification for routine that will invoke
%	  the unix system call
%		(takes a string and causes a shell to execute it)
%

unix_cmd = proc(cmd: string)
	end unix_cmd

%
% PCLU Garbage Collector interface
%
% On non-PCLU implementations, all routines signal illegal.
%
% The heap is allocated in blocks.  A heap block is active if it contains an
% object.
%
% The read-only gc parameters are:
%  "active_heap" = memory in active heap blocks
% The read-write gc parameters are:
%  "max_heap"    = upper bound of "active_heap"
%  "min_gc"      = threshold for "active_heap" below which the collector
%                  never runs.
% All parameters are in units of bytes.  Programs should use _cluref_size()
% to obtain the number of bytes required to store a pointer or an int in the
% heap.

%
% The collector will not run automatically when "active_heap" < "min_gc".
% In general, the collector will run well before "active_heap" reaches
% "max_heap".
%
% Setting "min_gc" to a high value, e.g., "max_heap", can hurt
% performance by reducing locality, increasing fragmentation, or causing the
% program to page.

_get_active_heap = proc () returns(int) signals (not_applicable)
    % Returns "active_heap", in bytes.
    % This value is continuously updated by allocation and garbage
    % collection.
    % Signals
    %   not_applicable when not running under PCLU
    end _get_active_heap

_get_max_heap = proc () returns(int) signals (not_applicable)
    % Returns "max_heap", in bytes.
    % Signals
    %   not_applicable when not running under PCLU
    end _get_max_heap

_set_max_heap = proc (n: int)
	        signals (not_applicable, illegal, illegal_size(int))
    % Effect: "max_heap" := the greatest multiple of the heap block
    % size <= N.  N is in bytes.
    % Signals
    %   not_applicable when not running under PCLU
    %   illegal when N < max("active_heap","min_gc")
    %   illegal_size(l)
    %     when N > L, where L is the maximum size of that data segment,
    %                 in bytes, allowed by the operating system
    end _set_max_heap

_get_min_gc = proc () returns(int) signals (not_applicable)
    % Returns "min_gc", in bytes.
    % Signals
    %   not_applicable when not running under PCLU
    end _get_min_gc

_set_min_gc = proc (n: int) signals (not_applicable, toobig, negative_size)
    % Effect: "min_gc" := the greatest multiple of the heap block
    % size <= N.  N is in bytes.
    % Signals
    %   not_applicable when not running under PCLU
    %   toobig("exceeds max_heap") when N > "max_heap"
    %   negative_size when N < 0
    end _set_min_gc

_gc_control = proc (add, mask: int) signals (illegal)
    % This operation is provided for compatibility with the _gc$control
    % operation of the VAX and Sun native CLU compilers.
    %
    % With the native compilers, after a GC, the new GC point is
    % (heap_size+ADD)&~MASK, where heap_size, ADD, and MASK are in bytes.
    % Under PCLU, ADD and MASK can be used to decrease the frequency of
    % garbage collections.
    %
    % Signals illegal when ADD <= 0 or not(MASK lssu ADD)
    end _gc_control

_cluref_size = proc () returns(int) signals (not_applicable)
    % Returns the size, in bytes, of pointers and ints, i.e.,
    % the size of a machine word.
    end _cluref_size
