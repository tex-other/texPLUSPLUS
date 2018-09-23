
/*
 * $Id: pack.h,v 1.5 2002/04/19 11:21:10 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#define EXACTLY		0
#define ADDITIONAL	1

namespace tex {
	extern ptr	adjust_head;
	extern ptr	adjust_tail;
	extern scal	total_stretch[];
	extern scal	total_shrink[];
	extern int	last_badness;
	extern int	pack_begin_line;
}

int   get_stretch_order();
int   get_shrink_order();
void  clr_dimens();
ptr	hpack(ptr p, scal w, int m);
ptr   vpack(ptr p, scal h, int m);
ptr	vpackage(ptr p, scal h, int m, scal l);
void	_pack_init();
void	_pack_init_once();
