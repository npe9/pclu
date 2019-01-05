% Copyright Massachusetts Institute of Technology 1982, 1989

# extend

gcd_tab = cluster is grind, grindc, grindp

    rep = record[next:  int,        % next address to allocate
		 cnt:   int,        % gc count
		 oq:    aobj,       % objects to process
		 iq:    ainf,       % info to process
		 bucks: veco]       % hash chains

    aobj = array[_obj]
    ainf = array[ginfo]

    veco = _vec[_obj]
    tago = _tagcell[_obj]
    advo = _adv[_obj]
    seqo = sequence[_obj]

    ginfo = oneof[a_bvec: null,
		  b_vec:  gproc,
		  c_sel:  gproclist,
		  d_cell: gproclist,
		  e_sell: gproc,
		  f_adv:  _obj,
		  g_arp:  gproc]

    gproc = proctype (_obj, gcd_tab) returns (int)
    gproclist = sequence[gproc]

    grind = proc [t: type] (x: t, fn: file_name) returns (file_name)
	      signals (not_possible(string))
		where t has _gcd: proctype (t, gcd_tab) returns (int)
	end grind

    % untested for pmax version, probably works
    grindc = proc [t: type] (x: t, ch: _chan, pos: int)
	       signals (not_possible(string))
		where t has _gcd: proctype (t, gcd_tab) returns (int)
	end grindc

    % untested for pmax version, may work
    grindp = proc [t: type] (x: t, ch: _chan) signals (not_possible(string))
		where t has _gcd: proctype (t, gcd_tab) returns (int)
	end grindp

    % untested for pmax version, may work
    pgrind = proc [t: type] (tab: gcd_tab, x: t)
		where t has _gcd: proctype (t, gcd_tab) returns (int)
	end pgrind

    % reworked for pmax version of clu
    mgrind = proc [t: type] (tab: gcd_tab, buf: _wordvec, ch: _chan, x: t)
	       signals (not_possible(string))
		where t has _gcd: proctype (t, gcd_tab) returns (int)
	end mgrind

    insert = proc (tab: cvt, z: int, inf: ginfo, x: _obj) returns (int)
	end insert

    end gcd_tab

gcd_store = cluster is init, store_id, store_lit, store_ref, store_hdr,
		       store_array_desc, store_string, done

    rep = null
    advo = _adv[_obj]

    init = proc(w: _wordvec, c:_chan)
	end init

    store_id = proc()

	end store_id

    store_lit = proc(i1: int)
	end store_lit

    store_ref = proc(i1: int)
	end store_ref

    store_hdr = proc(hid, size: int)
	end store_hdr

    store_array_desc = proc(a: advo)
	end store_array_desc

    store_string = proc(s: _bytevec)
	end store_string

    done = proc()
	end done

    need = proc(i:int) returns (int)
	end need

    end gcd_store

