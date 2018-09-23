
/*
 * $Id: error.c,v 1.10 2002/04/19 11:21:10 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <stdlib.h>
#include "tex.h"

int	interaction;
int	history;
int	error_cnt;

bool	interrupted;
bool	OK_to_interrupt;
bool	deletions_allowed;
bool	use_err_help;

str	help_line[6];
int	help_cnt;
int	old_setting;

tex_error::tex_error()
	: grade(warning), line_start(0), line_end(0)
	{
	}

std::ostream& operator<<(std::ostream& str, const tex_error& err)
	{
	if((err.grade==tex_error::warning && display_warnings) || 
		(err.grade==tex_error::error) || 
		(err.grade==tex_error::progress && display_progress)) {
		if(err.line_start) {
			if(err.line_end)
				str << std::setw(5) << err.line_start << "-" << std::setw(5) << err.line_end;
			else 
				str << "      " << std::setw(5) << err.line_start;
			str << ": " << err.msg.str() << std::endl;
			}
		}
	return str;
	}

void begin_diagnostic ()
	{
	old_setting = selector;
	if (tracing_online <= 0 && selector == TERM_AND_LOG) {
		decr(selector);
		if (history == SPOTLESS)
			history = WARNING_ISSUED;
		}
	}

void end_diagnostic(bool blank_line)
	{
	print_nl(null_str);
	if (blank_line)
		print_ln();
	selector = old_setting;
	fflush(stdout);
	}

/* FIXME: depreciate */
void print_err(str msg)
	{
	print_nl("! ");
	print(msg);
	}

void error()
	{
	history = ERROR_MESSAGE_ISSUED;
	print(".");
	show_context();
	// KP: error correction cut out completely
	clear_for_error_prompt();
	give_err_help();
	// KP: Can this go too? NO: it prints all those help() messages. cleanup.
//	  else {
//		  if (help_cnt != 0) {
//			  for (hx = 0; hx < help_cnt; incr(hx)) {
//				  print(help_line[hx]);
//				  print_ln();
//				  }
//			  help_help();
//			  }
//		  }
	throw std::logic_error("compilation error");
	}

void int_error(int i)
	{
	print(" (");
	print_int(i);
	print(")");
	error();
	}

void normalize_selector()
	{
	if (log_opened)
		selector = TERM_AND_LOG;
	else selector = TERM_ONLY;
	if (job_name == null_str)
		open_log_file();
	if (interaction == BATCH_MODE)
		decr(selector);
	}

void succumb()
	{
	if (interaction == ERROR_STOP_MODE)
	  interaction = SCROLL_MODE;
	if (log_opened)
	  error();
	history = FATAL_ERROR_STOP;
	throw std::logic_error("fatal error");
	}

void fatal_error(str s)
	{
	normalize_selector();
	print_err("Emergency stop");
	help1(s);
	succumb();
	}

void overflow(const char *s, int n)
	{
	normalize_selector();
	std::cerr << "TeX capacity exceeded, sorry [" 
				 << s << "=" << n << "]" << std::endl;
	throw std::overflow_error("capacity exceeded");
	}

void confusion(str s)
	{
	normalize_selector();
	if (history < ERROR_MESSAGE_ISSUED) {
		print_err("This can't happen (");
		print(s);
		print(")");
//		help_broken();
		} 
	else {
		print_err("I can't go on meeting you like this");
//		help_wounded();
		}
	succumb();
	}

void _error_init ()
	{
	interrupted = FALSE;
	OK_to_interrupt = TRUE;
	deletions_allowed = TRUE;
	history = FATAL_ERROR_STOP;
	use_err_help = FALSE;
	error_cnt = 0;
	help_cnt = 0;
	}

void _error_init_once ()
	{
	interrupted = FALSE;
	OK_to_interrupt = TRUE;
	deletions_allowed = TRUE;
	history = FATAL_ERROR_STOP;
	interaction = ERROR_STOP_MODE;
	use_err_help = FALSE;
	error_cnt = 0;
	help_cnt = 0;
	}

