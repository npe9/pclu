stream = cluster is open, primary_input, primary_output, error_output, 
		can_read, can_write, reset, flush, close, abort, is_closed, 
		is_terminal, getc, peekc, empty, getl, gets, putc, putl, puts, 
		putzero, putleft, putright, putspace, getc_image, putc_image, 
		puts_image, gets_image, get_lineno, set_lineno, 
		get_line_length, get_page_length, get_date, set_date,
		get_name, set_output_buffered, get_output_buffered, equal, 
		similar, copy, 
		create_input, create_output, get_contents,     % string I/O
		get_buf, get_prompt, set_prompt,           % terminal I/O
		get_input_buffered, set_input_buffered,    % terminal I/O
		add_script, rem_script, unscript    % scripting

rep = any
%
% This file was created for documentation purposes only, using the
% book "Abstraction and Specification in Program Development",
% Appendix A, section 10. It lacks specs for the "terminal I/O" routines.
%
% DO NOT USE THIS FILE FOR BUILDING ~CLU/lib/*.lib FILES
%
% Overview
%  
% Streams provide the means to read and write text files and to perform some
% other operations on file objects.  The operations allowed on any particular
% stream depend upon the access mode.  In addition, certain operations may have
% no effect in some implementations.
% 
% When an operation cannot be performed because of an incorrect access mode,
% implementation limitations, or properties of an individual file or device,
% the operation will signal not_possible (unless the description of the
% operation explicitly says that the invocation will be ignored).  End_of_file
% is signaled by reading operations when there are no more characters to read.
% 
% Streams provide operations to connect streams to strings, to interact with a
% user at a terminal, and to record input/output in one stream on another.
% These operations are described in subsequent sections.

 open = proc (fn: file_name, mode: string) returns (stream) 
						signals (not_possible(string))
    end open
%   effects  Opens a stream to fn in the given mode. The possible access modes 
%	are "read", "write", and "append". If mode is not one of these strings,
%	not_possible("bad access mode") is signaled. In those cases where the 
%	system is able to detect that the specified preexisting file is not a 
%	text file, not_possible("wrong file type") is signaled. If mode is 
%	"read", the named file must exist and a stream is returned upon which 
%	input operations can be performed.
%	If mode is "write", a new file is created or an old file is rewritten.
%	A stream is returned upon which output operations can be performed.
%	Write mode to storage files should guarantee exclusive access to the 
%	file, if possible. 
%	If mode is "append" and if the named file does not exist, one is 
%	created.  A stream is returned, positioned at the end of the file, 
%	upon which output operations can be performed. Append mode to storage
%	files should guarantee exclusive access to the file, if possible.

 primary_input = proc( ) returns (stream) end primary_input
%   effects  Returns the "primary" input stream, suitable for reading. This is 
%	usually a stream to the user's terminal, but may be set by the 
%	operating system.
 primary_output = proc( ) returns (stream) end primary_output
%   effects  Returns the "primary" output stream, suitable for writing. This is
%	usually a stream to the user's terminal, but may be set by the 
%	operating system.
 error_output = proc( ) returns (stream) end error_output
%   effects  Returns the "primary" output stream for error messages, suitable 
%	for writing. This is usually a stream to the user's terminal, but may 
%	be set by the operating system.
 can_read = proc (s: stream) returns (bool) end can_read
%   effects  Returns true if input operations appear possible on s.
 can_write = proc (s: stream) returns (bool) end can_write
%   effects  Returns true if output operations appear possible on s.
 getc = proc (s: stream) returns (char) 
			 signals (end_of_file, not_possible(string))
    end getc
%   modifies  s. 
%   effects  Removes the next character from s and returns it.
%	Signals end_of_file if there are no more characters.
 peekc = proc (s: stream) returns (char) 
			  signals (end_of_file, not_possible(string)) 
    end peekc
%   effects  This input operation is like STREAM$GETC, except that the 
%	character is not removed from s.
 empty = proc (s: stream) returns (bool) signals (not_possible(string)) 
    end empty
%   effects  Returns true if and only if there are no more characters in the 
%	stream.  It is equivalent to an invocation of STREAM$PEEKC, where true
%	is returned if PEEKC returns a character and false is returned if
%	PEEKC signals end_of_file. Thus in the case of terminals, for example,
%	this operation may wait until additional characters have been 
%	typed by the user.
 getl = proc (s: stream) returns (string) 
			 signals (end_of_file, not_possible(string)) end getl
%   modifies  s. 
%   effects  Reads and returns (the remainder of) the current input line and
%	reads but does not return the terminating newline (if any).
%	This operation signals end_of_file only if there are no characters 
%	and end-of-file is detected.
 gets = proc (s: stream, term: string) returns (string)
			signals (end_of_file, not_possible(string)) end gets
%   modifies  s. 
%   effects  Reads characters until a terminating character (one in term) or 
%	end-of-file is seen. The characters up to the terminator are returned;
%	the terminator (if any) is left in the stream. Signals end_of_file
%	only if there are no characters and end-of-file is detected.
 putc = proc (s: stream, c: char) signals (not_possible(string)) end putc
%   modifies  s. 
%   effects  Appends c to s. Writing a newline indicates the end of the 
%	current line.
 putl = proc (s: stream, str: string) signals (not_possible(string)) end putl
%   modifies  s. 
%   effects  Writes the characters of @i(str) onto s, followed by a newline.
 puts = proc (s: stream, str: string) signals (not_possible(string)) end puts
%   modifies  s. 
%   effects  Writes the characters in str using STREAM$PUTC. Naturally it may 
%	be somewhat more efficient than doing a series of individual PUTCs.
 putzero = proc (s: stream, str: string, cnt: int)
			signals (negative_field_width, not_possible(string))
	end putzero
%   modifies  s. 
%   effects  Outputs str. However, if the length of str is less than cnt,
%	outputs   cnt - length(str)   zeros before the first digit or period
%	in the string (or at the end, if no such characters).
 putleft = proc (s: stream, str: string, cnt: int)
			signals (negative_field_width, not_possible(string))
	end putleft
%   modifies  s. 
%   effects  Outputs str. However, if the length of str is less than cnt,
%	outputs   cnt - length(str)   spaces after the string.
 putright = proc (s: stream, str: string, cnt: int)
			signals (negative_field_width, not_possible(string))
    end putright
%   modifies  s. 
%   effects  Outputs str. However, if the length of str is less than cnt,
%	outputs    cnt - length(str)   spaces before the string.
 putspace = proc (s: stream, cnt: int) 
			signals (negative_field_width, not_possible(string))
    end putspace
%   modifies  s.
%   effects  Outputs cnt spaces on s.
 putc_image = proc (s: stream, c: char)  signals (not_possible(string))
    end putc_image
%   modifies  s. 
%   effects  Like STREAM$PUTC, except that an arbitrary character may be 
%	written and the character is not interpreted by the CLU I/O system.
%	(For example, the ITS XGP program expects a text file containing
%	certain escape sequences.  An escape sequence consists of a special 
%	character followed by a fixed number of arbitrary characters.  
%	These characters could be the same as an end-of-line mark, but they 
%	are recognized as data by their context.
%	On a record-oriented system, such characters would be part of the data.
%	In either case, writing a newline in image mode would not be 
%	interpreted by the CLU system as indicating an end-of-line.)
%	Characters written to a terminal stream with this operation can be 
%	used to cause terminal-dependent control functions.
 getc_image = proc (s: stream) returns (char) 
				signals (end_of_file, not_possible(string))
    end getc_image
%   modifies  s. 
%   effects  Provided to read escape sequences in text files, as might be 
%	written using STREAM$PUTC_IMAGE.  Using this operation inhibits the
%	recognition of end-of-line marks, where used.  When reading from a
%	terminal stream, the character is not echoed and is not subject to
%	interpretation as an editing command.
 puts_image = proc (s: stream, str: string) signals (not_possible(string))
    end puts_image
%   modifies  s. 
%   effects  Writes the characters in str using STREAM$PUTC_IMAGE. Naturally
%	it may be somewhat more efficient than doing a series of individual
%	PUTC_IMAGEs.
 gets_image = proc (s: stream, str: string) returns (string)
				signals (end_of_file, not_possible(string))
    end gets_image
%   modifies  s. 
%   effects  Reads characters until a terminating character (one in str) or 
%	end-of-file is seen. Using this operation inhibits the recognition 
%	of end-of-line marks, where used.  When reading from a terminal stream,
%	the characters read are not echoed and are not subject to 
%	interpretation as editing commands.
%	The characters up to the terminator are returned; the terminator (if 
%	any) is left in the stream.
%	This operation signals end_of_file only if there are no characters 
%	and end-of-file is detected.
 reset = proc (s: stream) signals (not_possible(string))
    end reset
%   modifies  s. 
%   effects  Resets s so that the next input or output operation will read 
%	or write the first character in the file. The line number is reset
%	to its initial value. The end-of-file status is cleared on a 
%	terminal stream.
 flush = proc (s: stream) signals (not_possible(string))
    end flush
%   modifies  s. 
%   effects  Any output buffered on s is written to the file, if possible. This
%	operation should be used for streams that record the progress of a 
%	program. It can be used to maximize the amount of recorded status 
%	visible to the user or available in case the program terminates
%	prematurely.
 close = proc (s: stream) signals (not_possible(string))
    end close
%   modifies  s. 
%   effects  Attempts to terminate I/O and remove the association between
%	the stream and the file.  If successful, further use of operations 
%	that signal not_possible will signal not_possible.
%	This operation will fail if buffered output cannot be written.
 abort = proc (s: stream)
    end abort
%   modifies  s. 
%   effects  Terminates I/O and removes the association between the stream 
%	and the file.
%	If buffered output cannot be written, it will be lost, and if a new
%	file is being written, it may or may not exist.
 is_closed = proc (s: stream) returns (bool)
    end is_closed
%   effects  Returns true if and only if the stream is closed.
 is_terminal = proc (s: stream) returns (bool) 
    end is_terminal
%   effects  Returns true if and only if the stream is attached to
%	an interactive terminal (see TERMINAL_I/O).
 get_lineno = proc (s: stream) returns (int) 
				signals (end_of_file, not_possible(string)) 
    end get_lineno
%   effects  Returns the line number of the current (being or about to be read)
%	line.  If the system maintains explicit line numbers in the file,
%	said line numbers are returned.  Otherwise lines are implicitly
%	numbered, starting with 1.
 set_lineno = proc (s: stream, num: int) signals (not_possible(string))
    end set_lineno
%   modifies  s. 
%   effects  If the system maintains explicit line numbers in the file, sets 
%	the line number of the next (not yet started) line to num. Otherwise
%	it is ignored.
 get_line_length = proc (s: stream) returns (int) signals (no_limit) 
    end get_line_length
%   effects  If the file or device to which the stream is attached has a 
%	natural maximum line length, that length is returned. Otherwise 
%	no_limit is signaled.  The line length does not include newline 
%	characters.
 get_page_length = proc (s: stream) returns (int) signals (no_limit) 
    end get_page_length
%   effects  If the device to which the stream is attached has a natural 
%	maximum page length, that length is returned. Otherwise no_limit is 
%	signaled.  Storage files will generally not have page lengths.
 get_date = proc (s: stream) returns (date) signals (not_possible(string)) 
    end get_date
%   effects  Returns the date of the last modification of the corresponding 
%	storage file.
 set_date = proc (s: stream, d: date) signals (not_possible(string))
    end set_date
%   modifies  s. 
%   effects  Sets the modification date of the corresponding storage file.
%	The modification date is set automatically when a file is opened in
%	"write" or "append" mode.
 get_name = proc (s: stream) returns (file_name) signals (not_possible(string))
    end get_name
%   effects  Returns the name of the corresponding file.  It may be different
%	than the name used to open the file, in that defaults have been 
%	resolved and link indirections have been followed.
 set_output_buffered = proc (s: stream, b: bool) signals (not_possible(string))
    end set_output_buffered
%   modifies  s. 
%   effects  Sets the output buffering mode. Normally output may be arbitrarily
%	buffered before it is actually written out. Unbuffered output can be 
%	used on some systems to decrease the amount of information lost if 
%	the program terminates prematurely.
%	For terminal streams, unbuffered output is useful in programs that 
%	output incomplete lines as they are working to allow the user to watch
%	the progress of the program.
 get_output_buffered = proc (s: stream) returns (bool) 
    end get_output_buffered
%   effects  Returns true if and only if output to the stream is being buffered.
 equal = proc (s1, s2: stream) returns (bool) 
    end equal
%   effects  Returns true if and only if both arguments are the same stream.
 similar = proc (s1, s2: stream) returns (bool)
    end similar
%   effects  Returns true if and only both arguments are the same stream.
 copy = proc (s: stream) returns (stream)
    end copy
%   effects  Returns a stream equal to s.
 add_script = proc(s1, s2: stream) signals (script_failed) end add_script
%   modifies s1
%   effects  Adds s2 as a script stream of s1. All subsequent input from
%            and output to s1 will also be output to s2. "Not_possible"
%            exceptions that arise in actually outputting to s2 will be
%            ignored. This operation signals "script_failed" if s2 cannot
%            be written to or if either stream is a direct or indirect script
%            stream of the other.
 rem_script = proc(s1, s2: stream) end rem_script
%   modifies s2
%   effects  Removes, but does not close, s2 as a direct script stream of s1.
 unscript = proc(s: stream) end unscript
%   effects  Removes, but does not close, all direct script streams of s.

end stream
