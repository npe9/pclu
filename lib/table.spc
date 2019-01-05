% A mutable mapping from objects of one arbitrary type to another

table = cluster [key, val: type] is	create, flush, cond_flush, pcond_flush,
					bind, mbind, alter, lookup, mlookup,
					get_pair, exists, remove, delete,
					rehash, elements, count, empty, sizes

tab	= table[key, val]
htype	= proctype (key, int) returns (int)
ctype	= proctype (key, key) returns (bool)
ptype	= proctype (val) returns (bool)
rep	= any

% Create a table.  SIZE indicates the number of hashing buckets to use:
%	1 <= SIZE
% HASHER is the hashing function:
%	0 <= HASHER(key, SIZE) <= SIZE - 1
% for all keys.
% COMPER is an equivalence relation on keys.  For any K, K' is the key in the
% table such that COMPER(K, K') holds.  K' exists if and only if it has a
% value bound to it.
% For things to work, it must be the case that
%	COMPER(K, K')  implies  HASHER(K, SIZE) = HASHER(K', SIZE)

create = proc (size: int, hasher: htype, comper: ctype) returns (tab) signals (bad_size)
	end create

% Removes all bindings

flush = proc (t: tab)
	end flush

% Flushes all (key, val) pairs for which PRED(val) is true

cond_flush = proc (t: tab, pred: ptype)
	end cond_flush

% Flushes all (key, val) pairs for which PRED(val, THING) is true

pcond_flush = proc [u: type] (t: tab, pred: pptype, thing: u)
	pptype = proctype (val, u) returns (bool)
	end pcond_flush

% Bind the value V to the key K.  Signal EXISTS if K' exists.

bind = proc (t: tab, k: key, v: val) signals (exists)
	end bind

% If K' exists, return the value bound to it, else bind V to K and return V.

mbind = proc (t: tab, k: key, v: val) returns (val)
	end mbind

% Bind the value V to the key K', if it exists, else to K.

alter = proc (t: tab, k: key, v: val)
	end alter

% Get the value bound to K'.  Signal NOT_FOUND if K' does not exist.

lookup = proc (t: tab, k: key) returns (val) signals (not_found)
	end lookup

% Get the value bound to K'.  Return DEFV if K' does not exist.

mlookup = proc (t: cvt, k: key, defv: val) returns (val)
	end mlookup

% Get the value bound to K', and return both K' and the value.
% Signal NOT_FOUND if K' does not exist.

get_pair = proc (t: tab, k: key) returns (key, val) signals (not_found)
	end get_pair

% Test if the key K' exists.

exists = proc (t: tab, k: key) returns (bool)
	end exists

% Remove the value bound to key K' and return it.  Signal NOT_FOUND if K' does not
% exist.  Afterwards K' does not exist.

remove = proc (t: tab, k: key) returns (val) signals (not_found)
	end remove

% Remove the value bound to key K', if any.  Afterwards K' does not exist.

delete = proc (t: tab, k: key)
	end delete

% Rebuild the hash table with a new HASHER.

rehash = proc (t: tab, hasher: htype)
	end rehash

% Yield all (key, value) pairs in the table in random order

elements = iter (t: tab) yields (key, val)
	end elements

% Return the total number of (key, value) pairs

count = proc (t: tab) returns (int)
	end count

% Test if table is empty

empty = proc (t: tab) returns (bool)
	end empty

% Return an array of integers with element i being the number of buckets with i
% (key, val) pairs

sizes = proc (t: tab) returns (array[int])
	end sizes

end table


% A hashing function for strings.

hash = proc (s: string, size: int) returns (int)
	end hash

% A hashing function for integers.

hash_int = proc (i, size: int) returns (int)
	end hash_int

% A hashing function for objects
%  added 8/28/90 dcurtis

hash_obj = proc (o: _obj, max: int) returns (int)
	end hash_obj
