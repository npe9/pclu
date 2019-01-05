%
%     specification for routine that will invoke
%	a builtin procedure
%	stack contains the arguments as _objs
%	signature contains # ant type of args, results, signals, signal values

%	the first element of the stack on exit is
%		either the name of the signal
%		or an empty string for the normal case
%

call_clu = proc(addr: int, type_ops: int, op_ops: int, stck: _vec[_obj], sp: int, 
		is_iter, is_sel: bool, nargs, nvals: int) 
	end call_clu

CLU_add_parm_info_const = proc(nth: int, val: _obj)
	end CLU_add_parm_info_const

CLU_add_parm_info_type = proc(nth: int, ops: int, reqs: int)
	end CLU_add_parm_info_type

CLU_find_type_instance = proc(ops: int, nparms: int, ownreqs: int) returns (int)
	end CLU_find_type_instance

CLU_find_typeop_instance = proc(ops: int, op: int, nparms: int, ntparms: int,
			op_own_reqs: int, type_own_reqs: int) returns (int)
	end CLU_find_typeop_instance

CLU_find_prociter_instance = proc(op: int, nparms: int, ownreqs: int) returns (int)
	end CLU_find_prociter_instance

add_selector_info2 = proc(field_name: string, n: int, ops: int)
	end add_selector_info2

find_selector_ops2 = proc(typ: string, nfields: int) returns (int)
	end find_selector_ops2

% results are valcount, valops, alt_op_num
find_valops = proc(typ: string, op: string, tops: int) returns (int, ops, int)
	end find_valops

% results are type name, valcount, valops, alt_op_num
%	e.g. "record",                     make_f1 => n where f1 is the nth field
rep_find_valops = proc(op: string, tops: int) returns (string, int, ops, int)
	end rep_find_valops

% given the address of an instantiated type, 
%	return abstract ops for that type, the number of parameters, and the index of the instantiation

find_tgen = proc(tops: ops) returns (ops, int, int) signals (not_found)
	end find_tgen

% given the index of an instantiation and and the index of the parameter for that instantiation
%	return ops for that parameter

find_tgen_parm = proc(index: int, ith: int) returns (ops)
	end find_tgen_parm

% results are value corresponding to nm in module mod at stack index ind
%	and its operations
debugopnm2val_ops = proc(ind: int, mod, nm: string) returns (_obj, int)
	end debugopnm2val_ops

%
%	specification for routine that will invoke
%	  the unix system call
%		(takes a string and causes a shell to execute it)
%

unix_cmd = proc(cmd: string)
	end unix_cmd

%
%	routine to compare two signal names
%	(conforms to pclu calling conventions)
%

errcmp2 = proc(str1, str2: _obj) returns (bool)
	end errcmp2

%
%	routine to convert signal name to string
%

_pclu_erstr2 = proc(signm: _obj) returns (_obj)
	end _pclu_erstr2

debugOPenter_proc = proc(localsp: _vec[int])
	end debugOPenter_proc

debugOPenter_forbody = proc(localsp: _vec[int])
	end debugOPenter_forbody

debugOPleave_proc = proc(sig: int)
	end debugOPleave_proc

debugOPbegin_line = proc(lineno: int)
	end debugOPbegin_line

debugOPline_stop = proc(module: string, lineno: int)
	end debugOPline_stop

debugOPclear_line_stop = proc(module: string, lineno: int)
	end debugOPclear_line_stop

debugOPfunc_stop = proc(module: string)
	end debugOPfunc_stop

debugOPclear_func_stop = proc(module: string)
	end debugOPclear_func_stop

debugOPassign = proc(addr: int, val: int)
	end debugOPassign

debugOPprint_val = proc(po: pstream, stack_index: int, mod, nm: string)
	end debugOPprint_val

debugOPprint_val2 = proc(po: pstream, val: _obj, tops: _vec[_obj])
	end debugOPprint_val2

debugOPget_val = proc(stack_index: int, module: string, nm: string) returns (_obj)
	end debugOPget_val

debugOPset_val = proc(stack_index: int, module: string, nm: string, val: _obj)
	end debugOPset_val

debugOPframe_count = proc() returns (int)
	end debugOPframe_count

debugOPframe_info = proc(nth: int) returns (rtn_info, int) signals (none)
	end debugOPframe_info

debugOPget_name = proc(ri: rtn_info) returns (string)
	end debugOPget_name

debugOPget_fname = proc(ri: rtn_info) returns (string)
	end debugOPget_fname

debugOPget_nth_formal = proc(ref: int, n: int) returns (string)
	end debugOPget_nth_formal

debugOPget_nth_type_formal = proc(ri: rtn_info, n: int) returns (string)
	end debugOPget_nth_type_formal

debugOPget_nth_op_formal = proc(ri: rtn_info, n: int) returns (string)
	end debugOPget_nth_op_formal

debugOPget_nth_op = proc(vals:ops, n:int, info: rtn_info, tops, oops: int) returns (anop) 
				signals (not_found (string))
	end debugOPget_nth_op

debugOPget_nth_sigval = proc(i:int) returns (_obj)
	end debugOPget_nth_sigval

debugOPcmp_sig = proc(s1: string, s2: string) returns (bool)
	end debugOPcmp_sig

debugOPcvt_sig = proc(s1: int) returns (string)
	end debugOPcvt_sig

debugOPget_sigvalcount = proc(sd: sigdesc) returns (int)
	end debugOPget_sigvalcount

debugOPget_nth_sigvalops = proc(sd: sigdesc, n: int, info:rtn_info, tops, oops: int) returns (anop)
	end debugOPget_nth_sigvalops

debugOPget_count_formals = proc(ref: int) returns (int)
	end debugOPget_count_formals
