
/*
 * Common TeX:cmd.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#define ESCAPE			0
#define RELAX			0
#define LEFT_BRACE		1
#define RIGHT_BRACE		2
#define MATH_SHIFT		3
#define TAB_MARK		4
#define CAR_RET			5
#define OUT_PARAM		5
#define MAC_PARAM		6
#define SUP_MARK		7
#define SUB_MARK		8
#define IGNORE			9
#define ENDV			9
#define SPACER			10
#define LETTER			11
#define OTHER_CHAR		12
#define ACTIVE_CHAR		13
#define PAR_END			13
#define MATCH			13
#define COMMENT			14
#define END_MATCH		14
#define STOP			14
#define INVALID_CHAR		15
#define DELIM_NUM		15

#define MAX_CHAR_CODE		15

#define CHAR_NUM		16
#define MATH_CHAR_NUM		17
#define MARK			18
#define XRAY			19
#define MAKE_BOX		20
#define HMOVE			21
#define VMOVE			22
#define UN_HBOX			23
#define UN_VBOX			24
#define REMOVE_ITEM		25
#define HSKIP			26
#define VSKIP			27
#define MSKIP			28
#define KERN			29
#define MKERN			30
#define LEADER_SHIP		31
#define HALIGN			32
#define VALIGN			33
#define NO_ALIGN		34
#define VRULE			35
#define HRULE			36
#define INSERT			37
#define VADJUST			38
#define IGNORE_SPACES		39
#define AFTER_ASSIGNMENT	40
#define AFTER_GROUP		41
#define BREAK_PENALTY		42
#define START_PAR		43
#define ITAL_CORR		44
#define ACCENT			45
#define MATH_ACCENT		46
#define DISCRETIONARY		47
#define EQ_NO			48
#define LEFT_RIGHT		49
#define MATH_COMP		50
#define LIMIT_SWITCH		51
#define ABOVE			52
#define MATH_STYLE		53
#define MATH_CHOICE		54
#define NON_SCRIPT		55
#define VCENTER			56
#define CASE_SHIFT		57
#define MESSAGE			58
#define EXTENSION		59
#define IN_STREAM		60
#define BEGIN_GROUP		61
#define END_GROUP		62
#define OMIT			63
#define EX_SPACE		64
#define NO_BOUNDARY		65
#define RADICAL			66
#define END_CS_NAME		67
#define MIN_INTERNAL		68
#define CHAR_GIVEN		68
#define MATH_GIVEN		69
#define LAST_ITEM		70

#define MAX_NON_PREFIXED_COMMAND	70

#define TOKS_REGISTER		71
#define ASSIGN_TOKS		72
#define ASSIGN_INT		73
#define ASSIGN_DIMEN		74
#define ASSIGN_GLUE		75
#define ASSIGN_MU_GLUE		76
#define ASSIGN_FONT_DIMEN	77
#define ASSIGN_FONT_INT		78
#define SET_AUX			79
#define SET_PREV_GRAF		80
#define SET_PAGE_DIMEN		81
#define SET_PAGE_INT		82
#define SET_BOX_DIMEN		83
#define SET_SHAPE		84
#define DEF_CODE		85
#define DEF_FAMILY		86
#define SET_FONT		87
#define DEF_FONT		88
#define REGISTER		89

#define MAX_INTERNAL		89

#define ADVANCE			90
#define MULTIPLY		91
#define DIVIDE			92
#define PREFIX			93
#define LET			94
#define SHORTHAND_DEF		95
#define READ_TO_CS		96
#define DEF			97
#define SET_BOX			98
#define HYPH_DATA		99
#define SET_INTERACTION		100

#define MAX_COMMAND		100

#define UNDEFINED_CS		(MAX_COMMAND + 1)
#define EXPAND_AFTER		(MAX_COMMAND + 2)
#define NO_EXPAND		(MAX_COMMAND + 3)
#define INPUT			(MAX_COMMAND + 4)
#define IF_TEST			(MAX_COMMAND + 5)
#define FI_OR_ELSE		(MAX_COMMAND + 6)
#define CS_NAME			(MAX_COMMAND + 7)
#define CONVERT			(MAX_COMMAND + 8)
#define THE			(MAX_COMMAND + 9)
#define TOP_BOT_MARK		(MAX_COMMAND + 10)
#define CALL			(MAX_COMMAND + 11)
#define LONG_CALL		(MAX_COMMAND + 12)
#define OUTER_CALL		(MAX_COMMAND + 13)
#define LONG_OUTER_CALL		(MAX_COMMAND + 14)
#define END_TEMPLATE		(MAX_COMMAND + 15)
#define DONT_EXPAND		(MAX_COMMAND + 16)
#define BOX_REG			(MAX_COMMAND + 17)
#define MU_SKIP_REG		(MAX_COMMAND + 18)
#define SKIP_REG		(MAX_COMMAND + 19)
#define DIMEN_REG		(MAX_COMMAND + 20)
#define INT_REG			(MAX_COMMAND + 21)
#define TOKS_REG		(MAX_COMMAND + 22)
#define FNT_REG			(MAX_COMMAND + 23)
#define CAT_CODE_REG		(MAX_COMMAND + 24)
#define LC_CODE_REG		(MAX_COMMAND + 25)
#define UC_CODE_REG		(MAX_COMMAND + 26)
#define SF_CODE_REG		(MAX_COMMAND + 27)
#define MATH_CODE_REG		(MAX_COMMAND + 28)
#define DEL_CODE_REG		(MAX_COMMAND + 29)

void	show_cur_cmd_chr();
void	print_cmd_chr(int cmd, int chr);
void	print_mu_skip_param(int);
void	print_skip_param(int);
void	print_param(int);
void	print_length_param(int);
void	print_toks_param(int);

void	_cmd_init();
void	_cmd_init_once();
