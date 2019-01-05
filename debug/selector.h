
/* Copyright Massachusetts Institute of Technology 1990,1991 */

/* $Header: selector.h,v 1.2 91/06/06 13:33:20 root Exp $
 * $Log:	selector.h,v $
 * Revision 1.2  91/06/06  13:33:20  root
 * added copyright notice
 * 
 * Revision 1.1  91/02/04  15:49:59  mtv
 * Initial revision
 * 
 */

extern errcode recordOPget_1();
extern errcode recordOPset_1();
extern errcode recordOPget_2();
extern errcode recordOPset_2();
extern errcode recordOPget_3();
extern errcode recordOPset_3();
extern errcode recordOPget_4();
extern errcode recordOPset_4();
extern errcode recordOPget_5();
extern errcode recordOPset_5();
extern errcode recordOPget_6();
extern errcode recordOPset_6();
extern errcode recordOPget_7();
extern errcode recordOPset_7();
extern errcode recordOPget_8();
extern errcode recordOPset_8();
extern errcode recordOPget_9();
extern errcode recordOPset_9();
extern errcode recordOPget_10();
extern errcode recordOPset_10();
extern errcode recordOPget_11();
extern errcode recordOPset_11();
extern errcode recordOPget_12();
extern errcode recordOPset_12();
extern errcode recordOPget_13();
extern errcode recordOPset_13();
extern errcode recordOPget_14();
extern errcode recordOPset_14();
extern errcode recordOPget_15();
extern errcode recordOPset_15();
extern errcode recordOPget_16();
extern errcode recordOPset_16();
extern errcode recordOPget_17();
extern errcode recordOPset_17();
extern errcode recordOPget_18();
extern errcode recordOPset_18();
extern errcode recordOPget_19();
extern errcode recordOPset_19();
extern errcode recordOPget_20();
extern errcode recordOPset_20();
extern errcode recordOPget_21();
extern errcode recordOPset_21();
extern errcode recordOPget_22();
extern errcode recordOPset_22();
extern errcode recordOPget_23();
extern errcode recordOPset_23();
extern errcode recordOPget_24();
extern errcode recordOPset_24();
extern errcode recordOPget_25();
extern errcode recordOPset_25();

PROC *record_prefix_fcn_table[50] = {
    recordOPget_1,
    recordOPset_1,
    recordOPget_2,
    recordOPset_2,
    recordOPget_3,
    recordOPset_3,
    recordOPget_4,
    recordOPset_4,
    recordOPget_5,
    recordOPset_5,
    recordOPget_6,
    recordOPset_6,
    recordOPget_7,
    recordOPset_7,
    recordOPget_8,
    recordOPset_8,
    recordOPget_9,
    recordOPset_9,
    recordOPget_10,
    recordOPset_10,
    recordOPget_11,
    recordOPset_11,
    recordOPget_12,
    recordOPset_12,
    recordOPget_13,
    recordOPset_13,
    recordOPget_14,
    recordOPset_14,
    recordOPget_15,
    recordOPset_15,
    recordOPget_16,
    recordOPset_16,
    recordOPget_17,
    recordOPset_17,
    recordOPget_18,
    recordOPset_18,
    recordOPget_19,
    recordOPset_19,
    recordOPget_20,
    recordOPset_20,
    recordOPget_21,
    recordOPset_21,
    recordOPget_22,
    recordOPset_22,
    recordOPget_23,
    recordOPset_23,
    recordOPget_24,
    recordOPset_24,
    recordOPget_25,
    recordOPset_25,
    };


extern errcode structOPget_1();
extern errcode structOPreplace_1();
extern errcode structOPget_2();
extern errcode structOPreplace_2();
extern errcode structOPget_3();
extern errcode structOPreplace_3();
extern errcode structOPget_4();
extern errcode structOPreplace_4();
extern errcode structOPget_5();
extern errcode structOPreplace_5();
extern errcode structOPget_6();
extern errcode structOPreplace_6();
extern errcode structOPget_7();
extern errcode structOPreplace_7();
extern errcode structOPget_8();
extern errcode structOPreplace_8();
extern errcode structOPget_9();
extern errcode structOPreplace_9();
extern errcode structOPget_10();
extern errcode structOPreplace_10();
extern errcode structOPget_11();
extern errcode structOPreplace_11();
extern errcode structOPget_12();
extern errcode structOPreplace_12();
extern errcode structOPget_13();
extern errcode structOPreplace_13();
extern errcode structOPget_14();
extern errcode structOPreplace_14();
extern errcode structOPget_15();
extern errcode structOPreplace_15();
extern errcode structOPget_16();
extern errcode structOPreplace_16();
extern errcode structOPget_17();
extern errcode structOPreplace_17();
extern errcode structOPget_18();
extern errcode structOPreplace_18();
extern errcode structOPget_19();
extern errcode structOPreplace_19();
extern errcode structOPget_20();
extern errcode structOPreplace_20();
extern errcode structOPget_21();
extern errcode structOPreplace_21();
extern errcode structOPget_22();
extern errcode structOPreplace_22();
extern errcode structOPget_23();
extern errcode structOPreplace_23();
extern errcode structOPget_24();
extern errcode structOPreplace_24();
extern errcode structOPget_25();
extern errcode structOPreplace_25();

