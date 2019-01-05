% This data type is similar to oneof[exists: t, none: null],
% but it doesn't require any additional storage.
% However, its operations also aren't inlined by the compiler and some
% instantiations can cause maybe$make to signal failure.

% Restrictions to guarantee that make doesn't signal failure:
%	1. The type t is not an instantiation of maybe.
%	   I.e., maybe[maybe[...]]$make can signal failure.
%	   If t is a cluster whose primitive rep is an instantiation of
%	   maybe, maybe[t] is not supported.  A useful convention is not
%	   to use reps that are instantiations of maybe.
%	2. Do not call maybe[int]$make(Smallest_Integer), which signals
%	   failure.  Maybe uses the smallest integer to represent the "none"
%	   value.
%	3. maybe[real] is a bad idea, since the smallest integer is the
%	   same as a real number slightly less than zero.

% Note: Under UNIX, the smallest integer is a kernel address, so maybe[t]
% is guaranteed to work when t is an array, record, sequence, or struct.
% If t is an abstract type whose rep is guaranteed to work, maybe[t] is
% also guaranteed to work provided that t is not an intantiation of maybe.
% Other instantiations may fail by signalling failure when make is called.

maybe = cluster [t: type] is none, make, exists, value, equal, similar, copy,
                             _gcd

rep = any

none = proc () returns (cvt)
    % return the object with the NONE tag
    end none

make = proc (x: t) returns (cvt)
    % return a maybe with tag EXISTS and value X
    end make

exists = proc (m: cvt) returns (bool)
    % test if M has tag EXISTS
    end exists

value = proc (m: cvt) returns (t) signals (wrong_tag)
    % return the value of M if the tag is EXISTS, else signal wrong_tag
    end value

equal = proc (m1, m2: cvt) returns (bool)
	    where t has equal: proctype (t, t) returns (bool)
    % true if both have tag NONE or
    % both have tag EXISTS and the values are t$equal
    end equal

similar = proc (m1, m2: cvt) returns (bool)
	    where t has similar: proctype (t, t) returns (bool)
    % true if both have tag NONE or
    % both have tag EXISTS and the values are t$similar
    end similar

copy = proc (m: cvt) returns (cvt)
	    where t has copy: proctype (t) returns (t)
    % if M has tag NONE, return it
    % else return a maybe with tag EXISTS and value t$copy(value(m))
    end copy

_gcd = proc (m: cvt, tab: gcd_tab) returns (int)
	    where t has _gcd: proctype (t, gcd_tab) returns (int)
    end _gcd

end maybe
