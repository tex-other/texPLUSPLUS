
/*
 * $Id: box.c,v 1.5 2002/04/19 11:21:10 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include <cstdlib>
#include <cmath>
#include "tex.h"

int	depth_threshold;
int	breadth_max;
fnt	font_in_short_display;

ptr
new_null_box ()
{
	ptr	p;

	p = new_node(BOX_NODE_SIZE);
	type(p) = HLIST_NODE;
	subtype(p) = MIN_QUARTERWORD;
	box_width(p) = 0;
	box_depth(p) = 0;
	box_height(p) = 0;
	shift_amount(p) = 0;
	list_ptr(p) = null;
	glue_sign(p) = NORMAL;
	glue_order(p) = NORMAL;
	glue_set(p) = 0.0;

	return p;
}

ptr
new_rule ()
{
	ptr	p;

	p = new_node(RULE_NODE_SIZE);
	type(p) = RULE_NODE;
	subtype(p) = 0;
	rule_width(p) = NULL_FLAG;
	rule_depth(p) = NULL_FLAG;
	rule_height(p) = NULL_FLAG;

	return p;
}

ptr new_ligature(int f, int c, ptr q)
	{
	ptr 	p;

	p = new_node(SMALL_NODE_SIZE);
	type(p) = LIGATURE_NODE;
	subtype(p) = 0;
	font(lig_char(p)) = f;
	character(lig_char(p)) = c;
	lig_ptr(p) = q;

	return p;
	}

ptr new_lig_item(int c)
	{
	ptr	p;

	p = new_node(SMALL_NODE_SIZE);
	type(p) = LIGATURE_NODE;
	subtype(p) = c;
	lig_ptr(p) = null;

	return p;
	}

ptr make_char_from_lig(ptr p) 
	{
	static mcell m;
	mcopy(&m, lig_char(p));
	link((ptr)&m) = link(p);
	return ((ptr)&m);
	}

ptr new_disc ()
	{
	ptr	p;

	p = new_node(SMALL_NODE_SIZE);
	type(p) = DISC_NODE;
	replace_count(p) = 0;
	pre_break(p) = null;
	post_break(p) = null;

	return p;
	}

ptr new_math(scal w, int s)
	{
	ptr	p;

	p = new_node(SMALL_NODE_SIZE);
	type(p) = MATH_NODE;
	subtype(p) = s;
	math_width(p) = w;

	return p;
	}

ptr new_spec(ptr p)
	{
	ptr	q;

	q = new_node(GLUE_SPEC_SIZE);
	stretch_order(q) = stretch_order(p);
	shrink_order(q) = shrink_order(p);
	glue_ref_count(q) = 0;
	glue_width(q) = glue_width(p);
	stretch(q) = stretch(p);
	shrink(q) = shrink(p);

	return q;
	}

ptr new_param_glue(int n)
	{
	ptr	p;
	ptr	q;

	p = new_node(SMALL_NODE_SIZE);
	type(p) = GLUE_NODE;
	subtype(p) = n + 1;
	leader_ptr(p) = null;
	q = glue_par(n);
	glue_ptr(p) = q;
	incr(glue_ref_count(q));

	return p;
	}

ptr new_glue(ptr q)
	{
	ptr	p;

	p = new_node(SMALL_NODE_SIZE);
	type(p) = GLUE_NODE;
	subtype(p) = NORMAL;
	leader_ptr(p) = null;
	glue_ptr(p) = q;
	incr(glue_ref_count(q));

	return p;
	}

ptr new_skip_param(int n)
	{
	ptr	p;
	ptr	q;

	q = new_spec(glue_par(n));
	p = new_glue(q);
	glue_ref_count(q) = 0;
	subtype(p) = n + 1;

	return p;
	}

ptr new_kern(scal w)
	{
	ptr	p;

	p = new_node(SMALL_NODE_SIZE);
	type(p) = KERN_NODE;
	subtype(p) = NORMAL;
	kern_width(p) = w;

	return p;
	}

ptr new_penalty(int i)
	{
	ptr	p;

	p = new_node(SMALL_NODE_SIZE);
	type(p) = PENALTY_NODE;
	subtype(p) = 0;
	penalty(p) = i;

	return p;
	}

void print_font_and_char(ptr p)
	{
	if (font(p) < FONT_BASE || font(p) > FONT_MAX) {
		print("* ");
		} 
	else {
		print_cs((char *)font_id(font(p)));
		print_ASCII(character(p));
		}
	}

void print_mark(ptr p)
	{
	print("{");
	show_token_list(token_link(p), null, MAX_PRINT_LINE - 10);
	print("}");
	}

void print_rule_dimen(scal d)
	{
	if (is_running(d))
		print("*");
	else print_scaled(d);
	}

void print_glue(scal d, int o, str s)
	{
	print_scaled(d);
	if (o < NORMAL || o > FILLL) {
		print("foul");
		} 
	else if (o > NORMAL) {
		print("fil");
		while  (o >  FIL) {
			print("l");
			decr(o);
			}
		} 
	else if (s) {
		print(s);
		}
	}

void print_spec(ptr p, str s)
	{
	print_scaled(glue_width(p));
	print(s);
	if (stretch(p) != 0) {
		print(" plus ");
		print_glue(stretch(p), stretch_order(p), s);
		}
	if (shrink(p) != 0) {
		print(" minus ");
		print_glue(shrink(p), shrink_order(p), s);
		}
	}

void short_display(ptr p)
	{
	int	n;

	for (; p != null; p = link(p)) {
		if (is_char_node(p)) {
			if (font(p) != font_in_short_display) {
				if (font(p) < FONT_BASE || font(p) > FONT_MAX) {
					print("*");
				} else {
					print_esc(null_str);
					print(font_id_text(font(p)));
				}
				print(" ");
				font_in_short_display = font(p);
			}
			print_ASCII(character(p));
		} else {
			switch (type(p))
			{
			case HLIST_NODE:
			case VLIST_NODE: 
			case INS_NODE:
			case WHATSIT_NODE:
			case MARK_NODE:
			case ADJUST_NODE:
			case UNSET_NODE:
				print("[]");
				break;

			case RULE_NODE:
				print("|");
				break;

			case GLUE_NODE:
				if (glue_ptr(p) != zero_glue)
					print(" ");
				break;

			case MATH_NODE:
				print("$");
				break;

			case LIGATURE_NODE:
				short_display(lig_ptr(p));
				break;

			case DISC_NODE:
				short_display(pre_break(p));
				short_display(post_break(p));
				n = replace_count(p);
				while (n > 0) {
					if (link(p) != null)
						p = link(p);
					decr(n);
				}
				break;

			default:
				break;
			}
		}
	}
}

void show_box(ptr p)
	{
	depth_threshold = show_box_depth;
	breadth_max = show_box_breadth;
	if (breadth_max <= 0)
		breadth_max = 5;
	flush_str();
	if (cur_str_ptr + depth_threshold >= cur_str_end)
		depth_threshold = cur_str_end - cur_str_ptr - 1;
	show_node_list(p);
	}

void show_glue_set(ptr p)
	{
	if (glue_set(p) != 0 && glue_sign(p) != NORMAL) {
		print(", glue set ");
		if (glue_sign(p) == SHRINKING)
			print("- ");
		if (fabs(glue_set(p)) > 20000.0) {
			if (glue_set(p) > 0)
				print(">");
			else print("< -");
			print_glue(20000 * UNITY, glue_order(p), null_str);
		} else {
			print_glue(round(glue_set(p) * UNITY),
				glue_order(p), null_str);
		}
	}
}

void show_box1(ptr p)
	{
	if (type(p) == HLIST_NODE)
		print_esc("h");
	else if (type(p) == VLIST_NODE)
		print_esc("v");
	else print_esc("unset");
	print("box(");
	print_scaled(box_height(p));
	print("+")	;
	print_scaled(box_depth(p));
	print(")x")	;
	print_scaled(box_width(p));
	if (type(p) == UNSET_NODE) {
		if (unset_span_count(p) != MIN_QUARTERWORD) {
			print(" (");
			print_int(unset_span_count(p)+1);
			print(" columns)");
		}
		if (unset_stretch(p) != 0) {
			print(", stretch ");
			print_glue(unset_stretch(p), glue_order(p), null_str);
		}
		if (unset_shrink(p) != 0) {
			print(", shrink ");
			print_glue(unset_shrink(p), glue_sign(p), null_str);
		}
	} else {
		show_glue_set(p);
		if (shift_amount(p) != 0) {
			print(", shifted ");
			print_scaled(shift_amount(p));
		}
	}
	node_list_display(list_ptr(p));
}

void show_rule(ptr p)
	{
	print_esc("rule(");
	print_rule_dimen(rule_height(p));
	print("+");
	print_rule_dimen(rule_depth(p));
	print(")x");
	print_rule_dimen(rule_width(p));
	}

void show_insertion(ptr p)
	{
	print_esc("insert");
	print_int(subtype(p));
	print(", natural size ");
	print_scaled(ins_height(p));
	print("; split(");
	print_spec(split_top_ptr(p), null_str);
	print(",");
	print_scaled(ins_depth(p));
	print("); float cost ");
	print_int(float_cost(p));
	node_list_display(ins_ptr(p));
	}

void show_leaders(ptr p)
	{
	print_esc(null_str);
	if (subtype(p) == C_LEADERS)
		print("c");
	else if (subtype(p) == X_LEADERS)
		print("x");
	print("leaders ");
	print_spec(glue_ptr(p), null_str);
	node_list_display(leader_ptr(p));
}

void show_glue_type(int n)
	{
	print("(");
	if (n < COND_MATH_GLUE) {
		if (n <= GLUE_PARS) {
			print_skip_param(n - 1);
		} else {
			print_mu_skip_param(n - 1 - GLUE_PARS);
		}
	} else if (n == COND_MATH_GLUE) {
		print_esc("nonscript");
	} else {
		print_esc("mskip");
	}
	print(")");
}

void show_glue(ptr p)
	{
	if (subtype(p) >= A_LEADERS) {
		show_leaders(p);
	} else {
		print_esc("glue");
		if (subtype(p) != NORMAL)
			show_glue_type(subtype(p));
		if (subtype(p) != COND_MATH_GLUE) {
			print(" ");
			if (subtype(p) < COND_MATH_GLUE)
				print_spec(glue_ptr(p), null_str);
			else print_spec(glue_ptr(p), "mu");
		}
	}
}

void show_kern(ptr p)
	{
	if (subtype(p) != MU_GLUE) {
		print_esc("kern");
		if (subtype(p) != NORMAL)
			print(" ");
		print_scaled(kern_width(p));
		if (subtype(p) == ACC_KERN)
			print(" (for accent)");
	} else {
		print_esc("mkern");
		print_scaled(kern_width(p));
		print("mu");
	}
	}

void show_math(ptr p)
	{
	print_esc("math");
	if (subtype(p) == BEFORE) {
		print("on");
	} else {
		print("off");
	}
	if (math_width(p) != 0) {
		print(", surrounded ");
		print_scaled(math_width(p));
	}
	}
	
void show_ligature(ptr p)
	{
	print_font_and_char(lig_char(p));
	print(" (ligature ");
	if (subtype(p) > 1)
		print("|");
	font_in_short_display = font(lig_char(p));
	short_display(lig_ptr(p));
	if (odd(subtype(p)))
		print("|");
	print(")");
	}

void show_discretionary(ptr p)
	{
	print_esc("discretionary");
	if (replace_count(p) > 0) {
		print(" replacing ");
		print_int(replace_count(p));
	}
	node_list_display(pre_break(p));
	append_char('|');
	show_node_list(post_break(p));
	flush_char();
	}

void show_penalty(ptr p)
	{
	print_esc("penalty ");
	print_int(penalty(p));
	}

void show_mark(ptr p)
	{
	print_esc("mark");
	print_mark(mark_ptr(p));
	}

void show_adjust(ptr p)
	{
	print_esc("vadjust");
	node_list_display(adjust_ptr(p));
	}

void show_node_list(ptr p)
	{
	int	n;

	if (cur_length() > depth_threshold) {
		if (p > null)
			print(" []");
		return;
	}
	n = 0;
	while (p > null) {
		print_ln();
		print_str();
		incr(n);
		if (n > breadth_max) {
			print("etc.");
			return;
		}
		if (is_char_node(p)) {
			print_font_and_char(p);
		} else {
			switch (type(p))
			{
			case HLIST_NODE:
			case VLIST_NODE:
			case UNSET_NODE:
				show_box1(p);
				break;

			case RULE_NODE:
				show_rule(p);
				break;
			
			case INS_NODE:
				show_insertion(p);
				break;
			
			case WHATSIT_NODE:
				show_whatsit(p);
				break;
			
			case GLUE_NODE:
				show_glue(p);
				break;
			
			case KERN_NODE:
				show_kern(p);
				break;
			
			case MATH_NODE:
				show_math(p);
				break;
			
			case LIGATURE_NODE:
				show_ligature(p);
				break;

			case PENALTY_NODE:
				show_penalty(p);
				break;
		
			case DISC_NODE:
				show_discretionary(p);
				break;
			
			case MARK_NODE:
				show_mark(p);
				break;
			
			case ADJUST_NODE:
				show_adjust(p);
				break;
			
			case STYLE_NODE:
				print_style(subtype(p));
				break;
			
			case CHOICE_NODE:
				show_choice_node(p);
				break;

			case INNER_NOAD:
			case ORD_NOAD:
			case OP_NOAD:
			case BIN_NOAD:
			case REL_NOAD:
			case OPEN_NOAD:
			case CLOSE_NOAD:
			case PUNCT_NOAD:
			case RADICAL_NOAD:
			case OVER_NOAD:
			case UNDER_NOAD:
			case VCENTER_NOAD:
			case ACCENT_NOAD:
			case LEFT_NOAD:
			case RIGHT_NOAD:
				show_normal_noad(p);
				break;
			
			case FRACTION_NOAD:
				show_fraction_noad(p);
				break;

			default:
				print("Unknown node type!");
				break;
			}
		}
		p = link(p);
	}
}

void delete_glue_ref(ptr p)
	{
	if (glue_ref_count(p) == 0) {
		free_node(p, GLUE_SPEC_SIZE);
		} 
	else {
		decr(glue_ref_count(p));
		}
	}

void flush_node_list(ptr p)
	{
	ptr	q;

	while (p != null) {
		q = link(p);
		if (is_char_node(p)) {
			free_avail(p);
		} else {
			switch (type(p))
			{
			case HLIST_NODE:
			case VLIST_NODE:
			case UNSET_NODE:
				flush_node_list(list_ptr(p));
				free_node(p, BOX_NODE_SIZE);
				goto done;
					
			case RULE_NODE:
				free_node(p, RULE_NODE_SIZE);
				goto done;

			case INS_NODE:
				flush_node_list(ins_ptr(p));
				delete_glue_ref(split_top_ptr(p));
				free_node(p, INS_NODE_SIZE);
				goto done;

			case WHATSIT_NODE:
				free_whatsit(p);
				goto done;
			
			case GLUE_NODE:
				fast_delete_glue_ref(glue_ptr(p));
				if (leader_ptr(p) != null)
					flush_node_list(leader_ptr(p));
				break;

			case KERN_NODE:
			case MATH_NODE:
			case PENALTY_NODE:
				break;

			case LIGATURE_NODE:
				flush_node_list(lig_ptr(p));
				break;

			case MARK_NODE:
				delete_token_ref(mark_ptr(p));
				break;
			
			case DISC_NODE:
				flush_node_list(pre_break(p));
				flush_node_list(post_break(p));
				break;

			case ADJUST_NODE:
				flush_node_list(adjust_ptr(p));
				break;

			case STYLE_NODE:
				free_node(p, STYLE_NODE_SIZE);
				goto done;

			case CHOICE_NODE:
				flush_node_list(display_mlist(p));
				flush_node_list(text_mlist(p));
				flush_node_list(script_mlist(p));
				flush_node_list(script_script_mlist(p));
				free_node(p, STYLE_NODE_SIZE);
				goto done;

			case ORD_NOAD:
			case OP_NOAD:
			case BIN_NOAD:
			case REL_NOAD:
			case OPEN_NOAD:
			case CLOSE_NOAD:
			case PUNCT_NOAD:
			case INNER_NOAD:
			case RADICAL_NOAD:
			case OVER_NOAD:
			case UNDER_NOAD:
			case VCENTER_NOAD:
			case ACCENT_NOAD:
				if (math_type(nucleus(p)) >= SUB_BOX)
					flush_node_list(math_link(nucleus(p)));
				if (math_type(supscr(p)) >= SUB_BOX)
					flush_node_list(math_link(supscr(p)));
				if (math_type(subscr(p)) >= SUB_BOX)
					flush_node_list(math_link(subscr(p)));
				if (type(p) == RADICAL_NOAD)
					free_node(p, RADICAL_NOAD_SIZE);
				else if (type(p) == ACCENT_NOAD)
					free_node(p, ACCENT_NOAD_SIZE);
				else free_node(p, NOAD_SIZE);
				goto done;
			
			case LEFT_NOAD:
			case RIGHT_NOAD:
				free_node(p, NOAD_SIZE);
				goto done;
			
			case FRACTION_NOAD:
				flush_node_list(math_link(numerator(p)));
				flush_node_list(math_link(denominator(p)));
				free_node(p, FRACTION_NOAD_SIZE);
				goto done;

			default:
				confusion("flushing");
				break;
			}
			free_node(p, SMALL_NODE_SIZE);
			done:;	
		}
		p = q;
	}
}

ptr copy_node_list(ptr p)
	{
	ptr	h;
	ptr	q;
	ptr	r=0;

	h = q = new_avail();
	while (p != null) {
		if (is_char_node(p)) {
			r = new_avail();
			font(r) = font(p);
			character(r) = character(p);
		} else {
			switch (type(p))
			{
			case HLIST_NODE:
			case VLIST_NODE:
			case UNSET_NODE:
				r = new_node(BOX_NODE_SIZE);
				memcpy((void *)r, (void *)p, BOX_NODE_SIZE);
				list_ptr(r) = copy_node_list(list_ptr(p));
				break;
			
			case RULE_NODE:
				r = new_node(RULE_NODE_SIZE);
				memcpy((void *)r, (void *)p, RULE_NODE_SIZE);
				break;

			case INS_NODE:
				r = new_node(INS_NODE_SIZE);
				memcpy((void *)r, (void *)p, INS_NODE_SIZE);
				add_glue_ref(split_top_ptr(p));
				ins_ptr(r) = copy_node_list(ins_ptr(p));
				break;

			case WHATSIT_NODE:
				r = copy_whatsit(p);
				break;
			
			case GLUE_NODE:
				r = new_node(SMALL_NODE_SIZE);
				memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
				add_glue_ref(glue_ptr(p));
				glue_ptr(r) = glue_ptr(p);
				leader_ptr(r) = copy_node_list(leader_ptr(p));
				break;
			
			case KERN_NODE:
			case MATH_NODE:
			case PENALTY_NODE:
				r = new_node(SMALL_NODE_SIZE);
				memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
				break;
			
			case LIGATURE_NODE:
				r = new_node(SMALL_NODE_SIZE);
				memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
				lig_ptr(r) = copy_node_list(lig_ptr(p));
				break;
				
			case DISC_NODE:
				r = new_node(SMALL_NODE_SIZE);
				memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
				pre_break(r) = copy_node_list(pre_break(p));
				post_break(r) = copy_node_list(post_break(p));
				break;

			case MARK_NODE:
				r = new_node(SMALL_NODE_SIZE);
				memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
				add_token_ref(mark_ptr(p));
				break;

			case ADJUST_NODE:
				r = new_node(SMALL_NODE_SIZE);
				memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
				adjust_ptr(r) = copy_node_list(adjust_ptr(p));
				break;
			
			default:
				confusion("copying");
				break;
			}
		}
		q = link(q) = r;
		p = link(p);
	}
	link(q) = null;
	q = link(h);
	free_avail(h);
	return q;
}

void
_box_init ()
{
}

void
_box_init_once ()
{
}