PROC *struct_prefix_fcn_table[50] = {
    structOPget_1,
    structOPreplace_1,
    structOPget_2,
    structOPreplace_2,
    structOPget_3,
    structOPreplace_3,
    structOPget_4,
    structOPreplace_4,
    structOPget_5,
    structOPreplace_5,
    structOPget_6,
    structOPreplace_6,
    structOPget_7,
    structOPreplace_7,
    structOPget_8,
    structOPreplace_8,
    structOPget_9,
    structOPreplace_9,
    structOPget_10,
    structOPreplace_10,
    structOPget_11,
    structOPreplace_11,
    structOPget_12,
    structOPreplace_12,
    structOPget_13,
    structOPreplace_13,
    structOPget_14,
    structOPreplace_14,
    structOPget_15,
    structOPreplace_15,
    structOPget_16,
    structOPreplace_16,
    structOPget_17,
    structOPreplace_17,
    structOPget_18,
    structOPreplace_18,
    structOPget_19,
    structOPreplace_19,
    structOPget_20,
    structOPreplace_20,
    structOPget_21,
    structOPreplace_21,
    structOPget_22,
    structOPreplace_22,
    structOPget_23,
    structOPreplace_23,
    structOPget_24,
    structOPreplace_24,
    structOPget_25,
    structOPreplace_25,
    };


extern errcode oneofOPmake_1();
extern errcode oneofOPvalue_1();
extern errcode oneofOPis_1();
extern errcode oneofOPmake_2();
extern errcode oneofOPvalue_2();
extern errcode oneofOPis_2();
extern errcode oneofOPmake_3();
extern errcode oneofOPvalue_3();
extern errcode oneofOPis_3();
extern errcode oneofOPmake_4();
extern errcode oneofOPvalue_4();
extern errcode oneofOPis_4();
extern errcode oneofOPmake_5();
extern errcode oneofOPvalue_5();
extern errcode oneofOPis_5();
extern errcode oneofOPmake_6();
extern errcode oneofOPvalue_6();
extern errcode oneofOPis_6();
extern errcode oneofOPmake_7();
extern errcode oneofOPvalue_7();
extern errcode oneofOPis_7();
extern errcode oneofOPmake_8();
extern errcode oneofOPvalue_8();
extern errcode oneofOPis_8();
extern errcode oneofOPmake_9();
extern errcode oneofOPvalue_9();
extern errcode oneofOPis_9();
extern errcode oneofOPmake_10();
extern errcode oneofOPvalue_10();
extern errcode oneofOPis_10();
extern errcode oneofOPmake_11();
extern errcode oneofOPvalue_11();
extern errcode oneofOPis_11();
extern errcode oneofOPmake_12();
extern errcode oneofOPvalue_12();
extern errcode oneofOPis_12();
extern errcode oneofOPmake_13();
extern errcode oneofOPvalue_13();
extern errcode oneofOPis_13();
extern errcode oneofOPmake_14();
extern errcode oneofOPvalue_14();
extern errcode oneofOPis_14();
extern errcode oneofOPmake_15();
extern errcode oneofOPvalue_15();
extern errcode oneofOPis_15();
extern errcode oneofOPmake_16();
extern errcode oneofOPvalue_16();
extern errcode oneofOPis_16();
extern errcode oneofOPmake_17();
extern errcode oneofOPvalue_17();
extern errcode oneofOPis_17();
extern errcode oneofOPmake_18();
extern errcode oneofOPvalue_18();
extern errcode oneofOPis_18();
extern errcode oneofOPmake_19();
extern errcode oneofOPvalue_19();
extern errcode oneofOPis_19();
extern errcode oneofOPmake_20();
extern errcode oneofOPvalue_20();
extern errcode oneofOPis_20();
extern errcode oneofOPmake_21();
extern errcode oneofOPvalue_21();
extern errcode oneofOPis_21();
extern errcode oneofOPmake_22();
extern errcode oneofOPvalue_22();
extern errcode oneofOPis_22();
extern errcode oneofOPmake_23();
extern errcode oneofOPvalue_23();
extern errcode oneofOPis_23();
extern errcode oneofOPmake_24();
extern errcode oneofOPvalue_24();
extern errcode oneofOPis_24();
extern errcode oneofOPmake_25();
extern errcode oneofOPvalue_25();
extern errcode oneofOPis_25();
extern errcode oneofOPmake_26();
extern errcode oneofOPvalue_26();
extern errcode oneofOPis_26();
extern errcode oneofOPmake_27();
extern errcode oneofOPvalue_27();
extern errcode oneofOPis_27();
extern errcode oneofOPmake_28();
extern errcode oneofOPvalue_28();
extern errcode oneofOPis_28();
extern errcode oneofOPmake_29();
extern errcode oneofOPvalue_29();
extern errcode oneofOPis_29();
extern errcode oneofOPmake_30();
extern errcode oneofOPvalue_30();
extern errcode oneofOPis_30();

