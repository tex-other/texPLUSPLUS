
/*
 * $Id: print.h,v 1.5 2002/04/18 09:50:43 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

extern	file	log_file;
extern	str	log_name;

extern	int	selector;

#define NO_PRINT	16 
#define TERM_ONLY	17
#define LOG_ONLY	18
#define TERM_AND_LOG	19
#define PSEUDO		20
#define NEW_STRING	21
#define MAX_SELECTOR	21

extern	int	term_offset;
extern	int	file_offset;

extern	int	tally;
extern	int	first_count;
extern	int	trick_count;
extern	int	trick_buf[];

#define HALF_ERROR_LINE		32
#define ERROR_LINE		64
#define MAX_PRINT_LINE		72

extern	int	dig[];

#define wterm(c)	fputc(c,stderr);
#define wterm_ln(c)	{fputc(c,stderr); fputc('\n',stderr);}
#define wterm_cr()	fputc('\n',stderr);
/*
#define wlog(c)		putc(c, log_file)
#define wlog_ln(c)	{putc(c, log_file); putc('\n', log_file);}
#define wlog_cr()	putc('\n', log_file);
*/
#define wlog(c)		{}
#define wlog_ln(c)	{}
#define wlog_cr()	   {}
#define wfile(c)	putc(c, write_file[selector])
#define wfile_cr()	putc('\n', write_file[selector]);

int	begin_pseudoprint();
void	set_trick_count();
void	print(const char *);
void	print_char(int);
void	print_ASCII(int);
void	print_ln();
void	print_esc(str);
void	print_nl(str);
void	print_the_digs(int);
void	print_int(int);
void	print_two(int);
void	print_hex(int);
void	print_roman_int(int);
void	_print_init();
void	_print_init_once();
