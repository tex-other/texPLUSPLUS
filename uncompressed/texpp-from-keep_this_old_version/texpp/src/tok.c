
/*
 * $Id: tok.c,v 1.10 2002/04/19 11:21:10 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include <stdexcept>
#include "tex.h"

tok	cur_tok;
tok	par_tok;

int	cur_cmd;
int	cur_chr;

ptr	def_ref;
ptr	match_toks;
ptr	align_toks;
ptr	omit_template;
ptr	null_list;

int	scanner_status;
int	align_state;

str	name;
int	line;
bool	force_eof;
int	open_parens;

input	cur_input;

int	ninputs;
input	*input_stack;
input	*input_end;
input 	*input_ptr;
input 	*max_in_stack;
input	*base_ptr;

#define STACK_SIZE	200

int	nfiles;
infile	*file_stack;
infile	*file_end;
infile	*file_ptr;
infile	*max_file_stack;

#define INFILE_SIZE	45

int	nparams;
ptr	*param_stack;
ptr	*param_end;
ptr	*param_ptr;
ptr	*max_param_stack;

#define PARAM_SIZE	100

void get_token ()
	{
	no_new_control_sequence = FALSE;
	get_next();
	no_new_control_sequence = TRUE;
	if (cur_cs == null_sym) {
		cur_tok = cur_cmd * 0400 + cur_chr;
		} 
	else {
		cur_tok = sym2tok(cur_cs);
		}
	}

#define any_state(CAT) \
	case MID_LINE + CAT: \
	case SKIP_BLANKS + CAT: \
	case NEW_LINE + CAT

#define delims(STATE) \
	case STATE + MATH_SHIFT: \
	case STATE + TAB_MARK: \
	case STATE + MAC_PARAM: \
	case STATE + SUB_MARK: \
	case STATE + LETTER: \
	case STATE + OTHER_CHAR 

#define mid_line(CAT) \
	case MID_LINE + CAT

#define new_line(CAT) \
	case NEW_LINE + CAT

#define skip_blanks(CAT) \
	case SKIP_BLANKS + CAT

#define is_hex(C) \
	((C) >= '0' && (C) <= '9' || (C) >= 'a' && (C) <= 'f')

#define hex_to_cur_chr(C, CC) \
{	cur_chr = 16 * ((C <= '9') ? C - '0' : C - 'a' + 10); \
	cur_chr += (CC <= '9') ? CC - '0' : CC - 'a' + 10; \
}

void get_next ()
	{
	tok	t;
	int	c, cc;
	
#define JOB_ABORT "*** (job aborted, no legal \\end found)"
#define USER_CMD "(Please type a command or say `\\end')"
#define INVAL_CHAR "Text line contains an invalid character"
#define WOVEN_ALIGNS "(interwoven alignment preambles are not allowed)"

	restart:
	cur_cs = null_sym;
	if (file_state) {
	   reread:
		if (next <= limit) {
			cur_chr = *next++;
		reswitch:
			cur_cmd = cat_code(cur_chr);
			switch (cur_input.state_field + cur_cmd) {
				any_state(IGNORE):
					skip_blanks(SPACER):
					new_line(SPACER):
					goto reread;
				
				any_state(ESCAPE):
					get_cs();
				   cur_cmd = eq_type(cur_cs);
				   cur_chr = equiv(cur_cs);
				   if (cur_cmd >= OUTER_CALL)
					   check_outer_validity();
					break;
				
				any_state(ACTIVE_CHAR):
					cur_cs = active_base[cur_chr];
				   cur_cmd = eq_type(cur_cs);
					cur_chr = equiv(cur_cs);
					cur_input.state_field = MID_LINE;
					if (cur_cmd >= OUTER_CALL)
						check_outer_validity();
					break;
					
				any_state(SUP_MARK):
					if (cur_chr != *next
						 || next >= limit
						 || (c = next[1]) >= 0200) {
						cur_input.state_field = MID_LINE;
						break;
						}
				next += 2;
				if (is_hex(c) && next <= limit) {
					cc = *next;
					if (is_hex(cc)) {
						next++;
						hex_to_cur_chr(c, cc);
						goto reswitch;
					}
				}
				if (c < 0100) {
					cur_chr = c + 0100;
				} else {
					cur_chr = c - 0100;
				}
				goto reswitch;

			any_state(INVALID_CHAR):
				print_err(INVAL_CHAR);
				help_funny();
				deletions_allowed = FALSE;
				error();
				deletions_allowed = TRUE;
				goto restart;
			
			mid_line(SPACER):
				cur_input.state_field = SKIP_BLANKS;
				cur_chr = ' ';
				break;
			
			mid_line(CAR_RET):
				next = limit + 1;
				cur_cmd = SPACER;
				cur_chr = ' ';
				break;
			
			skip_blanks(CAR_RET):
			any_state(COMMENT):
				next = limit + 1;
				goto reread;

			new_line(CAR_RET):
				next = limit + 1;
				cur_cs = par_cs;
				cur_cmd = eq_type(cur_cs);
				cur_chr = equiv(cur_cs);
				if (cur_cmd >= OUTER_CALL)
					check_outer_validity();
				break;
			
			mid_line(LEFT_BRACE):
				incr(align_state);
				break;

			skip_blanks(LEFT_BRACE):
			new_line(LEFT_BRACE):
				cur_input.state_field = MID_LINE;
				incr(align_state);
				break;
			
			mid_line(RIGHT_BRACE):
				decr(align_state);
				break;
			
			skip_blanks(RIGHT_BRACE):
			new_line(RIGHT_BRACE):
				cur_input.state_field = MID_LINE;
				decr(align_state);
				break;
			
			delims(SKIP_BLANKS):
			delims(NEW_LINE):
				cur_input.state_field = MID_LINE;
				break;

			default:
				break;
			}
			} 
		else {
			cur_input.state_field = NEW_LINE; 
			if (index > 17) {
				incr(line);
				if (!force_eof) {
					if(!input_ln(cur_file)) {
						force_eof = TRUE;
						}
					}
				if (force_eof) {
					print(")");
					decr(open_parens);
					force_eof = FALSE;
					end_file_reading();
					check_outer_validity();
					goto restart;
					}
				if (end_line_char_active)
					*++limit = end_line_char;
				} 
			else {
				if (!terminal_input) {
					cur_cmd = 0;
					cur_chr = 0;
					return;
					}
				if (input_ptr > input_stack) {
					end_file_reading();
					goto restart;
					}
				if (selector < LOG_ONLY)
					open_log_file();
				if (interaction > NONSTOP_MODE) {
					if (limit <= buffer)
						print_nl(USER_CMD);
					print_ln();
					prompt_input("*");
					memcpy(buffer, cur_str, cur_length());
					next = buffer;
					limit = buffer + cur_length() - 1;
					if (end_line_char_active)
						*++limit = end_line_char;
					flush_str();
					} 
				else {
					fatal_error(JOB_ABORT);
					}
				}
			if(interrupted) throw std::logic_error("interrupted");
			goto reread;
			}
		} 
	else {
		if (loc != null) {
			t = token(loc);
			loc = token_link(loc);
			if (t >= CS_TOKEN_FLAG) {
				cur_cs = tok2sym(t);
				cur_cmd = eq_type(cur_cs);
				cur_chr = equiv(cur_cs);
				if (cur_cmd >= OUTER_CALL) {
					if (cur_cmd == DONT_EXPAND) {
						cur_cs = tok2sym(token(loc));
						loc = null;
						cur_cmd = eq_type(cur_cs);
						cur_chr = equiv(cur_cs);
						if (cur_cmd > MAX_COMMAND) {
							cur_cmd = RELAX;
							cur_chr = NO_EXPAND_FLAG;
						}
					} else {
						check_outer_validity();
					}
				}
			} else {
				cur_cmd = t / 0400;
				cur_chr = t % 0400;
				switch (cur_cmd)
				{
				case LEFT_BRACE:
					incr(align_state);
					break;

				case RIGHT_BRACE:
					decr(align_state);
					break;

				case OUT_PARAM:
					begin_token_list(
						param_start[cur_chr - 1],
						PARAMETER
					);
					goto restart;

				default:
					break;
				}
			}
		} else {
			end_token_list();
			goto restart;
		}
	}
	if (cur_cmd <= CAR_RET && cur_cmd >= TAB_MARK && align_state == 0) {
		if (scanner_status == ALIGNING)
			fatal_error(WOVEN_ALIGNS);
		cur_cmd = unset_info(cur_align);
		unset_info(cur_align) = cur_chr;
		if (cur_cmd == OMIT)
			begin_token_list(omit_template, V_TEMPLATE);
		else begin_token_list(v_part(cur_align), V_TEMPLATE);
		align_state = 1000000;
		goto restart;
	}
}

void get_cs ()
	{
	byte	*nx;
	int	cat=0;
	int	d, c, cc=0;
	
	if (next > limit) {
		cur_cs = null_cs;
		return;
		}
	
#define reduce_expanded_cc(CC) 						\
{	if (*CC == cur_chr && cat == SUP_MARK && CC < limit) { 		\
		if ((c = CC[1]) < 0200) {				\
			d = 2;						\
			if (is_hex(c) && CC + 2 <= limit) {		\
				cc = CC[2];				\
				if (is_hex(cc))				\
					incr(d);			\
			}						\
			if (d > 2) {					\
				hex_to_cur_chr(c, cc);			\
				CC[-1] = cur_chr;			\
			} else if (c < 0100) { 				\
				CC[-1] = c + 0100; 			\
			} else {					\
				CC[-1] = c - 0100; 			\
			}						\
			limit -= d;					\
			while (CC <= limit) { 				\
				CC[0] = CC[d];				\
				incr(CC);				\
			}						\
			goto start_cs;					\
		}							\
	}								\
}

start_cs: 
	nx = next;
	cur_chr = *nx++;
	cat = cat_code(cur_chr);
	if (cat == LETTER)
		cur_input.state_field = SKIP_BLANKS;
	else if (cat == SPACER)
		cur_input.state_field = SKIP_BLANKS;
	else cur_input.state_field = MID_LINE;
	if (cat == LETTER && nx <= limit) {
		do {
			cur_chr = *nx++;
			cat = cat_code(cur_chr);
		} while (cat == LETTER && nx <= limit);
		reduce_expanded_cc(nx);
		if (cat != LETTER)
			decr(nx);
		if (nx > next + 1) {
			cur_cs = id_lookup((char *)next, nx - next);
			next = nx;
			return;
		}
	} else {
		reduce_expanded_cc(nx);
	}
	cur_cs = single_base[*next++];
}

void
check_outer_validity ()
{
	ptr	p, q;

	if (scanner_status != NORMAL) {
		deletions_allowed = FALSE;
		if (cur_cs != null_sym) {
			if (cur_input.state_field == TOKEN_LIST || index < 1 || index > 17) {
				p = new_token();
				token(p) = sym2tok(cur_cs);
				back_list(p);
			}
			cur_cmd = SPACER;
			cur_chr = ' ';
		}
		if (scanner_status > SKIPPING) {
			runaway();
			if (cur_cs == null_sym) {
				print_err("File ended");
			} else {
				cur_cs = null_sym;
				print_err("Forbidden control sequence found");
			}
			print(" while scanning ");
			p = new_token();
			switch (scanner_status)
			{
			case DEFINING:
				print("definition");
				token(p) = RIGHT_BRACE_TOKEN + '}';
				break;

			case MATCHING:
				print("use");
				token(p) = par_tok;
				long_state = OUTER_CALL;
				break;

			case ALIGNING:
				print("preamble");
				token(p) = RIGHT_BRACE_TOKEN + '}';
				q = p;
				p = new_token();
				token_link(p) = q;
				token(p) = sym2tok(FROZEN_CR);
				align_state = -1000000;
				break;

			case ABSORBING:
				print("text"); 
				token(p) = RIGHT_BRACE_TOKEN + '}';
				break;
			}
			ins_list(p);
			print(" of ");
			sprint_cs(warning_cs);
			help_scanner();
			error();
		} else {
			print_err("Incomplete ");
			print_cmd_chr(IF_TEST, cur_if);
			print("; all text was ignored after line ");
			print_int(skip_line);
			if (cur_cs != null_sym) {
				cur_cs = null_sym;
				help_skip();
			} else {
				help_skif();
			}
			cur_tok = sym2tok(FROZEN_FI);
			ins_error();
		}
		deletions_allowed = TRUE;
	}
}


// Push current input info (file handle, point to buffer and so on) onto the stack.

void push_input()
	{
	if(input_ptr > max_in_stack) {
		if (input_ptr == input_end)
			overflow("input stack size", ninputs);
		max_in_stack = input_ptr;
		}
	if(file_state) {
		if (index == 0 || index > 17) {
			file_line = line;
			file_name = name;
			}
		} 
	else {
		cs_name = name;
		}
	*input_ptr++ = cur_input;
	}

void pop_input()
	{
	cur_input = *--input_ptr;
	if (file_state) {
		if (index == 0 || index > 17) {
			line = file_line;
			name = file_name;
			}
		} 
	else {
		name = cs_name;
		}
	}

void begin_token_list(ptr p, int t)
	{
	push_input();
	cur_input.state_field = TOKEN_LIST;
	start = p;
	token_type = t;
	if (t >= MACRO) {
		add_token_ref(p);
		if (t == MACRO) {
			param_start = param_ptr;
			} 
		else {
			loc = token_link(p);
			if (tracing_macros > 1) {
				begin_diagnostic();
				print_nl(null_str);
				switch (t) {
					case MARK_TEXT:
						print_esc("mark");
						break;
						
					case WRITE_TEXT:
						print_esc("write");
						break;
						
					default:
						print_toks_param(t - OUTPUT_TEXT);
						break;
					}
				print("->");
				token_show(p);
				end_diagnostic(FALSE);
				}
			}
		} 
	else {
		loc = p;
		}
	}

void end_token_list ()
	{
	if (token_type >= BACKED_UP) {
		if (token_type <= INSERTED) {
			flush_list(start);
			} 
		else {
			delete_token_ref(start);
			if (token_type == MACRO)
				while (param_ptr > param_start)
					flush_list(*--param_ptr);
			}
		} 
	else if (token_type == U_TEMPLATE) {
		align_state = 0;
		}
	pop_input();
	if(interrupted) throw std::logic_error("interrupted");
	}

// Push current input info onto the stack (see above) and allocate
// memory for a new file (alloc new buffer and set cur_input state).

void begin_file_reading ()
	{
	push_input();
	if (file_ptr > max_file_stack) {
		if (file_ptr == file_end)
			overflow("file stack size", nfiles);
		max_file_stack = file_ptr;
		}
	in_file = (ptr)file_ptr++;
	cur_input.obj_field.f.buf_field = (byte *)new_str(BUF_SIZE);
	cur_input.state_field = MID_LINE;
	index = 0;
	}

void end_file_reading ()
	{
	if(index>17) {
		a_close(cur_file);
		}
	free_str((char *)cur_input.obj_field.f.buf_field);
	pop_input();
	decr(file_ptr);
	}

void back_input ()
	{
	ptr	p;
	
	while (cur_input.state_field == TOKEN_LIST && loc == null)
		end_token_list();
	p = new_token();
	token(p) = cur_tok;
	if (cur_tok < RIGHT_BRACE_LIMIT) {
		if (cur_tok < LEFT_BRACE_LIMIT) {
			decr(align_state);
			} 
		else {
			incr(align_state);
			}
		}
	push_input();
	cur_input.state_field = TOKEN_LIST;
	loc = start = p;
	token_type = BACKED_UP;
	}

void back_error ()
	{
	OK_to_interrupt = FALSE;
	back_input();
	OK_to_interrupt = TRUE;
	error();
	}

void ins_error ()
	{
	OK_to_interrupt = FALSE;
	back_input();
	token_type = INSERTED; 
	OK_to_interrupt = TRUE;
	error();
	}

void clear_for_error_prompt()
	{
	while (cur_input.state_field != TOKEN_LIST
			 && terminal_input
			 && input_ptr > input_stack
			 && next > limit)
		end_file_reading();
	}

void runaway ()
	{
	ptr	p=0;

	if (scanner_status > SKIPPING) {
		print_nl("Runaway ");
		switch (scanner_status) 
		{
		case DEFINING:
			print("definition");
			p = def_ref;
			break;

		case MATCHING:
			print("argument");
			p = match_toks;
			break;

		case ALIGNING:
			print("preamble");
			p = align_toks;
			break;

		case ABSORBING:
			print("text");
			p = def_ref;
			break;
		}
		print("?");
		print_ln();
		show_token_list(token_link(p), null, ERROR_LINE - 10);
		}
	}

// Display the location of the error

void show_context()
	{
	int	i, j, l, m, n, nn, p, q;
	int	old_setting;
	bool	bottom_line;

	base_ptr = input_ptr;
	push_input();
	nn = -1;
	bottom_line = FALSE;
	loop {
		cur_input = *base_ptr;
		if (cur_input.state_field != TOKEN_LIST && 
			 (index > 17 || base_ptr == input_stack)) {
			bottom_line = TRUE;
			}
		if (base_ptr == input_ptr - 1 || bottom_line || nn < error_context_lines) {
			if (base_ptr == input_ptr - 1 || cur_input.state_field != TOKEN_LIST || token_type != BACKED_UP || loc != null) {
				tally = 0;
				old_setting = selector;
				if (file_state) {
					if (index <= 17) {
						if (terminal_input) {
							if (base_ptr == input_stack)
								print_nl("<*>");
							else print_nl("<insert> ");
							} 
						else {
							print_nl("<read ");
							if (index == 17)
								print("*");
							else print_int(index - 1);
							print(">");
							}
						} 
					else {
						print(name_of_file);
						print_nl(" l.");
						print_int(file_line);
						}
					print(" ");
					l = begin_pseudoprint();
					j = limit - buffer;
					if (*limit != end_line_char)
						incr(j);
					if (j > 0) {
						for (i = 0; i < j; incr(i)) {
							if (buffer + i == next)
								set_trick_count();
							print_ASCII(buffer[i]);
							}
						}
					} 
				else {
					switch (token_type) {
						case PARAMETER:
							print_nl("<argument> ");
							break;
							
						case U_TEMPLATE:
						case V_TEMPLATE:
							print_nl("<template> ");
							break;
							
						case BACKED_UP:
							if (loc == null)
								print_nl("<recently read> "); 
							else  print_nl("<to be read again> ");
							break;
							
						case INSERTED:
							print_nl("<inserted text> ");
							break;
							
						case MACRO:
							print_ln();
							print_cs((char *)in_cs);
							break;
							
						case OUTPUT_TEXT:
							print_nl("<output> ");
							break;
							
						case EVERY_PAR_TEXT:
							print_nl("<everypar> ");
							break;
							
						case EVERY_MATH_TEXT:
							print_nl("<everymath> ");
							break;
							
						case EVERY_DISPLAY_TEXT:
							print_nl("<everydisplay> ");
							break;
							
						case EVERY_HBOX_TEXT:
							print_nl("<everyhbox> ");
							break;
							
						case EVERY_VBOX_TEXT:
							print_nl("<everyvbox> ");
							break;
							
						case EVERY_JOB_TEXT:
							print_nl("<everyjob>");
							break;
							
						case EVERY_CR_TEXT:
							print_nl("<everycr> ");
							break;
							
						case MARK_TEXT:
							print_nl("<mark> ");
							break;
							
						case WRITE_TEXT:
							print_nl("<write> ");
							break;
							
						default:
							print_nl("? ");
							break;
						}
					l = begin_pseudoprint();
					if (token_type < MACRO)
						show_token_list(start, loc, 100000);
					else
						show_token_list(token_link(start),
											 loc, 100000);
					}
				selector = old_setting;
				if (trick_count == 1000000)
					set_trick_count();
				if (tally < trick_count)
					m = tally - first_count;
				else m = trick_count - first_count;
				if (l + first_count <= HALF_ERROR_LINE) {
					p = 0;
					n = l + first_count;
					} 
				else {
					print("...");
					p = l + first_count - HALF_ERROR_LINE + 3;
					n = HALF_ERROR_LINE;
					}
				for (q = p; q < first_count; incr(q))
					print_char(trick_buf[q % ERROR_LINE]);
				print_ln();
				for (q = 1; q <= n; incr(q))
					print(" ");
				if (m + n <= ERROR_LINE)
					p = first_count + m;
				else p = first_count + ERROR_LINE - n - 3;
				for (q = first_count; q < p; q++)
					print_char(trick_buf[q % ERROR_LINE]);
				if (m + n > ERROR_LINE)
					print("...");
				incr(nn);
				}
			} 
		else if (nn == error_context_lines) {
			print_nl("...");
			incr(nn);
			}
		if (bottom_line)
			break;
		decr(base_ptr);
		}
	pop_input();
	}

void _tok_init ()
	{
	base_ptr = max_in_stack = input_ptr = input_stack;
	max_file_stack = file_ptr = file_stack;
	max_param_stack = param_ptr = param_stack;
	cur_input.state_field = NEW_LINE;
	index = 0;
	in_file = (ptr)file_ptr++;
	cur_file = stdin;
	name = file_name = "tty";
	line = file_line =  0;
	force_eof = FALSE;
	open_parens = 0;
	scanner_status = NORMAL;
	align_state = 1000000;
	buffer = (byte *)new_str(BUF_SIZE);  // IS this the global buffer? Yes, one for every file.
	next = buffer;
	limit = next - 1;
	token(omit_template) = END_TEMPLATE_TOKEN;
	token(null_list) = null_tok;
	}

void _tok_init_once ()
	{
	// FIXME: (memory) these are all stacks that can be stack<input>
	ninputs = STACK_SIZE;
	input_stack = (input *)malloc(ninputs*sizeof(input));
	if (input_stack == (input *) 0)
		overflow("input stack", ninputs);
	input_end = input_stack + ninputs;
	
	nfiles = INFILE_SIZE;
	file_stack = (infile *)malloc(nfiles * sizeof(infile));
	if (file_stack == (infile *) 0)
		overflow("infile stack", nfiles);
	file_end = file_stack + nfiles;
	
	nparams = PARAM_SIZE;
	param_stack = (ptr *)malloc(nparams * sizeof(ptr));
	if (param_stack == (ptr *) 0)
		overflow("param stack", nparams);
	param_end = param_stack + nparams;
	match_toks = new_token();
	align_toks = new_token();
	omit_template = new_token();
	null_list = new_token();
	}

/*
**	Help text
*/

void help_scanner()
	{
	help4("I suspect you have forgotten a `}', causing me",
	"to read past where you wanted me to stop.",
	"I'll try to recover; but if the error is serious,",
	"you'd better type `E' or `X' now and fix your file.");
	}

void help_funny()
	{
	help2("A funny symbol that I can't read has just been input.",
	"Continue, and I'll forget that it ever happened.");
	}

void help_skip()
	{
	help3("A forbidden control sequence occurred in skipped text.",
	"This kind of error happens when you say `\\if...' and forget",
	"the matching `\\fi'. I've inserted a `\\fi'; this might work.");
	}

void help_skif()
	{
	help3("The file ended while I was skipping conditional text.",
	"This kind of error happens when you say `\\if...' and forget",
	"the matching `\\fi'. I've inserted a `\\fi'; this might work.");
	}