PROC *oneof_prefix_fcn_table[90] = {
    oneofOPmake_1,
    oneofOPvalue_1,
    oneofOPis_1,
    oneofOPmake_2,
    oneofOPvalue_2,
    oneofOPis_2,
    oneofOPmake_3,
    oneofOPvalue_3,
    oneofOPis_3,
    oneofOPmake_4,
    oneofOPvalue_4,
    oneofOPis_4,
    oneofOPmake_5,
    oneofOPvalue_5,
    oneofOPis_5,
    oneofOPmake_6,
    oneofOPvalue_6,
    oneofOPis_6,
    oneofOPmake_7,
    oneofOPvalue_7,
    oneofOPis_7,
    oneofOPmake_8,
    oneofOPvalue_8,
    oneofOPis_8,
    oneofOPmake_9,
    oneofOPvalue_9,
    oneofOPis_9,
    oneofOPmake_10,
    oneofOPvalue_10,
    oneofOPis_10,
    oneofOPmake_11,
    oneofOPvalue_11,
    oneofOPis_11,
    oneofOPmake_12,
    oneofOPvalue_12,
    oneofOPis_12,
    oneofOPmake_13,
    oneofOPvalue_13,
    oneofOPis_13,
    oneofOPmake_14,
    oneofOPvalue_14,
    oneofOPis_14,
    oneofOPmake_15,
    oneofOPvalue_15,
    oneofOPis_15,
    oneofOPmake_16,
    oneofOPvalue_16,
    oneofOPis_16,
    oneofOPmake_17,
    oneofOPvalue_17,
    oneofOPis_17,
    oneofOPmake_18,
    oneofOPvalue_18,
    oneofOPis_18,
    oneofOPmake_19,
    oneofOPvalue_19,
    oneofOPis_19,
    oneofOPmake_20,
    oneofOPvalue_20,
    oneofOPis_20,
    oneofOPmake_21,
    oneofOPvalue_21,
    oneofOPis_21,
    oneofOPmake_22,
    oneofOPvalue_22,
    oneofOPis_22,
    oneofOPmake_23,
    oneofOPvalue_23,
    oneofOPis_23,
    oneofOPmake_24,
    oneofOPvalue_24,
    oneofOPis_24,
    oneofOPmake_25,
    oneofOPvalue_25,
    oneofOPis_25,
    oneofOPmake_26,
    oneofOPvalue_26,
    oneofOPis_26,
    oneofOPmake_27,
    oneofOPvalue_27,
    oneofOPis_27,
    oneofOPmake_28,
    oneofOPvalue_28,
    oneofOPis_28,
    oneofOPmake_29,
    oneofOPvalue_29,
    oneofOPis_29,
    oneofOPmake_30,
    oneofOPmake_30,
    oneofOPvalue_30,
    };


