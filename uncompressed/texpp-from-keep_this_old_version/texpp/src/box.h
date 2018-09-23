
/*
 * $Id: box.h,v 1.3 2002/04/18 09:50:43 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#define is_char_node(P)		((P) != null && type(P) >= FONT_BASE)
#define safe_is_char_node(P)	(type(P) >= FONT_BASE)

#define font			type
#define character		subtype

#define HLIST_NODE		0
#define VLIST_NODE		1

struct box_t {
	mcell	node_field;
	scal	width_field;
	scal	depth_field;
	scal	height_field;
	scal	shift_field;
	mcell	node_list_field;
	ratio	glue_set_field;
};
typedef struct box_t box_t;

#define BOX_NODE_SIZE		(sizeof(box_t))
#define box_width(B)		((box_t *) (B))->width_field
#define box_depth(B)		((box_t *) (B))->depth_field
#define box_height(B)		((box_t *) (B))->height_field
#define shift_amount(B)		((box_t *) (B))->shift_field
#define node_list(B)		((ptr) & ((box_t *) (B))->node_list_field)
#define list_ptr(B)		link(node_list(B))
#define glue_order(B)		type(node_list(B))
#define glue_sign(B)		subtype(node_list(B))
#define glue_set(B)		((box_t *) (B))->glue_set_field
#define NORMAL			0
#define STRETCHING		1
#define SHRINKING		2


#define RULE_NODE		2

struct rule_t {
	mcell	node_field;
	scal	width_field;
	scal	depth_field;
	scal	height_field;
};
typedef struct rule_t rule_t;

#define RULE_NODE_SIZE		(sizeof(rule_t))
#define rule_width(R)		((rule_t *) (R))->width_field
#define rule_depth(R)		((rule_t *) (R))->depth_field
#define rule_height(R)		((rule_t *) (R))->height_field
#define NULL_FLAG		-010000000000
#define is_running(R)		((R) == NULL_FLAG)

#define INS_NODE		3

struct ins_t {
	mcell	node_field;
	int	cost_field;
	scal	height_field;
	scal	depth_field;
	ptr	ins_field;
	ptr	split_top_field;
};
typedef struct ins_t ins_t;

#define INS_NODE_SIZE		(sizeof(ins_t))
#define float_cost(I)		((ins_t *) (I))->cost_field
#define ins_height(I)		((ins_t *) (I))->height_field
#define ins_depth(I)		((ins_t *) (I))->depth_field
#define ins_ptr(I)		((ins_t *) (I))->ins_field
#define split_top_ptr(I)	((ins_t *) (I))->split_top_field

#define SMALL_NODE_SIZE		(2 * sizeof(mcell))

#define MARK_NODE		4

struct mark_t {
	mcell	node_field;
	ptr	mark_field;
	int	fill_field;
};
typedef struct mark_t mark_t;

#define mark_ptr(M)		((mark_t *) (M))->mark_field
	
#define ADJUST_NODE		5

struct adjust_t {
	mcell	node_field;
	ptr	adjust_field;
	int	fill_field;
};
typedef struct adjust_t adjust_t;

#define adjust_ptr(A)		((adjust_t *) (A))->adjust_field

#define LIGATURE_NODE		6

struct lig_t {
	mcell	node_field;
	mcell	lig_field;
};
typedef struct lig_t lig_t;

#define lig_char(L)		((ptr) &((lig_t *) (L))->lig_field)
#define lig_ptr(L)		link(lig_char(L))

struct disc_t {
	mcell	node_field;
	ptr	pre_field;
	ptr	post_field;
};
typedef struct disc_t disc_t;

#define DISC_NODE		7
#define replace_count		subtype
#define pre_break(D)		((disc_t *) (D))->pre_field
#define post_break(D)		((disc_t *) (D))->post_field

#define WHATSIT_NODE		8

#define MATH_NODE		9

struct math_t {
	mcell	node_field;
	scal	width_field;
	int	fill_field;
};
typedef struct math_t math_t;

#define math_width(M)		((math_t *) (M))->width_field
#define BEFORE			0
#define AFTER			1

#define precedes_break(M)	(type(M) < MATH_NODE)
#define non_discardable(M)	(type(M) < MATH_NODE)

#define GLUE_NODE		10

struct glue_t {
	mcell	node_field;
	ptr	glue_field;
	ptr	leader_field;
};
typedef struct glue_t glue_t;

#define glue_ptr(G)		((glue_t *) (G))->glue_field
#define leader_ptr(G)		((glue_t *) (G))->leader_field
#define COND_MATH_GLUE		98
#define MU_GLUE			99
#define A_LEADERS		100
#define C_LEADERS		101
#define X_LEADERS		102

struct spec_t {
	int	ref_count_field;
	short	stretch_order_field;
	short	shrink_order_field;
	scal	width_field;
	scal	stretch_field;
	scal	shrink_field;
};
typedef struct spec_t spec_t;

#define GLUE_SPEC_SIZE		sizeof(spec_t)
#define stretch_order(S)	((spec_t *) (S))->stretch_order_field
#define shrink_order(S)		((spec_t *) (S))->shrink_order_field
#define glue_ref_count(S)	((spec_t *) (S))->ref_count_field
#define glue_width(S)		((spec_t *) (S))->width_field
#define stretch(S)		((spec_t *) (S))->stretch_field
#define shrink(S)		((spec_t *) (S))->shrink_field
#define FIL			1
#define FILL			2
#define FILLL			3

#define KERN_NODE		11

struct kern_t {
	mcell	node_field;
	scal	kern_field;
	int	fill_field;
};
typedef struct kern_t kern_t;

#define kern_width(K)		((kern_t *) (K))->kern_field
#define EXPLICIT		1
#define ACC_KERN		2

#define PENALTY_NODE		12

struct pen_t {
	mcell	node_field;
	int	penalty_field;
	int	fill_field;
};
typedef struct pen_t pen_t;

#define penalty(P)		((pen_t *) (P))->penalty_field
#define INF_PENALTY		10000
#define EJECT_PENALTY		-INF_PENALTY

#define UNSET_NODE		13

struct unset_t {
	mcell	node_field;
	scal	width_field;
	ptr	u_ptr_field;
	ptr	v_ptr_field;
	scal	stretch_field;
	mcell	info_field;
	scal	shrink_field;
};
typedef struct unset_t unset_t;

#define unset_span_count	subtype
#define unset_span_list		llink
#define	end_link		llink
#define unset_width(U)		((unset_t *) (U))->width_field
#define u_part(U)		((unset_t *) (U))->u_ptr_field
#define v_part(U)		((unset_t *) (U))->v_ptr_field
#define unset_info(U)		info((ptr)&((unset_t *) (U))->info_field)
#define unset_stretch(U)	((unset_t *) (U))->stretch_field
#define unset_shrink(U)		((unset_t *) (U))->shrink_field


#define node_list_display(N) \
	{append_char('.'); show_node_list(N); flush_char();}

extern	fnt	font_in_short_display;
extern	int	depth_threshold;
extern	int	breadth_max;

#define fast_delete_glue_ref(G)	\
{	if (glue_ref_count(G) == 0) { \
		free_node(G, GLUE_SPEC_SIZE); \
	} else { \
		decr(glue_ref_count(G)); \
	} \
}

#define add_glue_ref(G) \
	(incr(glue_ref_count(G)))


ptr	new_null_box();
ptr	new_rule();
ptr	new_ligature(int f, int c, ptr q);
ptr   new_lig_item(int c);
ptr	make_char_from_lig(ptr p);
ptr	new_disc();
ptr	new_math(scal w, int s);
ptr	new_spec(ptr p);
ptr	new_param_glue(int n);
ptr	new_glue(ptr q);
ptr	new_skip_param(int n);
ptr	new_kern(scal w);
ptr	new_penalty(int i);
void	print_short_display();
void	print_font_and_char(ptr p);
void	print_mark(ptr p);
void	print_rule_dimen(scal d);
void	print_glue(scal d, int o, str s);
void	print_spec(ptr p, str s); 
void	print_skip_param();
void	short_display(ptr p);
void	show_box(ptr p);
void  show_glue_set(ptr p);
void	show_box1(ptr p);
void  show_rule(ptr p);
void  show_insertion(ptr p);
void  show_leaders(ptr p);
void  show_glue_type(int n);
void  show_glue(ptr p);
void  show_kern(ptr p);
void  show_math(ptr p);
void  show_ligature(ptr p);
void  show_discretionary(ptr p);
void  show_penalty(ptr p);
void  show_mark(ptr p);
void  show_adjust(ptr p);
void	show_node_list(ptr p);
ptr	copy_node_list(ptr p);
void	flush_node_list(ptr p);
void	delete_glue_ref(ptr p);
void  flush_node_list(ptr p);
void	_box_init();
void	_box_init_once();
