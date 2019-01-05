
/* Copyright Massachusetts Institute of Technology 1990,1991 */

#ifndef lint
static char rcsid[] = "$Header: /pm/src/site/pclu/code/base/RCS/string.c,v 1.6 91/08/29 14:59:18 dcurtis Exp $";
#endif
/* $Log:	string.c,v $
 * Revision 1.6  91/08/29  14:59:18  dcurtis
 * removed elist from oneofOPnew call
 * 
 * Revision 1.5  91/06/06  13:31:15  root
 * added copyright notice
 * 
 * Revision 1.4  91/06/03  14:42:26  root
 * sparcstation compatibility: int->CLUREF
 * 
 * Revision 1.3  91/05/31  12:51:31  root
 * fixed aggregate initialization in ops_actual
 * added elist argument to various cons & cons0 calls
 * 
 * Revision 1.2  91/05/07  14:21:23  root
 * Switched from clu_alloc to clu_alloc_atomic.
 *    mtv notes: Because sizeof(CLU_string) is 12 rather than 9 on some C compilers
 *               (including MIPS), extra storage is allocated for many strings.
 * Added return value to pstream$text(c) calls
 * 
 * Revision 1.1  91/02/04  15:49:53  mtv
 * Initial revision
 * 
 */

/*								*/
/*		STRING IMPLEMENTATION				*/
/*								*/

/* Maybe use bcopy? Is it efficient as is?  Is bcopy portable? */

/* TODO: sc2s, s2sc */

#include "pclu_err.h"
#include "pclu_sys.h"

extern errcode stringOPprint();
extern errcode stringOPcons();

errcode stringOPsize(s, ans)
CLUREF s;
CLUREF *ans;
{
	ans->num = s.str->size;
	signal(ERR_ok);
	}

errcode stringOPindexs(find, s, ans)
CLUREF find, s, *ans;
{
long i,j;
bool found;
	for (i = 0; i < s.str->size; i++) {
		found = true;
		for (j = 0; j < find.str->size; j++) {
			if (s.str->data[i+j] != find.str->data[j]) {
				found = false;
				break;
				}
			}
		if (found == true) {
			ans->num = i + 1;
			signal(ERR_ok);
			}
		}
	ans->num = 0;
	signal(ERR_ok);
	}

errcode stringOPindexc(find, s, ans)
CLUREF find, s, *ans;
{
long i;
	for (i = 0; i < s.str->size; i++) {
		if (s.str->data[i] == find.ch) {
			ans->num = i + 1;
			signal(ERR_ok);
			}
		}
	ans->num = 0;
	signal(ERR_ok);
	}