extern errcode variantOPmake_1();
extern errcode variantOPvalue_1();
extern errcode variantOPis_1();
extern errcode variantOPchange_1();
extern errcode variantOPmake_2();
extern errcode variantOPvalue_2();
extern errcode variantOPis_2();
extern errcode variantOPchange_2();
extern errcode variantOPmake_3();
extern errcode variantOPvalue_3();
extern errcode variantOPis_3();
extern errcode variantOPchange_3();
extern errcode variantOPmake_4();
extern errcode variantOPvalue_4();
extern errcode variantOPis_4();
extern errcode variantOPchange_4();
extern errcode variantOPmake_5();
extern errcode variantOPvalue_5();
extern errcode variantOPis_5();
extern errcode variantOPchange_5();
extern errcode variantOPmake_6();
extern errcode variantOPvalue_6();
extern errcode variantOPis_6();
extern errcode variantOPchange_6();
extern errcode variantOPmake_7();
extern errcode variantOPvalue_7();
extern errcode variantOPis_7();
extern errcode variantOPchange_7();
extern errcode variantOPmake_8();
extern errcode variantOPvalue_8();
extern errcode variantOPis_8();
extern errcode variantOPchange_8();
extern errcode variantOPmake_9();
extern errcode variantOPvalue_9();
extern errcode variantOPis_9();
extern errcode variantOPchange_9();
extern errcode variantOPmake_10();
extern errcode variantOPvalue_10();
extern errcode variantOPis_10();
extern errcode variantOPchange_10();
extern errcode variantOPmake_11();
extern errcode variantOPvalue_11();
extern errcode variantOPis_11();
extern errcode variantOPchange_11();
extern errcode variantOPmake_12();
extern errcode variantOPvalue_12();
extern errcode variantOPis_12();
extern errcode variantOPchange_12();
extern errcode variantOPmake_13();
extern errcode variantOPvalue_13();
extern errcode variantOPis_13();
extern errcode variantOPchange_13();
extern errcode variantOPmake_14();
extern errcode variantOPvalue_14();
extern errcode variantOPis_14();
extern errcode variantOPchange_14();
extern errcode variantOPmake_15();
extern errcode variantOPvalue_15();
extern errcode variantOPis_15();
extern errcode variantOPchange_15();
extern errcode variantOPmake_16();
extern errcode variantOPvalue_16();
extern errcode variantOPis_16();
extern errcode variantOPchange_16();
extern errcode variantOPmake_17();
extern errcode variantOPvalue_17();
extern errcode variantOPis_17();
extern errcode variantOPchange_17();
extern errcode variantOPmake_18();
extern errcode variantOPvalue_18();
extern errcode variantOPis_18();
extern errcode variantOPchange_18();
extern errcode variantOPmake_19();
extern errcode variantOPvalue_19();
extern errcode variantOPis_19();
extern errcode variantOPchange_19();
extern errcode variantOPmake_20();
extern errcode variantOPvalue_20();
extern errcode variantOPis_20();
extern errcode variantOPchange_20();
extern errcode variantOPmake_21();
extern errcode variantOPvalue_21();
extern errcode variantOPis_21();
extern errcode variantOPchange_21();
extern errcode variantOPmake_22();
extern errcode variantOPvalue_22();
extern errcode variantOPis_22();
extern errcode variantOPchange_22();
extern errcode variantOPmake_23();
extern errcode variantOPvalue_23();
extern errcode variantOPis_23();
extern errcode variantOPchange_23();
extern errcode variantOPmake_24();
extern errcode variantOPvalue_24();
extern errcode variantOPis_24();
extern errcode variantOPchange_24();
extern errcode variantOPmake_25();
extern errcode variantOPvalue_25();
extern errcode variantOPis_25();
extern errcode variantOPchange_25();
extern errcode variantOPmake_26();
extern errcode variantOPvalue_26();
extern errcode variantOPis_26();
extern errcode variantOPchange_26();
extern errcode variantOPmake_27();
extern errcode variantOPvalue_27();
extern errcode variantOPis_27();
extern errcode variantOPchange_27();
extern errcode variantOPmake_28();
extern errcode variantOPvalue_28();
extern errcode variantOPis_28();
extern errcode variantOPchange_28();
extern errcode variantOPmake_29();
extern errcode variantOPvalue_29();
extern errcode variantOPis_29();
extern errcode variantOPchange_29();
extern errcode variantOPmake_30();
extern errcode variantOPvalue_30();
extern errcode variantOPis_30();
extern errcode variantOPchange_30();

