
/*
 * $Id: alloc.h,v 1.3 2002/04/18 09:50:43 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#define null 0

extern	ptr	avail;

#define fast_new_avail(M) \
    {M = avail; if (M == 0) M = new_avail(); \
    else {avail = link(M); link(M) = 0;}}
#define free_avail(M) \
    {link(M) = avail; avail = M;}

extern	ptr	rover;

ptr	new_avail();
ptr	new_node(int s);
void	free_node(ptr p, int s);
void	_alloc_init();
void	_alloc_init_once();
