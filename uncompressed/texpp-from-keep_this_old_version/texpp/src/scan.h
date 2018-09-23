
/*
 * $Id: scan.h,v 1.4 2002/04/18 09:50:43 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

extern	int	cur_val_level;
extern	int	cur_val;

#define INT_VAL		0
#define DIMEN_VAL	1
#define GLUE_VAL	2
#define MU_VAL		3
#define IDENT_VAL	4
#define TOK_VAL		5

#define INPUT_LINE_NO_CODE	3
#define BADNESS_CODE		4

extern	int	radix;

#define PLUS_TOKEN		(OTHER_TOKEN + '+')
#define MINUS_TOKEN		(OTHER_TOKEN + '-')
#define ZERO_TOKEN		(OTHER_TOKEN + '0')
#define A_TOKEN			(LETTER_TOKEN + 'A')
#define OTHER_A_TOKEN		(OTHER_TOKEN + 'A')
#define OCTAL_TOKEN		(OTHER_TOKEN + '\'')
#define HEX_TOKEN		(OTHER_TOKEN + '"')
#define ALPHA_TOKEN		(OTHER_TOKEN + '`')
#define POINT_TOKEN		(OTHER_TOKEN + '.')
#define EURO_POINT_TOKEN	(OTHER_TOKEN + ',')

#define MAX_DIMEN	07777777777

#define scan_normal_dimen()	scan_dimen(FALSE, FALSE, FALSE)

extern	int	cur_order;

#define scan_optional_space() \
	{get_x_token(); if (cur_cmd != SPACER) back_input();}

#define get_nbx_token() \
	{do get_x_token(); while (cur_cmd == SPACER);}

#define get_nbrx_token() \
	{do get_x_token(); while (cur_cmd == SPACER || cur_cmd == RELAX);}

void	scan_left_brace();
void	scan_optional_equals();
bool	scan_keyword(str);
void	scan_something_internal(int, bool);
void	mu_error();
void	scan_eight_bit_int();
void	scan_char_num();
void	scan_four_bit_int();
void	scan_fifteen_bit_int();
void	scan_twenty_seven_bit_int();
void	scan_int();
void	scan_dimen(bool mu, bool inf, bool shortcut);
void	scan_glue(int level);
void	scan_spec(int c, bool three_codes);
ptr	scan_rule_spec();
void  help_mathchar();
void  help_number();
void  help_char_num();
void  help_char_const();
void  help_big_num();
void  help_missing_number();
void  help_filll();
void  help_mu();
void  help_mu_error();
void  help_dimen();
void  help_big_dimen();
void  help_thee();
void  help_left();
void  help_aux();
void  help_del();
void  help_reg();
void	_scan_init();
void	_scan_init_once();