PROC *variant_prefix_fcn_table[120] = {
    variantOPmake_1,
    variantOPvalue_1,
    variantOPis_1,
    variantOPchange_1,
    variantOPmake_2,
    variantOPvalue_2,
    variantOPis_2,
    variantOPchange_2,
    variantOPmake_3,
    variantOPvalue_3,
    variantOPis_3,
    variantOPchange_3,
    variantOPmake_4,
    variantOPvalue_4,
    variantOPis_4,
    variantOPchange_4,
    variantOPmake_5,
    variantOPvalue_5,
    variantOPis_5,
    variantOPchange_5,
    variantOPmake_6,
    variantOPvalue_6,
    variantOPis_6,
    variantOPchange_6,
    variantOPmake_7,
    variantOPvalue_7,
    variantOPis_7,
    variantOPchange_7,
    variantOPmake_8,
    variantOPvalue_8,
    variantOPis_8,
    variantOPchange_8,
    variantOPmake_9,
    variantOPvalue_9,
    variantOPis_9,
    variantOPchange_9,
    variantOPmake_10,
    variantOPvalue_10,
    variantOPis_10,
    variantOPchange_10,
    variantOPmake_11,
    variantOPvalue_11,
    variantOPis_11,
    variantOPchange_11,
    variantOPmake_12,
    variantOPvalue_12,
    variantOPis_12,
    variantOPchange_12,
    variantOPmake_13,
    variantOPvalue_13,
    variantOPis_13,
    variantOPchange_13,
    variantOPmake_14,
    variantOPvalue_14,
    variantOPis_14,
    variantOPchange_14,
    variantOPmake_15,
    variantOPvalue_15,
    variantOPis_15,
    variantOPchange_15,
    variantOPmake_16,
    variantOPvalue_16,
    variantOPis_16,
    variantOPchange_16,
    variantOPmake_17,
    variantOPvalue_17,
    variantOPis_17,
    variantOPchange_17,
    variantOPmake_18,
    variantOPvalue_18,
    variantOPis_18,
    variantOPchange_18,
    variantOPmake_19,
    variantOPvalue_19,
    variantOPis_19,
    variantOPchange_19,
    variantOPmake_20,
    variantOPvalue_20,
    variantOPis_20,
    variantOPchange_20,
    variantOPmake_21,
    variantOPvalue_21,
    variantOPis_21,
    variantOPchange_21,
    variantOPmake_22,
    variantOPvalue_22,
    variantOPis_22,
    variantOPchange_22,
    variantOPmake_23,
    variantOPvalue_23,
    variantOPis_23,
    variantOPchange_23,
    variantOPmake_24,
    variantOPvalue_24,
    variantOPis_24,
    variantOPchange_24,
    variantOPmake_25,
    variantOPvalue_25,
    variantOPis_25,
    variantOPchange_25,
    variantOPmake_26,
    variantOPvalue_26,
    variantOPis_26,
    variantOPchange_26,
    variantOPmake_27,
    variantOPvalue_27,
    variantOPis_27,
    variantOPchange_27,
    variantOPmake_28,
    variantOPvalue_28,
    variantOPis_28,
    variantOPchange_28,
    variantOPmake_29,
    variantOPvalue_29,
    variantOPis_29,
    variantOPchange_29,
    variantOPmake_30,
    variantOPvalue_30,
    variantOPis_30,
    variantOPchange_30,
    };



char *oneof_prefix_name_table[3] = {"make_", "value_", "is_"};
char *variant_prefix_name_table[4] = {"make_", "value_", "is_", "change_"};
char *record_prefix_name_table[2] = {"get_", "set_"};
char *struct_prefix_name_table[2] = {"get_", "replace_"};

extern errcode oneofOPequal();
extern errcode oneofOPcopy();
extern errcode oneofOPsimilar();
extern errcode oneofOPprint();
extern errcode oneofOPencode();
extern errcode oneofOPdecode();
extern errcode oneofOP_gcd();
extern errcode oneofOPdebug_print();

extern errcode variantOPequal();
extern errcode variantOPcopy();
extern errcode variantOPsimilar();
extern errcode variantOPsimilar1();
extern errcode variantOPprint();
extern errcode variantOPencode();
extern errcode variantOPdecode();
extern errcode variantOP_gcd();
extern errcode variantOPdebug_print();

extern errcode recordOPequal();
extern errcode recordOPcopy();
extern errcode recordOPsimilar();
extern errcode recordOPsimilar1();
extern errcode recordOPprint();
extern errcode recordOPencode();
extern errcode recordOPdecode();
extern errcode recordOP_gcd();
extern errcode recordOPdebug_print();

