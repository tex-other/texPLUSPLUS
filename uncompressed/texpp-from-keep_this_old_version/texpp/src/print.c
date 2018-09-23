
/*
 * $Id: print.c,v 1.8 2002/04/19 11:21:10 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include <cstdlib>
#include <iostream>
#include "tex.h"

file	log_file;
str	log_name;

int	selector;
int	term_offset;
int	file_offset;
int	tally;
int	first_count;
int	trick_count;
int	trick_buf[ERROR_LINE];
int	dig[23];

void print_ln()
	{
	switch (selector) {
		case TERM_AND_LOG:
			if(display_progress) {
				wterm_cr();
				term_offset = 0;
				wlog_cr();
				file_offset = 0;
				}
			break;
		case LOG_ONLY:
			if(display_progress) {
				wlog_cr();
				file_offset = 0;
				}
			break;
		case TERM_ONLY:
			if(display_progress) {
				wterm_cr();
				term_offset = 0;
				}
			break;
		case NO_PRINT:
		case PSEUDO:
		case NEW_STRING:
			break;
			
		default:
			wfile_cr();
			break;
		}
	}	


void print_char(int c)
	{
	if (c == new_line_char && selector < PSEUDO) {
		print_ln();
		return;
		}
	switch (selector) {
		case TERM_AND_LOG:
			if(display_progress) {
				wterm(xchr[c]);
				incr(term_offset);
				wlog(xchr[c]);
				incr(file_offset);
				if (term_offset == MAX_PRINT_LINE)
					print_ln();
				if (file_offset == MAX_PRINT_LINE)
					print_ln();
				}
			break;
		case LOG_ONLY:
			if(display_progress) {
				wlog(xchr[c]);
				incr(file_offset);
				if (file_offset == MAX_PRINT_LINE)
					print_ln();
				}
			break;
		case TERM_ONLY:
			if(display_progress) {
				wterm(xchr[c]);
				incr(term_offset);
				if (term_offset == MAX_PRINT_LINE)
					print_ln();
				}
			break;
		case NO_PRINT:
			return;
		case PSEUDO:
			if (tally < trick_count) 
				trick_buf[tally % ERROR_LINE] = c;
			break;
		case NEW_STRING:
			if (cur_str_ptr < cur_str_end)
				append_char(c);
			break;
			
		default:
			wfile(xchr[c]);
			break;
		}
	incr(tally);
	}

void print_ASCII(int c)
	{
	if (c == new_line_char && selector < PSEUDO) {
		print_ln();
		return;
		}
	print(ASCII[c]);
	}

void print(const char *s)
	{
	while (*s)
		print_char(*s++);
	}

void print_nl(str s)
	{
	if (term_offset > 0 && odd(selector)
		 || file_offset > 0 && selector >= LOG_ONLY)
	  print_ln();
	print(s);
	}

void print_esc(str s)
	{
	int	c;
	
	c = escape_char;
	if (c >= 0 && c < 256) 
	  print_ASCII(c);
	print(s);
	}

void print_int(int n)
	{
	int	m;
	int	k;

	k = 0;
	if (n < 0)  {
		print("-");
		if (n > -100000000) {
			negate(n);
		} else {
			m = -1 - n;
			n = m / 10;
			m = m % 10 + 1;
			k = 1;
			if (m < 10) {
				dig[0] = m;
			} else {
				dig[0] = 0;
				incr(n);
			}
		}
	}
	do {
		dig[k] = n % 10;
		n /= 10;
		incr(k);
	} while (n != 0);
	print_the_digs(k);
}

void print_hex(int n)
	{
	int	k;

	k = 0;
	print("\"");
	do {
	  dig[k] = n % 16;
	  n /= 16;
	  incr(k);
	} while (n != 0);
	print_the_digs(k);
	}

void print_the_digs(int k)
	{
	while (k > 0) {
		decr(k);
		if (dig[k] < 10)
			print_char('0' + dig[k]);
		else print_char('A' - 10 + dig[k]);
		}
	}

void print_two(int n)
	{
	n = abs(n) % 100;
	print_char('0' + n / 10);
	print_char('0' + n % 10);
	}

void print_roman_int(int n)
	{
	str 	s;
	str 	t;
	int	u;
	int	v;

	s = "m2d5c2l5x2v5i";
	v = 1000;
	loop {
		while (n >= v) {
			print_char(*s);
			n -= v;
		}
		if (n <= 0)
			return;
		t = s + 2;
		u = v / (t[-1] - '0');
		if (t[-1] == '2')  {
			t += 2;
			u /= t[-1] - '0';
		}
		if (n + u >= v)  {
			print_char(*t);
			n += u;
		} else {
			s += 2;
			v /= s[-1] - '0';
		}
	}
}

int begin_pseudoprint()
	{
	int 	t;
	
	t = tally;
	tally = 0;
	selector = PSEUDO;
	trick_count = 1000000;

	return t;
	}

void set_trick_count()
	{
	first_count = tally;
	trick_count = tally + 1 + ERROR_LINE - HALF_ERROR_LINE;
	if (trick_count < ERROR_LINE) 
	  trick_count = ERROR_LINE;
	}

void _print_init()
	{
	selector = TERM_ONLY;
	tally = 0;
	term_offset = 0;
	file_offset = 0;
	if(display_progress) {
		std::cerr << banner << std::endl;
		}
	log_name = null_str;
	}

void _print_init_once()
	{
	selector = TERM_ONLY;
	tally = 0;
	term_offset = 0;
	file_offset = 0;
	log_name = null_str;
	}
