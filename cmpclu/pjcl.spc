ac = array[char]
as = array[str]
str = string
command = record[opts: as,
		 cmnd: str,
		 args: as]
commandlist = array[command]

parse_command_line = proc (line: ac) returns (commandlist, str, bool)
	end parse_command_line

get_output_part = proc (line: ac) returns (str) signals (none)
	end get_output_part

get_option_parts = proc (line: ac) returns (as)
	end get_option_parts

get_option_part = proc (line: ac) returns (str) signals (none)
	end get_option_part

get_command_part = proc (line: ac) returns (str) signals (none)
	end get_command_part

get_input_parts = proc (line: ac) returns (as)
	end get_input_parts

get_input_part = proc (line: ac) returns (str)
	end get_input_part

move_to_next = proc (line: ac) signals (extra(str))
	end move_to_next

flush_blanks = proc (line: ac) returns (char)
	end flush_blanks

gets_to = proc (line: ac, terms: str) returns (str)
	end gets_to

unify = proc (line: ac)
	end unify
