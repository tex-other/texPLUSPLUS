
/*
 * $Id: align.c,v 1.5 2002/04/19 11:21:10 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include "tex.h"

align_stack_t *align_stack;

ptr	align_head;

ptr	cur_align;
ptr	cur_span;
ptr	cur_loop;
ptr	cur_head;
ptr	cur_tail;

ptr	end_span;

void push_alignment()
	{
	align_stack_t *p;

	p = (align_stack_t *) new_node(ALIGN_STACK_NODE_SIZE);
	p->align_stack_field = align_stack;
	p->preamble_field = link(align_head);
	p->cur_align_field = cur_align;
	p->cur_span_field = cur_span;
	p->cur_loop_field = cur_loop;
	p->cur_head_field = cur_head;
	p->cur_tail_field = cur_tail;
	p->align_state_field = align_state;
	align_stack = p;
	cur_head = new_avail();
	}

void pop_alignment ()
	{
	align_stack_t *p;

	free_avail(cur_head);
	p = align_stack;
	align_stack = p->align_stack_field;
	link(align_head) = p->preamble_field;
	cur_align = p->cur_align_field;
	cur_span = p->cur_span_field;
	cur_loop = p->cur_loop_field;
	cur_head = p->cur_head_field;
	cur_tail = p->cur_tail_field;
	align_state = p->align_state_field;
	free_node((ptr)p, ALIGN_STACK_NODE_SIZE);
	}

void
init_align ()
{
	ptr	p;
	sym	save_cs_ptr;

#define MISSING_MAC_PARAM "Missing # inserted in alignment preamble"
#define EXTRA_MAC_PARAM "Only one # is allowed per tab"
	
	save_cs_ptr = cur_cs;
	push_alignment();
	align_state = -1000000;
	if (mode == MMODE && (tail != head || incompleat_noad != null)) {
		print_err("Improper ");
		print_esc("halign");
		print(" inside $$'s");
		help_display_align();
		error();
		flush_math();
	}
	push_nest();
	if (mode == MMODE) {
		mode = -VMODE;
		prev_depth = nest_ptr[-2].aux_field;
	} else if (mode > 0) {
		mode = -mode;
	}
	scan_spec(ALIGN_GROUP, FALSE);
	link(align_head) = null;
	cur_align = align_head;
	cur_loop = null;
	scanner_status = ALIGNING;
	warning_cs = save_cs_ptr;
	align_state = -1000000;
	loop {
		link(cur_align) = new_param_glue(TAB_SKIP_CODE);
		cur_align = link(cur_align);
		if (cur_cmd == CAR_RET)
			break;
		p = align_toks;
		token_link(p) = null;
		loop {
			get_preamble_token();
			if (cur_cmd == MAC_PARAM)
				break;
			if (cur_cmd <= CAR_RET &&
				cur_cmd >= TAB_MARK &&
				align_state == -1000000) {
				if (p == align_toks &&
					cur_loop == null &&
					cur_cmd == TAB_MARK) {
					cur_loop = cur_align;
				} else {
					print_err(MISSING_MAC_PARAM);
					help_preamble_missing();
					back_error();
					break;
				}
			} else if (cur_cmd != SPACER || p != align_toks) {
				p = token_link(p) = new_token();
				token(p) = cur_tok;
			}
		}
		link(cur_align) = new_null_box();
		cur_align = link(cur_align);
		end_link(cur_align) = end_span;
		unset_width(cur_align) = NULL_FLAG;
		u_part(cur_align) = token_link(align_toks);
		p = align_toks;
		token_link(p) = null;
		loop {
			get_preamble_token();
			if (cur_cmd <= CAR_RET &&
				cur_cmd >= TAB_MARK &&
				align_state == -1000000)
				break;
			if (cur_cmd == MAC_PARAM) {
				print_err(EXTRA_MAC_PARAM);
				help_preamble_many();
				error();
				continue;
			}
			p = token_link(p) = new_token();
			token(p) = cur_tok;
		}
		p = token_link(p) = new_token();
		token(p) = END_TEMPLATE_TOKEN;
		v_part(cur_align) = token_link(align_toks);
	}
	scanner_status = NORMAL;
	new_save_level(ALIGN_GROUP);
	if (every_cr != null)
		begin_token_list(every_cr, EVERY_CR_TEXT);
	align_peek();
}

void
get_preamble_token ()
{
restart:
	get_token();
	while (cur_chr == SPAN_CODE && cur_cmd == TAB_MARK) {
		get_token();
		if (cur_cmd > MAX_COMMAND) {
			expand();
			get_token();
		}
	}
	if (cur_cmd == ASSIGN_GLUE && cur_chr == TAB_SKIP_CODE) {
		scan_optional_equals();
		scan_glue(GLUE_VAL);
		if (global_defs > 0)
			reg_gdefine(skip_reg[TAB_SKIP_CODE], SKIP_REG, cur_val);
		else reg_define(skip_reg[TAB_SKIP_CODE], SKIP_REG, cur_val);
		goto restart;
	}
}

void
align_peek ()
{
restart:
	align_state = 1000000;
	get_nbx_token();
	if (cur_cmd == NO_ALIGN) {
		scan_left_brace();
		new_save_level(NO_ALIGN_GROUP);
		if (mode == -VMODE)
			normal_paragraph();
	} else if (cur_cmd == RIGHT_BRACE) {
		fin_align();
	} else if (cur_cmd == CAR_RET && cur_chr == CR_CR_CODE) {
		goto restart;
	} else {
		init_row();
		init_col();
	}
}

void
init_row ()
{
	push_nest();
	mode = (-HMODE - VMODE) - mode;
	if (mode == -HMODE)
		space_factor = 0;
	else prev_depth = 0;
	tail_append(new_glue(glue_ptr(link(align_head))));
	subtype(tail) = TAB_SKIP_CODE + 1;
	cur_align = link(link(align_head));
	cur_tail = cur_head;
	init_span(cur_align);
}

void init_span(ptr p)
	{
	push_nest();
	if (mode == -HMODE) {
		space_factor = 1000;
	} else {
		prev_depth = IGNORE_DEPTH;
		normal_paragraph();
	}
	cur_span = p;
}

void
init_col ()
{
	unset_info(cur_align) = cur_cmd;
	if (cur_cmd == OMIT) {
		align_state = 0;
	} else {
		back_input();
		begin_token_list(u_part(cur_align), U_TEMPLATE);
	}
}

bool
fin_col ()
{
	int	n;
	int	o;
	ptr	p;
	ptr	q;
	ptr	r;
	ptr	s;
	ptr	u;
	scal	w;

#define INTERWOVEN_ALIGNS "(interwoven alignment preambles are not allowed)"

	if (cur_align == null)
		confusion("endv");
	q = link(cur_align);
	if (q == null)
		confusion("endv");
	if (align_state < 500000)
		fatal_error(INTERWOVEN_ALIGNS);
	p = link(q);
	if (p == null && unset_info(cur_align) < CR_CODE) {
		if (cur_loop != null) {
			p = link(q) = new_null_box();
			end_link(p) = end_span;
			unset_width(p) = NULL_FLAG;
			cur_loop = link(cur_loop);
			q = align_toks;
			r = u_part(cur_loop);
			while (r != null) {
				q = token_link(q) = new_token();
				token(q) = token(r);
				r = token_link(r);
			}
			token_link(q) = null;
			u_part(p) = token_link(align_toks);
			q = align_toks;
			r = v_part(cur_loop);
			while (r != null) {
				q = token_link(q) = new_token();
				token(q) = token(r);
				r = token_link(r);
			}
			token_link(q) = null;
			v_part(p) = token_link(align_toks);
			cur_loop = link(cur_loop);
			link(p) = new_glue(glue_ptr(cur_loop));
		} else {
			print_err("Extra alignment tab has been changed to ");
			print_esc("cr");
			help_align_apply();
			unset_info(cur_align) = CR_CODE;
			error();
		}
	}
	if (unset_info(cur_align) != SPAN_CODE) {
		unsave();
		new_save_level(ALIGN_GROUP);
		if (mode == -HMODE) {
			tex::adjust_tail = cur_tail;
			u = hpack(link(head), 0, ADDITIONAL);
			w = box_width(u);
			cur_tail = tex::adjust_tail;
			tex::adjust_tail = null;
		} else {
			u = vpackage(link(head), 0, ADDITIONAL, 0);
			w = box_height(u);
		}
		n = MIN_QUARTERWORD;
		if (cur_span != cur_align) {
			q = cur_span;
			do	{
				incr(n);
				q = link(link(q));
			} while (q != cur_align);
			if (n > MAX_QUARTERWORD)
				confusion("256 spans");
			q = cur_span;
			while (span_count(span_link(q)) < n)
				q = span_link(q);
			if (span_count(span_link(q)) > n) {
				s = new_node(SPAN_NODE_SIZE);
				span_link(s) = span_link(q);
				span_link(q) = s;
				span_count(s) = n;
				span_width(s) = w;
			} else if (span_width(span_link(q)) < w) {
				span_width(span_link(q)) = w;
			}
		} else if (w > unset_width(cur_align)) {
			unset_width(cur_align) = w;
		}
		type(u) = UNSET_NODE;
		unset_span_count(u) = n;
		o = get_stretch_order();
		glue_order(u) = o;
		unset_stretch(u) = tex::total_stretch[o];
		o = get_shrink_order();
		glue_sign(u) = o;
		unset_shrink(u) = tex::total_shrink[o];
		pop_nest();
		tail_append(u);
		tail_append(new_glue(glue_ptr(link(cur_align))));
		subtype(tail) = TAB_SKIP_CODE + 1;
		if (unset_info(cur_align) >= CR_CODE)
			return TRUE;
		init_span(p);
	}
	align_state = 1000000;
	get_nbx_token();
	cur_align = p;
	init_col();
	return FALSE;
}

void fin_row ()
	{
	ptr	p;
	
	if (mode == -HMODE) {
		p = hpack(link(head), 0, ADDITIONAL);
		pop_nest();
		append_to_vlist(p);
		if(cur_head != cur_tail) {
			link(tail) = link(cur_head);
			tail = cur_tail;
			}
		} 
	else {
		p = vpack(link(head), 0, ADDITIONAL);
		pop_nest();
		tail = link(tail) = p;
		space_factor = 1000;
		}
	type(p) = UNSET_NODE;
	unset_stretch(p) = 0;
	if (every_cr != null)
		begin_token_list(every_cr, EVERY_CR_TEXT);
	align_peek();
	}

void
fin_align ()
{
	int	n;
	scal	o;
	ptr	p;
	ptr	q;
	ptr	r;
	ptr	s=0;
	scal	t;
	ptr	u;
	ptr	v;
	scal	w;
	int	aux_save;
	scal	rule_save;

	if (cur_group != ALIGN_GROUP)
		confusion("align1");
	unsave();
	if (cur_group != ALIGN_GROUP)
		confusion("align0");
	unsave();
	if (nest_ptr[-1].mode_field == MMODE) {
		o = display_indent;
	} else {
		o = 0;
	}
	q = link(link(align_head));
	do {
		flush_list(u_part(q));
		flush_list(v_part(q));
		p = link(link(q));
		if (unset_width(q) == NULL_FLAG) {
			unset_width(q) = 0;
			r = link(q);
			s = glue_ptr(r);
			if (s != zero_glue) {
				add_glue_ref(zero_glue);
				delete_glue_ref(s);
				glue_ptr(r) = zero_glue;
			}
		}
		if (unset_span_list(q) != end_span) {
			t = unset_width(q) + glue_width(glue_ptr(link(q)));
			r = unset_span_list(q);
			s = end_span;
			span_link(s) = p;
			n = MIN_QUARTERWORD + 1;
			do {	
				span_width(r) -= t;
				u = span_link(r);
				while (span_count(r) > n) {
					s = span_link(s);
					n = span_count(span_link(s)) + 1;
				}
				if (span_count(r) < n) {
					span_link(r) = span_link(s);
					span_link(s) = r;
					decr(span_count(r));
					s = r;
				} else {
					if (span_width(r) > span_width(span_link(s)))
						span_width(span_link(s)) =
							span_width(r);
					free_node(r, SPAN_NODE_SIZE);
				}
				r = u;
			} while (r != end_span);
		}
		type(q) = UNSET_NODE;
		unset_span_count(q) = MIN_QUARTERWORD;
		box_height(q) = 0;
		box_depth(q) = 0;
		glue_order(q) = NORMAL;
		glue_sign(q) = NORMAL;
		unset_stretch(q) = 0;
		unset_shrink(q) = 0;
		q = p;
	} while (q != null);
	save_ptr -= 2;
	tex::pack_begin_line = -mode_line;
	if (mode == -VMODE) {
		rule_save = overfull_rule;
		overfull_rule = 0;
		p = hpack(link(align_head), saved(1), saved(0));
		overfull_rule = rule_save;
	} else {
		q = link(link(align_head));
		do {
			box_height(q) = box_width(q);
			box_width(q) = 0;
			q = link(link(q));
		} while (q != null);
		p = vpackage(link(align_head), saved(1), saved(0), MAX_DIMEN);
		q = link(link(align_head));
		do {
			box_width(q) = box_height(q);
			box_height(q) = 0;
			q = link(link(q));
		} while (q != null);
	}
	tex::pack_begin_line = 0;
	for (q = link(head); q != null; q = link(q)) {
		if (type(q) == UNSET_NODE) {
			if (mode == -VMODE) {
				type(q) = HLIST_NODE;
				box_width(q) = box_width(p);
			} else {
				type(q) = VLIST_NODE;
				box_height(q) = box_height(p);
			}
			glue_order(q) = glue_order(p);
			glue_sign(q) = glue_sign(p);
			glue_set(q) = glue_set(p);
			shift_amount(q) = o;
			r = link(list_ptr(q));
			s = link(list_ptr(p)); 
			do	{
				n = unset_span_count(r); 
				w = t = unset_width(s);
				u = hold_head;
				while (n > MIN_QUARTERWORD) {
					decr(n);
					s = link(s);
					v = glue_ptr(s);
					link(u) = new_glue(v);
					u = link(u);
					subtype(u) = TAB_SKIP_CODE + 1;
					t += glue_width(v);
					if (glue_sign(p) == STRETCHING) {
						if (stretch_order(v) == glue_order(p))
							t += round(glue_set(p) *
								stretch(v));
					} else if (glue_sign(p) == SHRINKING) {
						if (shrink_order(v) == glue_order(p))
							t -= round(glue_set(p) *
								shrink(v));
					}
					s = link(s);
					u = link(u) = new_null_box();
					t += box_width(s);
					if (mode == -VMODE) {
						box_width(u) = box_width(s);
					} else {
						type(u) = VLIST_NODE;
						box_height(u) = box_width(s);
					}
				}
				if (mode == -VMODE) {
					box_height(r) = box_height(q);
					box_depth(r) = box_depth(q);
					if (t == box_width(r)) {
						glue_sign(r) = NORMAL;
						glue_order(r) = NORMAL;
						glue_set(r) = 0.0;
					} else if (t > box_width(r)) {
						glue_sign(r) = STRETCHING;
						glue_set(r) = (unset_stretch(r) == 0) ? 0.0 : 
							(float) (t - glue_width(r)) / unset_stretch(r);
					} else {
						glue_order(r) = glue_sign(r);
						glue_sign(r) = SHRINKING;
						if (unset_shrink(r) == 0)
							glue_set(r) = 0.0;
						else if (glue_order(r) == NORMAL &&
							box_width(r) - t > unset_shrink(r))
							glue_set(r) = 1.0;
						else glue_set(r) = (float) (box_width(r) - t) /
											unset_shrink(r);
					}
					box_width(r) = w;
					type(r) = HLIST_NODE;
				} else {
					box_width(r) = box_width(q);
					if (t == box_height(r)) {
						glue_sign(r) = NORMAL;
						glue_order(r) = NORMAL;
						glue_set(r) = 0.0;
					} else if (t > box_height(r)) {
						glue_sign(r) = STRETCHING;
						glue_set(r) = (unset_stretch(r) == 0) ? 0.0 :
								(float) (t - box_height(r)) / unset_stretch(r);
					} else {
						glue_order(r) = glue_sign(r);
						glue_sign(r) = SHRINKING;
						if (unset_shrink(r) == 0)
							glue_set(r) = 0.0;
						else if (glue_order(r) == NORMAL &&
							box_height(r) - t > unset_shrink(r))
							glue_set(r) = 1.0;
						else glue_set(r) = (float) (box_height(r) - t) /
													unset_shrink(r);
					}
					box_height(r) = w;
					type(r) = VLIST_NODE;
				} 
				shift_amount(r) = 0; 
				if (u != hold_head) {
					link(u) = link(r);
					link(r) = link(hold_head);
					r = u;
				}
				r = link(link(r));
				s = link(link(s));
			} while (r != null);
		} else if (type(q) == RULE_NODE) {
			if (is_running(rule_width(q)))
				rule_width(q) = rule_width(p);
			if (is_running(rule_height(q)))
				rule_height(q) = rule_height(p);
			if (is_running(rule_depth(q)))
				rule_depth(q) = rule_depth(p);
			if (o != 0) {
				r = link(q);
				link(q) = null;
				q = hpack(q, 0, ADDITIONAL);
				shift_amount(q) = o;
				link(q) = r;
				link(s) = q;
			}
		}
	}
	flush_node_list(p);
	pop_alignment();
	aux_save = aux;
	p = link(head);
	q = tail;
	pop_nest();
	if (mode == MMODE) {
		do_assignments();
		if (cur_cmd != MATH_SHIFT) {
			print_err("Missing $$ inserted");
			help_fin_display_align();
			back_error();
		} else {	
			get_x_token();
			if (cur_cmd != MATH_SHIFT) {
				print_err("Display math should end with $$");
				help_fin_display();
				back_error();
			}
		}
		pop_nest();
		tail_append(new_penalty(pre_display_penalty));
		tail_append(new_param_glue(ABOVE_DISPLAY_SKIP_CODE));
		link(tail) = p;
		if (p != null)
			tail = q;
		tail_append(new_penalty(post_display_penalty));
		tail_append(new_param_glue(BELOW_DISPLAY_SKIP_CODE));
		prev_depth = aux_save;
		resume_after_display();
	} else {
		aux = aux_save;
		link(tail) = p;
		if (p != null)
			tail = q;
		if (mode == VMODE)
			build_page();
	}
}

void
_align_init ()
{
}
	
void
_align_init_once ()
{
	align_head = new_avail(); 
	end_span = new_node(SPAN_NODE_SIZE);
	span_count(end_span) = MAX_QUARTERWORD + 1;
	span_link(end_span) = null;
}

/*
**	Help text
*/

void help_display_align()
	{
	help3("Displays can use special alignments (like \\eqalignno)",
	"only if nothing but the alignment itself is between $$'s.",
	"So I've deleted the formulas that preceded this alignment.");
	}

void help_fin_display_align()
	{
	help2("Displays can use special alignments (like \\eqalignno)",
	"only if nothing but the alignment itself is between $$'s.");
	}

void help_preamble_missing()
	{
	help3("There should be exactly one # between &'s, when an",
	"\\halign or \\valign is being set up. In this case you had",
	"none, so I've put one in; maybe that will work.");
	}

void help_preamble_many()
	{
	help3("There should be exactly one # between &'s, when an",
	"\\halign or \\valign is being set up. In this case you had",
	"more than one, so I'm ignoring all but the first.");
	}

void help_align_apply()
	{
	help3("You have given more \\span or & marks than there were",
	"in the preamble to the \\halign or \\valign now in progress.",
	"So I'll assume that you meant to type \\cr instead.");
	}

void help_fin_display()
	{
	help2("The `$' that I just saw supposedly matches a previous `$$'.",
			"So I shall assume that you typed `$$' both times.");
	}

