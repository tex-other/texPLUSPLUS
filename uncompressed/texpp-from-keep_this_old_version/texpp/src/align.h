
/*
 * $Id: align.h,v 1.4 2002/04/19 11:21:10 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

struct align_stack_t {
	struct align_stack_t *align_stack_field;
	ptr	preamble_field;
	ptr	cur_align_field;
	ptr	cur_span_field;
	ptr	cur_loop_field;
	ptr	cur_tail_field;
	ptr	cur_head_field;
	int	align_state_field;
};
typedef struct align_stack_t align_stack_t;

#define ALIGN_STACK_NODE_SIZE	sizeof(align_stack_t)

extern	align_stack_t *align_stack;

extern	ptr	align_head;

//#define preamble	align_head->link()

extern	ptr	cur_align;
extern	ptr	cur_span;
extern	ptr	cur_loop;
extern	ptr	cur_head;
extern	ptr	cur_tail;

struct span_t {
	int	span_count_field;
	ptr	span_link_field;
	scal	span_width_field;
	ptr	fill_field;
};
typedef struct span_t span_t;

#define SPAN_NODE_SIZE		sizeof(span_t)
#define span_count(P)		((span_t *) (P))->span_count_field
#define span_link(P)		((span_t *) (P))->span_link_field
#define span_width(P)		((span_t *) (P))->span_width_field

extern	ptr	end_span;

#define SPAN_CODE		256
#define CR_CODE			257
#define CR_CR_CODE		CR_CODE + 1

void	push_alignment();
void	pop_alignment();
void	init_align();
void	get_preamble_token();
void	align_peek();
void	init_row();
void	init_span(ptr p);
void	init_col();
bool	fin_col();
void	fin_row();
void	fin_align();
void  help_display_align();
void  help_fin_display_align();
void  help_preamble_missing();
void  help_preamble_many();
void  help_align_apply();
void  help_fin_display();
void	_align_init();
void	_align_init_once();
