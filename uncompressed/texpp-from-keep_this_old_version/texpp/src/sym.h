
/*
 * $Id: sym.h,v 1.5 2002/04/19 11:21:10 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

struct reg_t {
	short	type_field;
	short	level_field;
	int	equiv_field;
};
typedef struct reg_t reg_t, *reg;

#define reg_type(R)		((reg) (R))->type_field
#define reg_level(R)		((reg) (R))->level_field
//#define reg_equiv(R)		((reg) (R))->equiv_field
#define reg_equiv(R)		((R))->equiv_field

struct sym_t {
	reg_t	reg_field;
	int	length_field;
	str	text_field;
	struct sym_t *hash_field;
};
typedef struct sym_t sym_t, *sym;

#define sym2reg(S)		((reg) & ((sym) (S))->reg_field)
#define reg2sym(R)		((sym) R)
#define eq_type(S)		reg_type(sym2reg(S))
#define eq_level(S)		reg_level(sym2reg(S))
#define equiv(S)		   reg_equiv(sym2reg(S))
#define text(S)			((sym) (S))->text_field
//#define hash(S)			((sym) (S))->hash_field

int& length(sym);
struct sym_t*& hash(sym);

#define null_sym ((sym) 0)

extern	sym	cur_cs;
extern	sym	warning_cs;
extern	sym	par_cs;
extern	sym	par_shape_cs;
extern	sym	undefined_cs;
extern	sym	null_cs;

extern	sym	*active_base;
extern	sym	*single_base;
extern	sym	*hash_base;
extern	sym	*frozen_cs_base;

extern	sym	*font_id_base;

#define font_id(F)		font_id_base[F]
#define font_id_text(F)		text(font_id_base[F])
#define font_id_length(F)	font_id_base[F]->length_field

extern	reg	*skip_reg;
extern	reg	*mu_skip_reg;
extern	reg	*box_reg;
extern	reg	*int_reg;
extern	reg	*dimen_reg;
extern	reg	*toks_reg;
extern	reg	*fnt_reg;
extern	reg	*cat_code_reg;
extern	reg	*lc_code_reg;
extern	reg	*uc_code_reg;
extern	reg	*sf_code_reg;
extern	reg	*math_code_reg;
extern	reg	*del_code_reg;

#define MU_GLUE_PARS			3
//#define mu_glue_par(G)			((ptr) reg_equiv(mu_skip_reg[G]))
#define mu_glue_par(G)			(reg_equiv(mu_skip_reg[G]))

#define THIN_MU_SKIP_CODE		0
#define MED_MU_SKIP_CODE		1
#define THICK_MU_SKIP_CODE		2

#define thin_mu_skip			mu_glue_par(THIN_MU_SKIP_CODE)
#define med_mu_skip			mu_glue_par(MED_MU_SKIP_CODE)
#define thick_mu_skip			mu_glue_par(THICK_MU_USKIP_CODE)

#define GLUE_PARS			15
//#define glue_par(G)			((ptr) reg_equiv(skip_reg[G]))
#define glue_par(G)			(reg_equiv(skip_reg[G]))

#define LINE_SKIP_CODE			0
#define BASELINE_SKIP_CODE		1
#define PAR_SKIP_CODE			2
#define ABOVE_DISPLAY_SKIP_CODE		3
#define BELOW_DISPLAY_SKIP_CODE		4
#define ABOVE_DISPLAY_SHORT_SKIP_CODE	5
#define BELOW_DISPLAY_SHORT_SKIP_CODE	6
#define LEFT_SKIP_CODE			7
#define RIGHT_SKIP_CODE			8
#define TOP_SKIP_CODE			9
#define SPLIT_TOP_SKIP_CODE		10
#define TAB_SKIP_CODE			11
#define SPACE_SKIP_CODE			12
#define XSPACE_SKIP_CODE		13
#define PAR_FILL_SKIP_CODE		14

#define line_skip			glue_par(LINE_SKIP_CODE)
#define baseline_skip			glue_par(BASELINE_SKIP_CODE)
#define par_skip			glue_par(PAR_SKIP_CODE)
#define above_display_skip		glue_par(ABOVE_DISPLAY_SKIP_CODE)
#define below_display_skip		glue_par(BELOW_DISPLAY_SKIP_CODE)
#define above_display_short_skip	glue_par(ABOVE_DISPLAY_SHORT_SKIP_CODE)
#define below_display_short_skip	glue_par(BELOW_DISPLAY_SHORT_SKIP_CODE)
#define left_skip			glue_par(LEFT_SKIP_CODE)
#define right_skip			glue_par(RIGHT_SKIP_CODE)
#define top_skip			glue_par(TOP_SKIP_CODE)
#define split_top_skip			glue_par(SPLIT_TOP_SKIP_CODE)
#define tab_skip			glue_par(TAB_SKIP_CODE)
#define space_skip			glue_par(SPACE_SKIP_CODE)
#define xspace_skip			glue_par(XSPACE_SKIP_CODE)
#define par_fill_skip			glue_par(PAR_FILL_SKIP_CODE)

#define DIMEN_PARS			21
//#define dimen_par(D)			((scal) reg_equiv(dimen_reg[D]))
#define dimen_par(D)			(reg_equiv(dimen_reg[D]))

#define PAR_INDENT_CODE			0
#define MATH_SURROUND_CODE		1
#define LINE_SKIP_LIMIT_CODE		2
#define HSIZE_CODE			3
#define VSIZE_CODE			4
#define MAX_DEPTH_CODE			5
#define SPLIT_MAX_DEPTH_CODE		6
#define BOX_MAX_DEPTH_CODE		7
#define HFUZZ_CODE			8
#define VFUZZ_CODE			9
#define DELIMITER_SHORTFALL_CODE	10
#define NULL_DELIMITER_SPACE_CODE	11
#define SCRIPT_SPACE_CODE		12
#define PRE_DISPLAY_SIZE_CODE		13
#define DISPLAY_WIDTH_CODE		14
#define DISPLAY_INDENT_CODE		15
#define OVERFULL_RULE_CODE		16
#define HANG_INDENT_CODE		17
#define H_OFFSET_CODE			18
#define V_OFFSET_CODE			19
#define EMERGENCY_STRETCH_CODE		20

#define par_indent			dimen_par(PAR_INDENT_CODE)
#define math_surround			dimen_par(MATH_SURROUND_CODE)
#define line_skip_limit			dimen_par(LINE_SKIP_LIMIT_CODE)
#define hsize				dimen_par(HSIZE_CODE)
#define vsize				dimen_par(VSIZE_CODE)
#define max_depth			dimen_par(MAX_DEPTH_CODE)
#define split_max_depth			dimen_par(SPLIT_MAX_DEPTH_CODE)
#define box_max_depth			dimen_par(BOX_MAX_DEPTH_CODE)
#define hfuzz				dimen_par(HFUZZ_CODE)
#define vfuzz				dimen_par(VFUZZ_CODE)
#define delimiter_shortfall		dimen_par(DELIMITER_SHORTFALL_CODE)
#define null_delimiter_space		dimen_par(NULL_DELIMITER_SPACE_CODE)
#define script_space			dimen_par(SCRIPT_SPACE_CODE)
#define pre_display_size		dimen_par(PRE_DISPLAY_SIZE_CODE)
#define display_width			dimen_par(DISPLAY_WIDTH_CODE)
#define display_indent			dimen_par(DISPLAY_INDENT_CODE)
#define overfull_rule			dimen_par(OVERFULL_RULE_CODE)
#define hang_indent			dimen_par(HANG_INDENT_CODE)
#define h_offset			dimen_par(H_OFFSET_CODE)
#define v_offset			dimen_par(V_OFFSET_CODE)
#define emergency_stretch		dimen_par(EMERGENCY_STRETCH_CODE)

#define INT_PARS			55
//#define int_par(I)			((int) reg_equiv(int_reg[I]))
#define int_par(I)			(reg_equiv(int_reg[I]))

#define PRETOLERANCE_CODE		0
#define TOLERANCE_CODE			1
#define LINE_PENALTY_CODE		2
#define HYPHEN_PENALTY_CODE		3		
#define CLUB_PENALTY_CODE		4
#define EX_HYPHEN_PENALTY_CODE		5
#define WIDOW_PENALTY_CODE		6
#define DISPLAY_WIDOW_PENALTY_CODE	7
#define BROKEN_PENALTY_CODE		8
#define BIN_OP_PENALTY_CODE		9
#define REL_PENALTY_CODE		10
#define PRE_DISPLAY_PENALTY_CODE	11
#define POST_DISPLAY_PENALTY_CODE	12
#define INTER_LINE_PENALTY_CODE		13
#define DOUBLE_HYPHEN_DEMERITS_CODE	14
#define FINAL_HYPHEN_DEMERITS_CODE	15
#define ADJ_DEMERITS_CODE		16
#define MAG_CODE			17
#define DELIMITER_FACTOR_CODE		18
#define LOOSENESS_CODE			19
#define TIME_CODE			20
#define DAY_CODE			21
#define MONTH_CODE			22
#define YEAR_CODE			23
#define SHOW_BOX_BREADTH_CODE		24
#define SHOW_BOX_DEPTH_CODE		25
#define HBADNESS_CODE			26
#define VBADNESS_CODE			27
#define PAUSING_CODE			28
#define TRACING_ONLINE_CODE		29
#define TRACING_MACROS_CODE		30
#define TRACING_STATS_CODE		31
#define TRACING_PARAGRAPHS_CODE		32
#define TRACING_PAGES_CODE		33
#define TRACING_OUTPUT_CODE		34
#define TRACING_LOST_CHARS_CODE		35
#define TRACING_COMMANDS_CODE		36
#define TRACING_RESTORES_CODE		37
#define UC_HYPH_CODE			38
#define OUTPUT_PENALTY_CODE		39
#define MAX_DEAD_CYCLES_CODE		40
#define HANG_AFTER_CODE			41
#define FLOATING_PENALTY_CODE		42
#define GLOBAL_DEFS_CODE		43
#define CUR_FAM_CODE			44
#define ESCAPE_CHAR_CODE		45
#define DEFAULT_HYPHEN_CHAR_CODE	46
#define DEFAULT_SKEW_CHAR_CODE		47
#define END_LINE_CHAR_CODE		48
#define NEW_LINE_CHAR_CODE		49
#define LANGUAGE_CODE			50
#define LEFT_HYPHEN_MIN_CODE		51
#define RIGHT_HYPHEN_MIN_CODE		52
#define HOLDING_INSERTS_CODE		53
#define ERROR_CONTEXT_LINES_CODE	54

#define pretolerance			int_par(PRETOLERANCE_CODE)
#define tolerance			int_par(TOLERANCE_CODE)
#define line_penalty			int_par(LINE_PENALTY_CODE)
#define hyphen_penalty			int_par(HYPHEN_PENALTY_CODE)
#define ex_hyphen_penalty		int_par(EX_HYPHEN_PENALTY_CODE)
#define club_penalty			int_par(CLUB_PENALTY_CODE)
#define widow_penalty			int_par(WIDOW_PENALTY_CODE)
#define display_widow_penalty		int_par(DISPLAY_WIDOW_PENALTY_CODE)
#define broken_penalty			int_par(BROKEN_PENALTY_CODE)
#define bin_op_penalty			int_par(BIN_OP_PENALTY_CODE)
#define rel_penalty			int_par(REL_PENALTY_CODE)
#define pre_display_penalty		int_par(PRE_DISPLAY_PENALTY_CODE)
#define post_display_penalty		int_par(POST_DISPLAY_PENALTY_CODE)
#define inter_line_penalty		int_par(INTER_LINE_PENALTY_CODE)
#define double_hyphen_demerits		int_par(DOUBLE_HYPHEN_DEMERITS_CODE)
#define final_hyphen_demerits		int_par(FINAL_HYPHEN_DEMERITS_CODE)
#define adj_demerits			int_par(ADJ_DEMERITS_CODE)
#define mag				int_par(MAG_CODE)
#define delimiter_factor		int_par(DELIMITER_FACTOR_CODE)
#define looseness			int_par(LOOSENESS_CODE)
#define time				int_par(TIME_CODE)
#define day				int_par(DAY_CODE)
#define month				int_par(MONTH_CODE)
#define year				int_par(YEAR_CODE)
#define show_box_breadth		int_par(SHOW_BOX_BREADTH_CODE)
#define show_box_depth			int_par(SHOW_BOX_DEPTH_CODE)
#define hbadness			int_par(HBADNESS_CODE)
#define vbadness			int_par(VBADNESS_CODE)
#define pausing				int_par(PAUSING_CODE)
#define tracing_online			int_par(TRACING_ONLINE_CODE)
#define tracing_macros			int_par(TRACING_MACROS_CODE)
#define tracing_stats			int_par(TRACING_STATS_CODE)
#define tracing_paragraphs		int_par(TRACING_PARAGRAPHS_CODE)
#define tracing_pages			int_par(TRACING_PAGES_CODE)
#define tracing_output			int_par(TRACING_OUTPUT_CODE)
#define tracing_lost_chars		int_par(TRACING_LOST_CHARS_CODE)
#define tracing_commands		int_par(TRACING_COMMANDS_CODE)
#define tracing_restores		int_par(TRACING_RESTORES_CODE)
#define uc_hyph				int_par(UC_HYPH_CODE)
#define max_dead_cycles			int_par(MAX_DEAD_CYCLES_CODE)
#define output_penalty			int_par(OUTPUT_PENALTY_CODE)
#define hang_after			int_par(HANG_AFTER_CODE)
#define floating_penalty		int_par(FLOATING_PENALTY_CODE)
#define global_defs			int_par(GLOBAL_DEFS_CODE)
#define cur_fam				int_par(CUR_FAM_CODE)
#define escape_char			int_par(ESCAPE_CHAR_CODE)
#define default_hyphen_char		int_par(DEFAULT_HYPHEN_CHAR_CODE)
#define default_skew_char		int_par(DEFAULT_SKEW_CHAR_CODE)
#define end_line_char			int_par(END_LINE_CHAR_CODE)
#define new_line_char			int_par(NEW_LINE_CHAR_CODE)
#define language			int_par(LANGUAGE_CODE)
#define left_hyphen_min			int_par(LEFT_HYPHEN_MIN_CODE)
#define right_hyphen_min		int_par(RIGHT_HYPHEN_MIN_CODE)
#define holding_inserts			int_par(HOLDING_INSERTS_CODE)
#define error_context_lines		int_par(ERROR_CONTEXT_LINES_CODE)

#define TOK_PARS			9
#define tok_par(T)			((ptr) reg_equiv(toks_reg[T]))

#define OUTPUT_ROUTINE_CODE		0
#define EVERY_PAR_CODE			1
#define EVERY_MATH_CODE			2
#define EVERY_DISPLAY_CODE		3
#define EVERY_HBOX_CODE			4
#define EVERY_VBOX_CODE			5
#define EVERY_JOB_CODE			6
#define EVERY_CR_CODE			7
#define ERR_HELP_CODE			8

#define output_routine_reg		toks_reg[OUTPUT_ROUTINE_CODE]
#define output_routine			tok_par(OUTPUT_ROUTINE_CODE)
#define every_par			tok_par(EVERY_PAR_CODE)
#define every_math			tok_par(EVERY_MATH_CODE)
#define every_display			tok_par(EVERY_DISPLAY_CODE)
#define every_hbox			tok_par(EVERY_HBOX_CODE)
#define every_vbox			tok_par(EVERY_VBOX_CODE)
#define every_job			tok_par(EVERY_JOB_CODE)
#define every_cr			tok_par(EVERY_CR_CODE)
#define err_help			tok_par(ERR_HELP_CODE)

#define FROZEN_CONTROLS			11
#define FROZEN_CONTROL_SEQUENCE		frozen_cs_base
#define FROZEN_PROTECTION		FROZEN_CONTROL_SEQUENCE[0]
#define FROZEN_CR			FROZEN_CONTROL_SEQUENCE[1]
#define FROZEN_END_GROUP		FROZEN_CONTROL_SEQUENCE[2]
#define FROZEN_RIGHT			FROZEN_CONTROL_SEQUENCE[3]
#define FROZEN_FI			FROZEN_CONTROL_SEQUENCE[4]
#define FROZEN_END_TEMPLATE		FROZEN_CONTROL_SEQUENCE[5]
#define FROZEN_ENDV			FROZEN_CONTROL_SEQUENCE[6]
#define FROZEN_RELAX			FROZEN_CONTROL_SEQUENCE[7]
#define FROZEN_END_WRITE		FROZEN_CONTROL_SEQUENCE[8]
#define FROZEN_DONT_EXPAND		FROZEN_CONTROL_SEQUENCE[9]
#define FROZEN_NULL_FONT		FROZEN_CONTROL_SEQUENCE[10]

#define cur_font_ptr		fnt_reg[0]
#define cur_font		   reg_equiv(cur_font_ptr)
#define fam_fnt(R)		reg_equiv(fnt_reg[(R)+1])

#define mu_skip(R)		reg_equiv(mu_skip_reg[MU_GLUE_PARS + (R)])
#define skip(R)			reg_equiv(skip_reg[GLUE_PARS + (R)])
#define dimen(R)		   reg_equiv(dimen_reg[DIMEN_PARS + (R)])
//#define count(R)		   reg_equiv(int_reg[INT_PARS + (R)])
int count(int R);

#define toks(R)			reg_equiv(toks_reg[TOK_PARS + (R)])
#define box(R)			   reg_equiv(box_reg[R])

#define CAT_CODE		   0
#define LC_CODE			1
#define UC_CODE			2
#define SF_CODE			3
#define MATH_CODE		   4
#define DEL_CODE	 	   5

#define cat_code(R)		reg_equiv(cat_code_reg[R])
#define lc_code(R)		reg_equiv(lc_code_reg[R])
#define uc_code(R)		reg_equiv(uc_code_reg[R])
#define sf_code(R)		reg_equiv(sf_code_reg[R])
#define math_code(R)		reg_equiv(math_code_reg[R])
#define del_code(R)		reg_equiv(del_code_reg[R])

#define par_shape_ptr	equiv(par_shape_cs)

struct shape_t {
	scal	indent_field;
	scal	width_field;
};
typedef struct shape_t shape_t;

struct par_shape_t {
	mcell 	node_field;
	shape_t	par_shape_field[1];
};
typedef struct par_shape_t par_shape_t;

#define par_shape	((par_shape_t *) par_shape_ptr)->par_shape_field

#define par_shape_indent(I)	par_shape[I - 1].indent_field
#define par_shape_width(I)	par_shape[I - 1].width_field

extern	ptr	hold_head;
extern	ptr	zero_glue;
extern	ptr	fil_glue;
extern	ptr	fill_glue;
extern	ptr	ss_glue;
extern	ptr	fil_neg_glue;

extern	bool	no_new_control_sequence;
extern	int	cs_count;

extern	int	cur_level;
extern	int	cur_group;

extern	int	nsaves;
extern	reg	save_stack;
extern	reg	save_end;
extern	reg	save_ptr;
extern	reg	max_save_stack;

// #define save_type	reg_type
// #define save_level	reg_level
// #define save_index	reg_equiv

#define saved(I)	reg_equiv(save_ptr+I)

#define RESTORE_OLD_VALUE		0
#define INSERT_TOKEN			1
#define LEVEL_BOUNDARY			2

#define LEVEL_ZERO			MIN_QUARTERWORD
#define LEVEL_ONE			(LEVEL_ZERO + 1)
#define BOTTOM_LEVEL			0
#define SIMPLE_GROUP			1
#define HBOX_GROUP			2
#define ADJUSTED_HBOX_GROUP		3
#define VBOX_GROUP			4
#define VTOP_GROUP			5
#define ALIGN_GROUP			6
#define NO_ALIGN_GROUP			7
#define OUTPUT_GROUP			8
#define MATH_GROUP			9
#define DISC_GROUP			10
#define INSERT_GROUP			11
#define VCENTER_GROUP			12
#define MATH_CHOICE_GROUP		13
#define SEMI_SIMPLE_GROUP		14
#define MATH_SHIFT_GROUP		15
#define MATH_LEFT_GROUP			16
#define MAX_GROUP_CODE			16

sym	id_lookup(str, int);
sym	new_sym(str, int);
sym	primitive(str s, int code, int order);
sym	freeze(sym);
void	print_cs(str);
void	sprint_cs(sym);
bool	issingle(sym s);
bool	isactive(sym s);
bool	isfrozen(sym s);
void	new_save_level(int);
bool	realloc_save_stack();
void	reg_destroy(reg);
void	reg_save(reg r, int l);
void	eq_define(sym s, int t, int e);
void	reg_define(reg r, int t, int e);
void	eq_gdefine(sym s, int t, int e);
void	reg_gdefine(reg r, int t, int e);
void	save_for_after(tok t);
void	unsave();
void	off_save();
void	restore_trace(reg r, str s);
void	show_reg(reg);
sym  *sym_alloc(int i);
reg  *reg_alloc(int i);
void  help_offsave_xtra();
void  help_offsave_missing();
void	_sym_init();
void	_sym_init_once();
