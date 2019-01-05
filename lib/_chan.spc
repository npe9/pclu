_chan = cluster is open, _open, pipe, socket, socketpair,
		   primary_input, primary_output, error_output, nul,
		   control, lock, sync, reset, close, abort,
		   is_tty,
		   getc, geti, getb, getbv, getw, getwv, get, recv, recvfrom,
		   pending, pend_count, buf_count,
		   putc, puti, puts, putb, putw, put, send, sendto,
		   read_wait, write_wait, wait,
		   listen, accept, bind, connect, shutdown, peername, sockname,
		   getsockopt, setsockopt,
		   get_name, get_date, get_speeds, get_block_size, stat,
		   get_access, set_access, get_length, set_length,
		   equal, similar, copy,
		   get_read_channel, get_write_channel,
		   get_def_mode, set_def_mode,
		   _save_tty, _restore_tty, close_all

rep = any

open = proc (fn: file_name, mode: string, fmode: int) returns (cvt)
					  signals (not_possible(string))
	% modes are "read"		read existing file
	%	    "write"		overwrite existing file/write new file
	%	    "append"		append to existing file/write new file
	%	    "random"		overwrite & read existing/new file
	%	    "modify"		read & write existing/new file
	% fmode is protection mode for new files
	% terminal _chan is always read/write for both normal and image I/O
	end open

_open = proc (fn: file_name, flags, fmode: int) returns (cvt)
						signals (not_possible(string))
	% flags are as in Unix open system call
	% fmode is protection mode for new files
	% terminal _chan is always read/write for both normal and image I/O
	end _open

pipe = proc () returns (cvt, cvt) signals (not_possible(string))
	% returns read _chan, write _chan
	end pipe

socket = proc (family, socktype, protocol: int) returns (cvt)
						signals (not_possible(string))
	% create a socket
	end socket

socketpair = proc (domain, socktype, protocol: int) returns (cvt, cvt)
						signals (not_possible(string))
	% create a pair of sockets
	end socketpair

primary_input = proc () returns (cvt)
	% primary input
	% invoking this when both primary input and output are the terminal
	% will cause a tty mode change.  from that point on, the program cannot
	% be run in the background.
	end primary_input

primary_output = proc () returns (cvt)
	% primary output
	% invoking this when both primary input and output are the terminal
	% will cause a tty mode change.  from that point on, the program cannot
	% be run in the background.
	end primary_output

error_output = proc () returns (cvt)
	% error output
	end error_output

nul = proc () returns (cvt)
	% infinite source/sink
	end nul

control = proc (ch: cvt, cmd, arg: int) returns (int)
					signals (not_possible(string))
	% cmd and arg are as in fcntl Unix system call
	end control

lock = proc (ch: cvt, op: int) signals (not_possible(string))
	% op is as in flock Unix system call
	end lock

sync = proc (ch: cvt) signals (not_possible(string))
	% flush in-core buffers to disk
	end sync

reset = proc (ch: cvt) signals (not_possible(string))
	% sets access pointer to 0
	end reset

close = proc (ch: cvt) signals (permanent, not_possible(string))
	% close _chan
	end close

abort = proc (ch: cvt) signals (permanent, not_possible(string))
	% close _chan, discard contents and file if new
	end abort

is_tty = proc (ch: cvt) returns (bool)
	% terminal
	end is_tty

getc = proc (ch: cvt, image: bool) returns (char)
				   signals (end_of_file, not_possible(string))
	% read one byte as a char (image only applies to terminals)
	% ^M cannot be read from a terminal, even in image mode
	end getc

geti = proc (ch: cvt, image: bool) returns (int)
		   signals (overflow, end_of_file, not_possible(string))
	% read one byte as an int (image only applies to terminals)
	% ^M cannot be read from a terminal, even in image mode
	end geti

getb = proc (ch: cvt, bv: _bytevec) returns (int)
			signals (bounds, end_of_file, not_possible(string))
	% read bytes into bv, return count read
	end getb

