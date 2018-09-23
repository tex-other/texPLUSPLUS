
/*
 * $Id: cmd.h,v 1.4 2003/09/14 13:30:19 kp229 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002,2003      Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef cmd_h_included
#define cmd_h_included

class commands {
	public:
		commands();
		
		enum cmd_codes {
			ESCAPE=0, RELAX=0, LEFT_BRACE=1, RIGHT_BRACE=2, MATH_SHIFT=3,
			TAB_MARK=4, CAR_RET=5, OUT_PARAM=5, MAC_PARAM=6, SUP_MARK=7,
			SUB_MARK=8, IGNORE=9, ENDV=9, SPACER=10, LETTER=11,
			OTHER_CHAR=12, ACTIVE_CHAR=13, PAR_END=13, MATCH=13,
			COMMENT=14, END_MATCH=14, STOP=14, INVALID_CHAR=15, 
			DELIM_NUM=15, MAX_CHAR_CODE=15,
			
			CHAR_NUM=16, MATH_CHAR_NUM=17, MARK=18, XRAY=19, MAKE_BOX=20,
			HMOVE=21, VMOVE=22, UN_HBOX=23, UN_VBOX=24, REMOVE_ITEM=25,
			HSKIP=26, VSKIP=27, MSKIP=28, KERN=29, MKERN=30, LEADER_SHIP=31,
			HALIGN=32, VALIGN=33, NO_ALIGN=34, VRULE=35, HRULE=36, INSERT=37,
			VADJUST=38, IGNORE_SPACES=39, AFTER_ASSIGNMENT=40, AFTER_GROUP=41,
			BREAK_PENALTY=42, START_PAR=43, ITAL_CORR=44, ACCENT=45, MATH_ACCENT=46,
			
			DISCRETIONARY=47, EQ_NO=48, LEFT_RIGHT=49, MATH_COMP=50, LIMIT_SWITCH=51,
			ABOVE=52, MATH_STYLE=53, MATH_CHOICE=54, NON_SCRIPT=55, VCENTER=56,
			CASE_SHIFT=57, MESSAGE=58, EXTENSION=59, IN_STREAM=60, BEGIN_GROUP=61,
			END_GROUP=62, OMIT=63, EX_SPACE=64, NO_BOUNDARY=65, RADICAL=66,
			END_CS_NAME=67, MIN_INTERNAL=68, CHAR_GIVEN=68, MATH_GIVEN=69, 
			LAST_ITEM=70, MAX_NON_PREFIXED_COMMAND=70,
			
			TOKS_REGISTER=71, ASSIGN_TOKS=72, ASSIGN_INT=73, ASSIGN_DIMEN=74,
			ASSIGN_GLUE=75, ASSIGN_MU_GLUE=76, ASSIGN_FONT_DIMEN=77,
			ASSIGN_FONT_INT=78, SET_AUX=79, SET_PREV_GRAF=80,
			SET_PAGE_DIMEN=81, SET_PAGE_INT=82, SET_BOX_DIMEN=83, SET_SHAPE=84,
			DEF_CODE=85, DEF_FAMILY=86, SET_FONT=87, DEF_FONT=88, REGISTER=89,
			MAX_INTERNAL=89,
			
			ADVANCE=90, MULTIPLY=91, DIVIDE=92, PREFIX=93, LET=94, SHORTHAND_DEF=95,
			READ_TO_CS=96, DEF=97, SET_BOX=98, HYPH_DATA=99, SET_INTERACTION=100,
			MAX_COMMAND=100,
			
			UNDEFINED_CS=101, EXPAND_AFTER=102, NO_EXPAND=103,
			INPUT=104, IF_TEST=105, FI_OR_ELSE=106, CS_NAME=107,
			CONVERT=108, THE=109, TOP_BOT_MARK=110, CALL=111,
			LONG_CALL=112, OUTER_CALL=113, LONG_OUTER_CALL=114,
			END_TEMPLATE=115, DONT_EXPAND=116, BOX_REG=117,
			MU_SKIP_REG=118, SKIP_REG=119, DIMEN_REG=120,
			INT_REG=121, TOKS_REG=122, FNT_REG=123, CAT_CODE_REG=124,
			LC_CODE_REG=125, UC_CODE_REG=126, SF_CODE_REG=127, 
			MATH_CODE_REG=128, DEL_CODE_REG=129
		};
		
		void	show_cur_cmd_chr();
		void	print_cmd_chr(int cmd, int chr);
		void	print_mu_skip_param(int);
		void	print_skip_param(int);
		void	print_param(int);
		void	print_length_param(int);
		void	print_toks_param(int);
		
		void	_cmd_init();
};

#endif
