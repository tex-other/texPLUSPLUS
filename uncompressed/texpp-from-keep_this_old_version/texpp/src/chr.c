
/*
 * $Id: chr.c,v 1.2 2002/03/27 20:40:49 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include "tex.h"

byte	xord[256];
byte	xchr[256];

void _chr_init()
	{
	int	i;
	
	for (i = 0; i <= 0377; incr(i))
	  xchr[i] = i;
	for (i = 0; i <= 0377; incr(i))
	  xord[xchr[i]] = i;
	}

