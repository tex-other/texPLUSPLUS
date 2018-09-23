
/*
 * $Id: boxlist.h,v 1.3 2002/04/18 09:50:43 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#define FIL_CODE	0
#define FILL_CODE	1
#define SS_CODE		2
#define FIL_NEG_CODE	3
#define SKIP_CODE	4
#define MSKIP_CODE	5

extern	ptr	cur_box;


#define BOX_FLAG	010000000000
#define SHIP_OUT_FLAG	(BOX_FLAG + 512)
#define LEADER_FLAG	(BOX_FLAG + 513)
#define BOX_CODE	0
#define COPY_CODE	1
#define LAST_BOX_CODE	2
#define VSPLIT_CODE	3
#define VTOP_CODE	4

void	handle_right_brace();
void	extra_right_brace();
void	append_glue();
void	append_kern();
void	scan_box(int box_context);
void	begin_box(int box_context);
void	box_end(int box_context);
void	package(int c);
void	normal_paragraph();
void	indent_in_hmode();
int	norm_min(int h);
void	new_graf(bool indented);
void	end_graf();
void	head_for_vmode();
void	append_to_vlist(ptr b);
void	begin_insert_or_adjust();
void	make_mark();
void	append_penalty();
void	delete_last();
void	unpackage();
void	append_italic_correction();
void	append_discretionary();
void	build_discretionary();
void	flush_discretionary(ptr p);
void	make_accent();
void	align_error();
void	no_align_error();
void	omit_error();
void	do_endv();
void	cs_error();

void	_boxlist_init();
void	_boxlist_init_once();

void help_head_for_vmode();
void help_close_group();
void help_output_balance();
void help_output();
void help_group_close();
void help_leaders();
void help_lastbox_m();
void help_lastbox_v();
void help_vsplit();
void help_box();
void help_insert_255();
void help_space_fac();
void help_delete_last_pen();
void help_delete_last_skip();
void help_delete_last_kern();
void help_pandora();
void help_math_disc();
void help_discretionary();
void help_disc();
void help_tab();
void help_align_error();
void help_align_cr();
void help_fix_alignment();
void help_noalign();
void help_omit();
void help_csname();
