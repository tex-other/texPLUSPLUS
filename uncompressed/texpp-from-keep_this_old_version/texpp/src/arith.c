
/*
 * Common TeX:arith.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#include "tex.h"

bool	tex::arith_error;
scal	tex::remainder;

int half(int x)
	{
	return (odd(x) ? (x + 1) / 2 : x / 2);
	}

scal round_decimals(int k)
	{
	int	a;

	a = 0;
	while (k > 0) {
		decr(k);
		a = (a + dig[k] * TWO) / 10;
		}
	return ((a + 1) / 2);
	}

std::string scal2string(scal s)
	{
	std::stringstream str;
	scal	delta;
	
	if (s < 0) {
		str << "-";
		s = -s;
		}
	str << (int)(s/UNITY) << ".";
	s = 10 * (s % UNITY) + 5;
	delta = 10;
	do {
		if (delta > UNITY)
			s += 0100000 - 50000;
		str << (char)('0' + s / UNITY);
		s = 10 * (s % UNITY);
		delta *= 10;
		} while (s > delta);

	return str.str();
	}

void print_scaled(scal s)
	{
	scal	delta;
	
	if (s < 0) {
		print("-");
		s = -s;
		}
	print_int(s / UNITY);
	print(".");
	s = 10 * (s % UNITY) + 5;
	delta = 10;
	do {
		if (delta > UNITY)
			s += 0100000 - 50000;
		print_char('0' + s / UNITY);
		s = 10 * (s % UNITY);
		delta *= 10;
		} while (s > delta);
	}

scal mult_and_add(int n, scal x, scal y, scal max_answer)
	{
	if (n < 0) {
		x = -x;
		n = -n;
		}
	if (n == 0)
		return y;
	if (x <= (max_answer - y) / n && -x <= (max_answer + y) / n)
		return (n * x + y);
	tex::arith_error = TRUE;
	return 0;
	}

scal x_over_n(scal x, int n)
	{
	bool	negative;
	scal	quotient;

	negative = FALSE;
	if (n == 0) {
		tex::arith_error = TRUE;
		tex::remainder = x;
		return 0;
	}
	if (n < 0) {
		x = -x;
		n = -n;
		negative = TRUE;
	}
	if (x >= 0) {
		quotient = x / n;
		tex::remainder = x % n;
	} else {
		quotient = -(-x / n);
		tex::remainder = -(-x % n);
	}
	if (negative)
		negate(tex::remainder);
	return quotient;
	}

scal xn_over_d(scal x, int n, int d)
	{
	int	t;
	int	u;
	int	v;
	bool	positive;

	if (x >= 0) {
		positive = TRUE;
	} else {
		x = -x;
		positive = FALSE;
	}
	t = x % 0100000 * n;
	u = x / 0100000 * n + t / 0100000;
	v = u % d * 0100000 + t % 0100000;
	if (u / d >= 0100000) {
		tex::arith_error = TRUE;
	} else {
		u = 0100000 * (u / d) + v / d;
	}
	if (positive) {
		tex::remainder = v % d;
		return u;
	} else {
		tex::remainder = - (v % d);
		return -u;
	}
	}

int badness(scal t, scal s)
	{
	int	r;

	if (t == 0) {
		return 0;
	} else if (s <= 0) {
		return INF_BAD;
	} else {
		if (t <= 7230584)
			r = t * 297 / s;
		else if (s >= 1663497)
			r = t / (s / 297);
		else
			r = t;
		if (r > 1290)
			return INF_BAD;
		else
			return ((r * r * r + 0400000) / 01000000);
	}
	}

