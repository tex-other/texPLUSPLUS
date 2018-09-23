
/*
 * Common TeX:arith.h:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#include <string>
#include <iostream>

#define UNITY	0200000
#define TWO	0400000

int	half(int);
scal	round_decimals(int);
void	print_scaled(scal s);
scal	mult_and_add(int n, scal x, scal y, scal max_answer);
scal	x_over_n(scal x, int n);
scal	xn_over_d(scal x, int n, int d);
int	badness(scal t, scal s);

std::string scal2string(scal s);

namespace tex {
	extern bool	arith_error;
	extern scal	remainder;
};

#define nx_plus_y(N,X,Y)	mult_and_add(N,X,Y,07777777777)
#define mult_integers(N,X)	mult_and_add(N,X,0,017777777777)


#define INF_BAD	10000 