errcode stringOPcons(buf, start, len, ans)
C_STRING buf;
CLUREF start, len;
CLUREF *ans;
{
CLUREF temp;
long i,j,count;

	clu_alloc_atomic(((len.num+1+CLUREFSZ) & ~(CLUREFSZ-1))
			 + sizeof(CLU_string) - 1, &temp);
	temp.str->size = len.num;
	temp.str->typ.val = CT_STRING;
	temp.str->typ.mark = 0;
	temp.str->typ.refp = 0;

	for (count = 0, i = start.num - 1, j = 0; count < len.num; i++, j++, count++) {
		temp.str->data[j] = buf[i];
		}
		temp.str->data[j] = '\000';
	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode stringOPcons0(len, ans)
CLUREF len;
CLUREF *ans;
{
errcode err;
CLUREF temp;

	clu_alloc_atomic(((len.num+1+CLUREFSZ) & ~(CLUREFSZ-1))
			 + sizeof(CLU_string) - 1, &temp);
	temp.str->size = len.num;
	temp.str->typ.val = CT_STRING;
	temp.str->typ.mark = 0;
	temp.str->typ.refp = 0;

	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode stringOPc2s(ch, ans)
CLUREF ch;
CLUREF *ans;
{
errcode err;
CLUREF temp;

	err = stringOPcons0(CLU_1, &temp);
	if (err != ERR_ok) resignal(err);
	temp.str->data[0] = ch.ch;
	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode stringOPconcat(s1, s2, ans)
CLUREF s1, s2, *ans;
{
errcode err;
CLUREF temp;
long size, i, j;
	if (s1.str->size == 0) {
		ans->str = s2.str;
		signal(ERR_ok);
		}
	if (s2.str->size == 0) {
		ans->str = s1.str;
		signal(ERR_ok);
		}
	size = s1.str->size + s2.str->size;
	if (size > MAX_STR) {
		elist[0].str = huge_allocation_request_STRING;
		signal(ERR_failure);
		}
	clu_alloc_atomic(((size+sizeof(CLUREF)) & ~(sizeof(CLUREF)-1)) + 
				sizeof(CLU_string) -1, &temp);
	temp.str->size = size;
	temp.str->typ.val = CT_STRING;
	temp.str->typ.mark = 0;
	temp.str->typ.refp = 0;

	for (i = 0; i < s1.str->size; i++)
		temp.str->data[i] = s1.str->data[i];

	for (i = 0, j = s1.str->size; i < s2.str->size; i++, j++)
		temp.str->data[j] = s2.str->data[i];
	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode stringOPappend(s, c, ans)
CLUREF s, c, *ans;
{
long size;
errcode err;
CLUREF temp;
long i;
	size = s.str->size + 1;
	if (size > MAX_STR) {
		elist[0].str = huge_allocation_request_STRING;
		signal(ERR_failure);
		}
	clu_alloc_atomic(((size+CLUREFSZ) & ~(CLUREFSZ-1))
				 + sizeof(CLU_string) -1, &temp);
	temp.str->size = size;
	temp.str->typ.val = CT_STRING;
	temp.str->typ.mark = 0;
	temp.str->typ.refp = 0;

	for (i = 0; i < s.str->size; i++)
		temp.str->data[i] = s.str->data[i];
	temp.str->data[size-1] = c.ch;
	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode stringOPempty(s, ans)
CLUREF s;
CLUREF *ans;
{
	if (s.str->size == 0)
		{ans->tf = true;}
	else
		{ans->tf = false;}
	signal(ERR_ok);
	}

errcode stringOPsubstr(s, start, len, ans)
CLUREF s;
CLUREF start, len;
CLUREF *ans;
{
errcode err;
CLUREF temp;
CLUREF count;
	if (start.num <= 0) signal(ERR_bounds);
	if (start.num > (s.str->size + 1)) signal(ERR_bounds);
	if (len.num < 0) signal(ERR_negative_size);
	/* inlined int$min 8/28/90 dcurtis */
	count.num = s.str->size - start.num + 1;
	if (len.num < count.num) count.num = len.num;
	err = stringOPcons(s.str->data, start, count, &temp);
	if (err != ERR_ok) resignal(err);
	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode stringOPrest(s, start, ans)
CLUREF s;
CLUREF start;
CLUREF *ans;
{
errcode err;
CLUREF temp;
CLUREF count;
	if (start.num <= 0) signal(ERR_bounds);
	if (start.num > s.str->size + 1) signal(ERR_bounds);
	count.num = s.str->size - start.num + 1;
	err = stringOPcons(s.str->data, start, count, &temp);
	if (err != ERR_ok) resignal(err);
	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode stringOPs2ac(s, ans)
CLUREF s, *ans;
{
CLUREF temp, c, num;
errcode err;
long i, j;

	num.num = s.str->size;
	err = arrayOPpredict(CLU_1, num, &temp);
	if (err != ERR_ok) resignal(err);

	j = temp.array->int_low;
	c.num = 0;
	for (i = 0; i < s.str->size; i++, j++) {
		c.ch = s.str->data[i];
		temp.array->store->data[j] = c.num;
		}
	temp.array->ext_size = s.str->size;
	temp.array->ext_high = s.str->size;
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode stringOPac2s(a, ans)
CLUREF a, *ans;
{
CLUREF temp, c, num;
errcode err;
long i, j;

	num.num = a.array->ext_size;
	err = stringOPcons0(num, &temp);
	if (err != ERR_ok) resignal(err);

	j = a.array->int_low;
	for (i = 0; i < a.array->ext_size; i++, j++) {
		c.num = a.array->store->data[j];
		temp.str->data[i] = c.ch;
		}
	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode stringOPs2sc(s, ans)
CLUREF s, *ans;
{
CLUREF temp, num;
errcode err;
long i;

	num.num = s.str->size;
	err = sequenceOPfill(num, CLU_0, &temp);
	if (err != ERR_ok) resignal(err);

	for (i = 0; i < s.str->size; i++) {
		temp.vec->data[i] = s.str->data[i];
		}
	ans->vec = temp.vec;
	signal(ERR_ok);
	}

errcode stringOPsc2s(a, ans)
CLUREF a, *ans;
{
CLUREF temp, num;
errcode err;
long i;

	num.num = a.vec->size;
	err = stringOPcons0(num, &temp);
	if (err != ERR_ok) resignal(err);

	for (i = 0; i < a.vec->size; i++) {
		temp.str->data[i] = a.vec->data[i];
		}
	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode stringOPchars(s, iterbod, locals, ecode2)
CLUREF s;
char *locals;
errcode (*iterbod)();
errcode *ecode2;
{
errcode err;
errcode ecode;
CLUREF iv_1;
long index;

	if (s.str->size == 0) signal(ERR_ok);
	index = 0;
	while (true) {
		iv_1.num = 0;
		iv_1.ch = s.str->data[index];
		err = iterbod(iv_1, locals, ecode2);
		if (err == ERR_ok) {
			index += 1;
			if (index < s.str->size) continue;
			signal(ERR_ok);
			}
	        if (err == ERR_iterbodyreturn) signal(err);
		/* 9/9/93 dwc: just propagate break, do not convert to ok */
		if (err == ERR_break) signal(ERR_break);
		if (err == ERR_iterbodyexit) signal(err);
		if (err == ERR_iterbodysignal) signal(err);
		if (err == ERR_iteriterbodysignal) signal(err);
		if (err == ERR_iteriterbodyexit) signal(err);
		if (err == ERR_iteriterbodyreturn) signal(err);
		}
	}

errcode stringOPlt(s1, s2, ans)
CLUREF s1, s2, *ans;
{
long size, i;
	
	ans->tf = true;
	size = s1.str->size;
	if (s2.str->size < size) size = s2.str->size;
	for (i = 0; i < size; i++) {
		if (s1.str->data[i] != s2.str->data[i]) {
			if (s1.str->data[i] >= s2.str->data[i]) ans->tf = false;
			signal(ERR_ok);
			}
		}
	if (s1.str->size >= s2.str->size) ans->tf = false;
	signal(ERR_ok);
	}


errcode stringOPle(s1, s2, ans)
CLUREF s1, s2, *ans;
{
long size, i;
	
	ans->tf = true;
	size = s1.str->size;
	if (s2.str->size < size) size = s2.str->size;
	for (i = 0; i < size; i++) {
		if (s1.str->data[i] != s2.str->data[i]) {
			if (s1.str->data[i] > s2.str->data[i]) ans->tf = false;
			signal(ERR_ok);
			}
		}
	if (s1.str->size > s2.str->size) ans->tf = false;
	signal(ERR_ok);
	}


errcode stringOPge(s1, s2, ans)
CLUREF s1, s2, *ans;
{
long size, i;
	
	ans->tf = true;
	size = s1.str->size;
	if (s2.str->size < size) size = s2.str->size;
	for (i = 0; i < size; i++) {
		if (s1.str->data[i] != s2.str->data[i]) {
			if (s1.str->data[i] < s2.str->data[i]) ans->tf = false;
			signal(ERR_ok);
			}
		}
	if (s1.str->size < s2.str->size) ans->tf = false;
	signal(ERR_ok);
	}


errcode stringOPgt(s1, s2, ans)
CLUREF s1, s2, *ans;
{
long size, i;
	
	ans->tf = true;
	size = s1.str->size;
	if (s2.str->size < size) size = s2.str->size;
	for (i = 0; i < size; i++) {
		if (s1.str->data[i] != s2.str->data[i]) {
			if (s1.str->data[i] <= s2.str->data[i]) ans->tf = false;
			signal(ERR_ok);
			}
		}
	if (s1.str->size <= s2.str->size) ans->tf = false;
	signal(ERR_ok);
	}

errcode stringOPfetch(s, i, ans)
CLUREF s;
CLUREF i;
CLUREF *ans;
{
	if (i.num < 1) signal(ERR_bounds);
	if (i.num > s.str->size) signal(ERR_bounds);
	ans->ch = s.str->data[i.num-1];
	signal(ERR_ok);
	}

errcode stringOPequal(s1, s2, ans)
CLUREF s1, s2;
CLUREF *ans;
{
long i;
	if (s1.str->size == s2.str->size) {
		for (i = 0; i < s1.str->size; i++) {
			if (s1.str->data[i] == s2.str->data[i]) continue;
			ans->tf = false;
			signal(ERR_ok);
			}
		ans->tf = true;
		signal(ERR_ok);
		}
	else {
		ans->tf = false;	
		signal(ERR_ok);
		}
	}

errcode stringOPsimilar(s1, s2, ans)
CLUREF s1, s2;
CLUREF *ans;
{
long i;
	if (s1.str->size == s2.str->size) {
		for (i = 0; i < s1.str->size; i++) {
			if (s1.str->data[i] == s2.str->data[i]) continue;
			ans->tf = false;
			signal(ERR_ok);
			}
		ans->tf = true;
		signal(ERR_ok);
		}
	else {
		ans->tf = false;	
		signal(ERR_ok);
		}
	}

errcode stringOPcopy(s1, ans)
CLUREF s1;
CLUREF *ans;
{
	ans->str = s1.str;
	signal(ERR_ok);
	}

errcode stringOPdebug_print(s, pst)
CLUREF s, pst;
{
errcode err;
	err = stringOPprint(s, pst);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode stringOPprint(s, pst)
CLUREF s, pst;
{
errcode err;
CLUREF lim, max, temp_str, ch;
long i, count;
char nthch, lowch, outch;
CLUREF prefix, ans;
bool meta;
static bool init = false;
static CLUREF t0, t1, t2, t3, t4;

        if (init == 0) {
                err = stringOPcons("", CLU_1, CLU_0, &t0);
                err = stringOPcons("\\^", CLU_1, CLU_2, &t1);
                err = stringOPcons("\\!", CLU_1, CLU_2, &t2);
                err = stringOPcons("\\",  CLU_1, CLU_1, &t3);
                err = stringOPcons("\\&", CLU_1, CLU_2, &t4);
                init = true;
                }

	err = pstreamOPget_max_width(pst, &lim);
	if (err != ERR_ok) resignal(err);
	lim.num = lim.num*16 + 4;
	ch.ch = '"';
	err = pstreamOPtextc(pst, ch, &ans);
	if (err != ERR_ok) resignal(err);
	count = s.str->size;
	if (count != 0) {
/*	max.num = lim.num + 3; */
	for (i = 0; i < count; i++) {
		if (i >= lim.num) {
			err = stringOPcons("...", CLU_1, CLU_3, &temp_str);
			if (err != ERR_ok) resignal(err);
			err = pstreamOPtext(pst, temp_str, &ans);
			if (err != ERR_ok) resignal(err);
			break;
			}
		nthch = s.str->data[i];
		prefix = t0;
		meta = false;
		if ((nthch & 0x80) != 0) meta = true;
		lowch = nthch & 0x7f;
		outch = lowch;
		do {
			if (lowch == '\177') {
				prefix = t1;
				if (meta) prefix = t2;
				outch = '?';
				break;
				}
			if (lowch == '"' || lowch == '\\') {
				prefix = t3;
				if (meta) prefix = t4;
				break;
				}
			if (lowch >= ' ') {
				if (!meta) break;
				prefix = t4;
				break;
				}
			if (meta) {
				prefix = t2;
				outch += 0x40;
				break;
				}
			if (outch == '\n') {
				prefix = t3;
				outch = 'n';
				break;
				}	
			if (outch == '\t') {
				prefix = t3;
				outch = 't';
				break;
				}	
			if (outch == '\f') {
				prefix = t3;
				outch = 'p';
				break;
				}	
			if (outch == '\b') {
				prefix = t3;
				outch = 'b';
				break;
				}	
			if (outch == '\r') {
				prefix = t3;
				outch = 'r';
				break;
				}	
			if (outch == '\v') {
				prefix = t3;
				outch = 'v';
				break;
				}
			prefix = t1;
			outch += 0x40;

		} while (false);
		err = pstreamOPtext(pst, prefix, &ans);
		if (err != ERR_ok) resignal(err);
		ch.ch = outch;
		err = pstreamOPtextc(pst, ch, &ans);
		if (err != ERR_ok) resignal(err);
		}
		}
	ch.ch = '"';
	err = pstreamOPtextc(pst, ch, &ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);	
	}
errcode stringOPencode(s, ist)
CLUREF s, ist;
{
errcode err;

	err = istreamOPputbv(ist, s);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}

errcode stringOPdecode(ist, ans)
CLUREF ist, *ans;
{
errcode err;
CLUREF temp;

	err = istreamOPgetbv(ist, &temp);
	if (err == ERR_end_of_file) signal(err);
	if (err == ERR_not_possible) signal(err);
	if (err != ERR_ok) resignal(err);
	ans->str = temp.str;
	signal(ERR_ok);
	}

errcode stringOP_gcd(s, tab, ans)
CLUREF s, tab, *ans;
{
errcode err;
CLUREF temp_oneof, sz;

	err = oneofOPnew(CLU_1, CLU_0, &temp_oneof);
	if (err != ERR_ok) resignal(err);
/*                    8 for type + size, 1 for trailing 0, +3 &~3 to round up */
	sz.num = 2*CLUREFSZ + (s.str->size+1 + CLUREFSZ-1)&~(CLUREFSZ-1);
	err = gcd_tabOPinsert(tab, sz, temp_oneof, s, ans);
	if (err != ERR_ok) resignal(err);
	signal(ERR_ok);
	}


typedef struct {
long count;
OWNPTR type_owns;
OWNPTR op_owns;
struct OP_ENTRY entry[29];
} string_OPS;

OWN_ptr string_own_init = {1, 0};
CLU_proc string_oe_copy = {{0,0,0,0}, stringOPcopy, &string_own_init, &string_own_init};
CLU_proc string_oe_size = {{0,0,0,0}, stringOPsize, &string_own_init, &string_own_init};
CLU_proc string_oe_fetch = {{0,0,0,0}, stringOPfetch, &string_own_init, &string_own_init};
CLU_proc string_oe_equal = {{0,0,0,0}, stringOPequal, &string_own_init, &string_own_init};
CLU_proc string_oe_similar = {{0,0,0,0}, stringOPsimilar, &string_own_init, &string_own_init};
CLU_proc string_oe_indexs = {{0,0,0,0}, stringOPindexs, &string_own_init, &string_own_init};
CLU_proc string_oe_indexc = {{0,0,0,0}, stringOPindexc, &string_own_init, &string_own_init};
CLU_proc string_oe_cons = {{0,0,0,0}, stringOPcons, &string_own_init, &string_own_init};
CLU_proc string_oe_cons0 = {{0,0,0,0}, stringOPcons0, &string_own_init, &string_own_init};
CLU_proc string_oe_c2s = {{0,0,0,0}, stringOPc2s, &string_own_init, &string_own_init};
CLU_proc string_oe_concat = {{0,0,0,0}, stringOPconcat, &string_own_init, &string_own_init};
CLU_proc string_oe_append = {{0,0,0,0}, stringOPappend, &string_own_init, &string_own_init};
CLU_proc string_oe_empty = {{0,0,0,0}, stringOPempty, &string_own_init, &string_own_init};
CLU_proc string_oe_substr = {{0,0,0,0}, stringOPsubstr, &string_own_init, &string_own_init};
CLU_proc string_oe_rest = {{0,0,0,0}, stringOPrest, &string_own_init, &string_own_init};
CLU_proc string_oe_s2ac = {{0,0,0,0}, stringOPs2ac, &string_own_init, &string_own_init};
CLU_proc string_oe_ac2s = {{0,0,0,0}, stringOPac2s, &string_own_init, &string_own_init};
CLU_proc string_oe_s2sc = {{0,0,0,0}, stringOPs2sc, &string_own_init, &string_own_init};
CLU_proc string_oe_sc2s = {{0,0,0,0}, stringOPsc2s, &string_own_init, &string_own_init};
CLU_proc string_oe_chars = {{0,0,0,0}, stringOPchars, &string_own_init, &string_own_init};
CLU_proc string_oe_lt = {{0,0,0,0}, stringOPlt, &string_own_init, &string_own_init};
CLU_proc string_oe_le = {{0,0,0,0}, stringOPle, &string_own_init, &string_own_init};
CLU_proc string_oe_ge = {{0,0,0,0}, stringOPge, &string_own_init, &string_own_init};
CLU_proc string_oe_gt = {{0,0,0,0}, stringOPgt, &string_own_init, &string_own_init};
CLU_proc string_oe_print = {{0,0,0,0}, stringOPprint, &string_own_init, &string_own_init};
CLU_proc string_oe_encode = {{0,0,0,0}, stringOPencode, &string_own_init, &string_own_init};
CLU_proc string_oe_decode = {{0,0,0,0}, stringOPdecode, &string_own_init, &string_own_init};
CLU_proc string_oe__gcd = {{0,0,0,0}, stringOP_gcd, &string_own_init, &string_own_init};
CLU_proc string_oe_debug_print = {{0,0,0,0}, stringOPdebug_print, &string_own_init, &string_own_init};
string_OPS string_ops_actual = {29,
&string_own_init,
&string_own_init, {
{&string_oe_equal, "equal"},
{&string_oe_similar, "similar"},
{&string_oe_copy, "copy"},
{&string_oe_print, "print"},
{&string_oe_encode, "encode"},
{&string_oe_decode, "decode"},
{&string_oe__gcd, "_gcd"},
{&string_oe_debug_print, "debug_print"},
{&string_oe_size, "size"},
{&string_oe_fetch, "fetch"},
{&string_oe_indexs, "indexs"},
{&string_oe_indexc, "indexc"},
{&string_oe_cons, "cons"},
{&string_oe_cons0, "cons0"},
{&string_oe_c2s, "c2s"},
{&string_oe_concat, "concat"},
{&string_oe_append, "append"},
{&string_oe_empty, "empty"},
{&string_oe_substr, "substr"},
{&string_oe_rest, "rest"},
{&string_oe_s2ac, "s2ac"},
{&string_oe_ac2s, "ac2s"},
{&string_oe_s2sc, "s2sc"},
{&string_oe_sc2s, "sc2s"},
{&string_oe_chars, "chars"},
{&string_oe_lt, "lt"},
{&string_oe_le, "le"},
{&string_oe_ge, "ge"},
{&string_oe_gt, "gt"}}
};

string_OPS *string_ops = &string_ops_actual;
