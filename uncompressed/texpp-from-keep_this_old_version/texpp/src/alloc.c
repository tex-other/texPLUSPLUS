
/*
 * $Id: alloc.c,v 1.3 2002/03/28 12:47:42 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include "tex.h"

ptr     avail;
ptr	avail_ptr;
ptr	avail_end;
	
struct blk_t {
	int	size_field;
	ptr	free_field;
	ptr	rblk_field;
	ptr	lblk_field;
};
typedef struct blk_t blk_t;

#define size(B)		((blk_t *) B)->size_field
#define free(B) 	((blk_t *) B)->free_field
#define rblk(B)		((blk_t *) B)->rblk_field
#define lblk(B)		((blk_t *) B)->lblk_field

#define is_free(B)	(free(B) == nil)

ptr	rover;
ptr	nil;

#ifdef STAT
int     dyn_used;
int	var_used;
#endif

#define BLOCK_SIZE	(8*8192)

ptr new_avail()
	{
	ptr	p=avail;

	if (p != null) {
		avail = link(avail);
		} 
	else if (avail_ptr < avail_end) {
		p = avail_ptr;
		avail_ptr += sizeof(mcell);
		} 
	else {
		p = avail_ptr = (ptr) malloc(BLOCK_SIZE);
		if (avail_ptr == null)
			overflow("new avail", BLOCK_SIZE);
		avail_end = avail_ptr + BLOCK_SIZE;
		avail_ptr += sizeof(mcell);
		}
	link(p) = null;
	type(p) = -1;
#ifdef STAT
	incr(dyn_used);
#endif
	return p;
	}

ptr new_node(int s)
	{
	ptr	p;
	ptr	q;
	ptr	r;
	ptr	t;
	
	restart:
	p = rover;
	do {
		q = p + size(p);
		while (is_free(q)) {
			t = rblk(q);
			if (q == rover)
				rover = t;
			lblk(t) = lblk(q);
			rblk(lblk(q)) = t;
			q += size(q);
			}
		r = q - s;
		if (r - p > (int)sizeof(blk_t))  {
			size(p) = r - p;
			rover = p;
			goto found;
			}
		if (r == p && rblk(p) != p) {
			rover = rblk(p);
			t = lblk(p);
			lblk(rover) = t;
			rblk(t) = rover;
			goto found;
			}
		size(p) = q - p;
		p = rblk(p);
		} 
	while (p != rover);
	
	q = (ptr)malloc(BLOCK_SIZE);
	if (q == null)
		overflow("get node", BLOCK_SIZE);
	p = lblk(rover);
	rblk(p) = q;
	lblk(rover) = q;
	rblk(q) = rover;
	lblk(q) = p;
	free(q) = nil;
	size(q) = BLOCK_SIZE - sizeof(blk_t);
	memset((void *)(q + size(q)), 0, sizeof(blk_t));
	rover = q;
	goto restart;
	
   found:
#ifdef STAT
	var_used += s;
#endif
	link(r) = null;
	return r;
	}

void free_node(ptr p, int s)
	{
	ptr     q;
	
	size(p) = s;
	free(p) = nil;
	q = lblk(rover);
	lblk(p) = q;
	rblk(p) = rover;
	lblk(rover) = p;
	rblk(q) = p;
#ifdef STAT
	var_used -= s;
#endif
	}

void _alloc_init_once()
	{
	nil = (ptr)&nil;
	avail = null;
	rover = (ptr)malloc(BLOCK_SIZE);
	if (rover == null)
		overflow("init alloc", BLOCK_SIZE);
	size(rover) = BLOCK_SIZE - sizeof(blk_t);
	free(rover) = nil;
	lblk(rover) = rover;
	rblk(rover) = rover;
	memset((void *)(rover + size(rover)), 0, sizeof(blk_t));
#ifdef STAT
	dyn_used = 0;
	var_used = 0;
#endif
	}
