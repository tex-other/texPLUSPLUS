
/*
 * $Id: error.h,v 1.9 2002/04/19 11:21:10 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include <string>
#undef time
#include <iostream>
#include <sstream>

class tex_error {
	public:
		tex_error();

		enum grade_t { progress, warning, error } grade;

		int line_start;
		int line_end;

		std::stringstream msg;
};

std::ostream& operator<<(std::ostream& str, const tex_error& err);

extern	int	interaction;

#define BATCH_MODE		0
#define NONSTOP_MODE		1
#define SCROLL_MODE		2
#define ERROR_STOP_MODE		3

extern	bool	deletions_allowed;
extern	int	error_cnt;

#ifdef NOHELP 
#define help0()
#define help1(h0)
#define help2(h0, h1)
#define help3(h0, h1, h2)
#define help4(h0, h1, h2, h3)
#define help5(h0, h1, h2, h3, h4) 
#define help6(h0, h1, h2, h3, h4, h5)
#else
#define help0() \
	{help_cnt = 0;}

#define help1(h0) \
	{help_cnt = 1; help_line[0] = h0;}

#define help2(h0, h1) \
	{help_cnt = 2; \
	help_line[0] = h0; help_line[1] = h1;}

#define help3(h0, h1, h2) \
	{help_cnt = 3; help_line[0] = h0; \
	help_line[1] = h1; help_line[2] = h2;}

#define help4(h0, h1, h2, h3) \
	{help_cnt = 4; \
	help_line[0] = h0; help_line[1] = h1; \
	help_line[2] = h2; help_line[3] = h3;}

#define help5(h0, h1, h2, h3, h4) \
	{help_cnt = 5; help_line[0] = h0; \
	help_line[1] = h1; help_line[2] = h2; \
	help_line[3] = h3; help_line[4] = h4;}

#define help6(h0, h1, h2, h3, h4, h5) \
	{help_cnt = 6; \
	help_line[0] = h0; help_line[1] = h1; \
	help_line[2] = h2; help_line[3] = h3; \
	help_line[4] = h4; help_line[5] = h5;}
#endif

extern	str	help_line[];
extern	int	help_cnt;
extern	bool	use_err_help;

extern	int	history;

#define SPOTLESS		0
#define WARNING_ISSUED		1
#define ERROR_MESSAGE_ISSUED	2
#define FATAL_ERROR_STOP	3

extern	bool	interrupted;
extern	bool	OK_to_interrupt;

void	begin_diagnostic();
void	end_diagnostic(bool );
void	print_err(str msg);
void	error();
void	normalize_selector();
void	int_error(int i);
void	fatal_error(str s);
void	overflow(const char *s, int n);
void	confusion(str s);
void	succumb();
void	_error_init();
void	_error_init_once();