getbv = proc (ch: cvt, bv: _bytevec, strt, n: int) returns (int)
			signals (bounds, end_of_file, not_possible(string))
	% read n bytes into bv starting at index strt
	% return count of bytes actually read
	end getbv

getw = proc (ch: cvt, wv: _wordvec) returns (int)
			signals (bounds, end_of_file, not_possible(string))
	% read bytes into wv, return count read
	end getw

getwv = proc (ch: cvt, wv: _wordvec, strt, n: int) returns (int)
			signals (bounds, end_of_file, not_possible(string))
	% read n bytes into wv starting at byte index strt
	% return count of bytes actually read
	end getwv

get = proc (ch: cvt, bvec: _bytevec, low, high: int, term: string, image: bool)
			returns (string, int, int)
			signals (bounds, end_of_file, not_possible(string))
	% do stream-like gets, using bvec for buffer
	% low-high gives current non-empty portion of buffer
	% returns new low-high
	end get

recv = proc (ch: cvt, buf: _wordvec, flags: int) returns (int)
						 signals (not_possible(string))
	% receive a message in buf
	% flags are as in recv Unix system call
	% returns length of message read
	end recv

recvfrom = proc (ch: cvt, buf: _wordvec, flags: int, addr: _wordvec)
			returns (int, int) signals (not_possible(string))
	% receive a message in buf
	% flags are as in recvfrom Unix system call
	% sender address is stored in addr
	% returns length of message read and length of addr stored
	end recvfrom

pending = proc (ch: cvt, image: bool) returns (bool)
				      signals (not_possible(string))
	% tests if input is pending
	end pending

pend_count = proc (ch: cvt) returns (int) signals (not_possible(string))
	% returns number of bytes pending
	end pend_count

buf_count = proc (ch: cvt) returns (int) signals (not_possible(string))
	% returns number of buffered bytes
	end buf_count

putc = proc (ch: cvt, c: char, image: bool) signals (not_possible(string))
	% write char as one byte
	end putc

puti = proc (ch: cvt, i: int, image: bool) signals (not_possible(string))
	% write int as one byte
	end puti

puts = proc (ch: cvt, s: string, image: bool) signals (not_possible(string))
	% write chars as bytes
	end puts

putb = proc (ch: cvt, bv: _bytevec, strt, stop: int, image: bool)
					signals (bounds, not_possible(string))
	% write chars from bv starting at strt to stop
	end putb

putw = proc (ch: cvt, wv: _wordvec, strt, stop: int, image: bool)
					signals (bounds, not_possible(string))
	% write bytes from wv starting at byte index strt to stop
	end putw

put = proc (ch: cvt, bvec: _bytevec, low, high: int, s: string, image: bool)
		returns (int, int) signals (bounds, not_possible(string))
	% do stream-like puts using bvec as buffer
	% low-high gives current non-empty portion of buffer
	% returns new low-high
	end put

send = proc (ch: cvt, buf: _wordvec, len, flags: int)
					signals (bounds, not_possible(string))
	% send the message in the first len bytes of buf
	% flags are as in send Unix system call
	% signals bounds if len < 0
	end send

sendto = proc (ch: cvt, buf: _wordvec, len, flags: int,
		addr: _wordvec, alen: int)
				signals (bounds, not_possible(string))
	% send the message in the first len bytes of buf to the address
	%  in the first alen bytes of addr
	% flags are as in sendto Unix system call
	% signals bounds if len < 0 or alen < 0
	end sendto

read_wait = proc (ch: cvt, millis: int) returns (bool)
					signals (not_possible(string))
	% wait until either ch has data or an exception available, or
	%  until millis milliseconds have elapsed
	% returns true if ch has data or an exception available
	end read_wait

write_wait = proc (ch: cvt, millis: int) returns (bool)
					signals (not_possible(string))
	% wait until either ch can take data or an exception is available, or
	%  until millis milliseconds have elapsed
	% returns true if ch can take data or an exception is available
	end write_wait

