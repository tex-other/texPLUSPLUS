
/*
 * $Id: expand.h,v 1.3 2002/04/18 09:50:43 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */


#define TOP_MARK_CODE		0
#define FIRST_MARK_CODE		1
#define BOT_MARK_CODE		2
#define SPLIT_FIRST_MARK_CODE	3
#define SPLIT_BOT_MARK_CODE	4

#define top_mark		cur_mark[TOP_MARK_CODE]
#define first_mark		cur_mark[FIRST_MARK_CODE]
#define bot_mark		cur_mark[BOT_MARK_CODE]
#define split_first_mark	cur_mark[SPLIT_FIRST_MARK_CODE]
#define split_bot_mark		cur_mark[SPLIT_BOT_MARK_CODE]

extern	ptr	cur_mark[];

extern	int	long_state;
extern	ptr	pstack[];

void	get_x_token();
void	expand();
void	insert_relax();
void	macro_call();
void	x_token();
void	runaway_arg(int n);
void  help_runaway();
void  help_match();
void  help_match_xtra();
void  help_undefd();
void  help_cs();
void  help_extra_if();
void	_expand_init();
void	_expand_init_once();