extern errcode structOPequal();
extern errcode structOPcopy();
extern errcode structOPsimilar();
extern errcode structOPprint();
extern errcode structOPencode();
extern errcode structOPdecode();
extern errcode structOP_gcd();
extern errcode structOPdebug_print();

PROC *oneof_param_fcn_table[8] = 
	{oneofOPequal, oneofOPsimilar, oneofOPcopy, oneofOPprint, 
		oneofOPencode, oneofOPdecode, oneofOP_gcd, oneofOPdebug_print};
PROC *variant_param_fcn_table[8] = 
	{variantOPsimilar, variantOPsimilar1, variantOPcopy, variantOPprint,
		variantOPencode, variantOPdecode, variantOP_gcd, variantOPdebug_print};
PROC *record_param_fcn_table[8] = 
	{recordOPsimilar, recordOPsimilar1, recordOPcopy, recordOPprint,
		recordOPencode, recordOPdecode, recordOP_gcd, recordOPdebug_print};
PROC *struct_param_fcn_table[8] = 
	{structOPequal, structOPsimilar, structOPcopy, structOPprint,
		structOPencode, structOPdecode, structOP_gcd, structOPdebug_print};

extern errcode oneofOPo2v();
extern errcode oneofOPv2o();
extern errcode variantOPv_gets_v();
extern errcode variantOPv_gets_o();
extern errcode variantOPequal();
extern errcode variantOPcopy1();
extern errcode recordOPr_gets_r();
extern errcode recordOPr_gets_s();
extern errcode recordOPequal();
extern errcode recordOPcopy1();
extern errcode structOPs2r();
extern errcode structOPr2s();

PROC *oneof_plain_fcn_table[2] = 
	{oneofOPo2v, oneofOPv2o};
PROC *variant_plain_fcn_table[4] = 
	{variantOPv_gets_v, variantOPv_gets_o, variantOPequal, variantOPcopy1};
PROC *record_plain_fcn_table[4] = 
	{recordOPr_gets_r, recordOPr_gets_s, recordOPequal, recordOPcopy1};
PROC *struct_plain_fcn_table[2] = 
	{structOPs2r, structOPr2s};

typedef struct {
int count;
struct OP_ENTRY entry[8];
} selector_OPS;

typedef struct {
int count;
struct REQ_ENTRY entry[8];
} selector_REQS;

typedef struct {
int count;
struct OP_ENTRY equal;
struct OP_ENTRY copy;
struct OP_ENTRY similar;
struct OP_ENTRY print;
struct OP_ENTRY encode;
struct OP_ENTRY decode;
struct OP_ENTRY _gcd;
struct OP_ENTRY debug_print;
} selector_TABLE;

selector_REQS selector_reqs_actual = {8, "equal", "similar", "copy", "print", "encode", "decode",
"_gcd", "debug_print"};

selector_REQS *selector_reqs = &selector_reqs_actual;

char *oneof_param_name_table[8] = {"equal", "similar", "copy", "print", "encode", "decode",
"_gcd", "debug_print"};
char *variant_param_name_table[8] = {"similar", "similar1", "copy", "print", "encode", "decode",
"_gcd", "debug_print"};
char *record_param_name_table[8] = {"similar", "similar1", "copy", "print", "encode", "decode",
"_gcd", "debug_print"};
char *struct_param_name_table[8] = {"equal", "similar", "copy", "print", "encode", "decode",
"_gcd", "debug_print"};

char *oneof_reqs_name_table[8] = {"equal", "similar", "copy", "print", "encode", "decode",
"_gcd", "debug_print"};
char *variant_reqs_name_table[8] = {"similar", "equal", "copy", "print", "encode", "decode",
"_gcd", "debug_print"};
char *record_reqs_name_table[8] = {"similar", "equal", "copy", "print", "encode", "decode",
"_gcd", "debug_print"};
char *struct_reqs_name_table[8] = {"equal", "similar", "copy", "print", "encode", "decode",
"_gcd", "debug_print"};

char *oneof_plain_name_table[2] = {"o2v", "v2o"};
char *variant_plain_name_table[4] = {"v_gets_v", "v_gets_o", "equal", "copy1"};
char *record_plain_name_table[4] = {"r_gets_r", "r_gets_s", "equal", "copy1"};
char *struct_plain_name_table[2] = {"s2r", "r2s"};