wait = proc (reads, writes: sequence[_chan], millis: int) returns (int)
					 signals (not_possible(string))
	% wait until one or more of reads have data or an exception available
	%  or writes can take data or have an exception available, or until
	%  millis milliseconds have elapsed
	% returns number of ready _chans
	end wait

listen = proc (ch: cvt, backlog: int) signals (not_possible(string))
	% allow socket connections
	% backlog is as in listen Unix system call
	end listen

accept = proc (ch: cvt, addr: _wordvec) returns (cvt, int)
					signals (not_possible(string))
	% accept a connection on the socket
	% the address of the connector is stored in addr
	% returns a new socket and the length of the address stored in addr
	end accept

bind = proc (ch: cvt, name: _wordvec, len: int)
					signals (bounds, not_possible(string))
	% bind a name to a socket
	% len is the length of the name in name
	end bind

connect = proc (ch: cvt, addr: _wordvec, len: int)
					signals (bounds, not_possible(string))
	% connect an addres to the socket
	% len is the length of the addres in addr
	end connect

shutdown = proc (ch: cvt, how: int) signals (not_possible(string))
	% shut down part of a connection
	% how is as in shutdown Unix system call
	end shutdown

peername = proc (ch: cvt, addr: _wordvec) returns (int)
					  signals (not_possible(string))
	% store name of connected peer in addr
	% returns length of stored name
	end peername

sockname = proc (ch: cvt, addr: _wordvec) returns (int)
					  signals (not_possible(string))
	% store name of socket in addr
	% returns length of stored name
	end sockname

getsockopt = proc (ch: cvt, level, option: int) returns (int)
						signals (not_possible(string))
	% get socket option
	% level and option are as in getsockopt Unix system call
	% returns value (assumed to be an int)
	end getsockopt

setsockopt = proc (ch: cvt, level, option, value: int)
						signals (not_possible(string))
	% set socket option
	% level and option are as in setsockopt Unix system call
	end setsockopt

get_name = proc (ch: cvt) returns (file_name) signals (not_possible(string))
	% exact file name
	end get_name

get_date = proc (ch: cvt) returns (date) signals (not_possible(string))
	% get date last written
	end get_date

set_date = proc (ch: cvt, d: date) signals (not_possible(string))
	% set date last written
	end set_date

get_speeds = proc (ch: cvt) returns (int, int) signals (not_possible(string))
	% get input and output line speeds if terminal
	end get_speeds

get_block_size = proc (ch: cvt) returns (int) signals (not_possible(string))
	% return optimal block size
	end get_block_size

stat = proc (ch: cvt, buf: _wordvec) signals (bounds, not_possible(string))
	% do fstat Unix system call storing result in buf
	end stat

get_access = proc (ch: cvt) returns (int) signals (not_possible(string))
	% get access pointer (starts at 0)
	end get_access

set_access = proc (ch: cvt, pos: int) signals (not_possible(string))
	% set access pointer to byte pos (starts at 0)
	end set_access

get_length = proc (ch: cvt) returns (int) signals (not_possible(string))
	% get length in bytes
	end get_length

set_length = proc (ch: cvt, len: int) signals (not_possible(string))
	% set length in bytes
	end set_length

equal = proc (ch1, ch2: cvt) returns (bool)
	% object identity
	end equal

similar = proc (ch1, ch2: cvt) returns (bool)
	% object identity
	end similar

copy = proc (ch: cvt) returns (cvt)
	% does not copy
	end copy

get_read_channel = proc (ch: cvt, image: bool) returns (int)
					       signals (not_possible(string))
	% get read jfn
	end get_read_channel

get_write_channel = proc (ch: cvt, image: bool) returns (int)
						signals (not_possible(string))
	% get write jfn
	end get_write_channel

get_def_mode = proc () returns (int)
	% default protection for new files
	end get_def_mode

set_def_mode = proc (mode: int)
	% default protection for new files
	end set_def_mode

_save_tty = proc ()
	% save current tty mode and restore normal mode
	end _save_tty

_restore_tty = proc ()
	% restore saved tty mode
	end _restore_tty

close_all = proc ()
	% close all open file descriptors
	end close_all

end _chan
