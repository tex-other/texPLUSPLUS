
/*
 * Common TeX:eval.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#define VMODE		1
#define HMODE		(VMODE + MAX_COMMAND + 1)
#define MMODE		(HMODE + MAX_COMMAND + 1)

extern	ptr	lig_stack;
extern	int	cur_l;
extern	int	cur_r;
extern	ptr	cur_q;
extern	bool	lft_hit;
extern	bool	rt_hit;
extern	bool	ligature_present;

extern	void	main_control();
extern	void	app_space();

extern void	push_nest();
extern void	pop_nest();
extern bool	realloc_nest();
extern void	print_mode(int);

#define IGNORE_DEPTH	-65536000

struct list {
	int	mode_field;
	ptr	head_field;
	ptr	tail_field;
	int	pg_field;
	int	aux_field;
	int	ml_field;
	int	clang_field;
	short	lhm_field;
	short	rhm_field;
};
typedef struct list list;

extern	list	cur_list;

extern	list	*nest;
extern	list	*nest_end;
extern	list	*nest_ptr;
extern	list	*max_nest_stack;

#define mode			cur_list.mode_field
#define head			cur_list.head_field
#define tail			cur_list.tail_field
#define prev_graf		cur_list.pg_field
#define aux			cur_list.aux_field
#define prev_depth		aux
#define space_factor		aux
#define incompleat_noad		aux
#define mode_line		cur_list.ml_field
#define clang			cur_list.clang_field
#define lhmin			cur_list.lhm_field
#define rhmin			cur_list.rhm_field

extern	int		shown_mode;

extern void	show_activities();

#define tail_append(N)		{tail = link(tail) = N;}

extern void	insert_dollar_sign();
extern void	you_cant();
extern void	report_illegal_case();
extern bool	privileged();
extern void	missing_font();
extern bool	its_all_over();
void   help_dollar();
void   help_illegal_case();
extern void	_eval_init();
extern void	_eval_init_once();
