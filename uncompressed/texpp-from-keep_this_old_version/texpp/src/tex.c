
/*
 * $Id: tex.c,v 1.24 2002/08/26 15:16:02 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include <exception>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include "tex.h"


char	*banner = "TeX++ 0.1 $Date: 2002/08/26 15:16:02 $";

bool compatible=true;  // fallback mode to disable fiddling with default parameters

bool display_debug=false;      // debugging output, only for things you never want to see
bool display_progress=false;   // display progress information (page counters and so on)
bool display_warnings=false;   // display all warnings (overfull boxes mainly)
bool display_messages=false;   // display all other messages (\message, \show*)

enum preload_t { pr_plain, pr_latex, pr_none } preload=pr_plain;

int main(int argc, str argv[])
	{
	static struct option long_options[] = {
		{"hsize",         1, 0,  1},
		{"vsize",         1, 0,  2},
		{"hoffset",       1, 0,  3},
		{"voffset",       1, 0,  4},
		{"offset",        1, 0,  5},
		{"margin",        1, 0,  6},
		{"warnings",      0, 0, 13},
		{"progress",      0, 0, 15},
		{"debug",         0, 0,  8},
		{"compatible",    0, 0,  9},
		{"magnification", 1, 0, 10},
		{"plain",         0, 0, 11},
		{"latex",         0, 0, 12},
		{"initex",        0, 0, 14},
		{"help",          0, 0, 'h'},
		{"version",       0, 0, 'v'},
		{0, 0, 0, 0}
	};
	
	mag_set = 1000;
	signal(SIGINT, handle_int); 
	set_paths();

	if(strcmp(argv[0],"texpp")==0)          preload=pr_plain;
	else if(strcmp(argv[0],"latexpp")==0)   preload=pr_latex;

	while(1) {
		int option_index = 0;
		int c = getopt_long(argc, argv, "hv", long_options, &option_index);
		if (c == EOF)
			break;

		switch (c) {
			case 1:
				override_internal[0].value=atol(optarg)*65536;
				compatible=false;
				break;
			case 2:
				override_internal[1].value=atol(optarg)*65536;
				compatible=false;
				break;
			case 3:
				override_internal[2].value=atol(optarg)*65536;
				compatible=false;
				break;
			case 4:
				override_internal[3].value=atol(optarg)*65536;
				compatible=false;
				break;
			case 5:
				override_internal[2].value=atol(optarg)*65536;
				override_internal[3].value=atol(optarg)*65536;
				compatible=false;
				break;
			case 6:
				// hoffset/voffset start at -1 inch.
				override_internal[2].value=(int)((atol(optarg)-72.27)*65536);  // Magnification problems.
				override_internal[3].value=(int)((atol(optarg)-72.27)*65536);
				// the others are more sane.
				override[7].value=atol(optarg);
				override[8].value=atol(optarg);
				override[9].value=(atol(optarg)-55)*65536;
				compatible=false;
				break;
			case 8:
				display_debug=true;
				break;
			case 9:
				compatible=true;
				break;
			case 10:
				mag_set=atol(optarg);
				compatible=false;
				break;
			case 11:
				preload=pr_plain;
				break;
			case 12:
				preload=pr_latex;
				break;
			case 13:
				display_warnings=true;
				break;
			case 15: 
				display_progress=true;
				break;
			case 14:
				preload=pr_none;
				break;
			case 'v': {
				std::cerr << banner << std::endl
							 << "Copyright (C) 1987-1991 Pat Monardo" << std::endl
							 << "Copyright (C) 2002      Kasper Peeters" << std::endl;
				exit(1);
				}
			case 'h': {
			   std::cerr << "Usage: texpp [options]" << std::endl << std::endl;
				unsigned int i=0;
				while(long_options[i].name!=0) {
					std::cerr << "  --" <<  long_options[i].name;
					if(long_options[i].has_arg!=0) {
						std::cerr << "=[dimension in pt]";
						}
					std::cerr << std::endl;
					++i;
					}
				exit(1);
				}
			default:
				std::cerr << "texpp: option handling internal error" << std::endl;
				exit(2);
			}
		}

	initialize_once();
	initialize();
	if(!compatible && mag_set>0)
		reg_gdefine(int_reg[MAG_CODE], INT_REG, mag_set);

	try {
		bool from_file=decode_args(argc,optind,argv);
		if (end_line_char_active)
			*++limit = end_line_char;
		if (interaction == BATCH_MODE)
			selector = NO_PRINT;
		else 
			selector = TERM_ONLY;
		fix_date_and_time();
		if(from_file) start_input_prepare("");
		else          start_input_prepare("stdin");
		if(display_progress)
			std::cerr << "main input stream open..." << std::endl;
		if(preload==pr_plain) {
			if(display_progress) 
				std::cerr << "reading plain.tex... " << std::endl;
			start_input("plain.tex");
			}
		else if(preload==pr_latex) {
			if(display_progress)
				std::cerr << "reading latex.ltx... " << std::endl;
			start_input("latex.ltx");
			}
		start_input_do();
		history = SPOTLESS;
		main_control();
		}
	catch(std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		}
	final_cleanup();
	if(history==SPOTLESS) return 0;
	else return history;
	}

//ptr& mcell::link()
//	  {
//	  return h1.p;
//	  }

bool decode_args(int argc, int offset, str argv[])
	{
	int	i, j;
	
	if(argc-offset==0) {
		return FALSE;
		}
	// There should be at most one argument left, which is
	// interpreted as a filename.
	for(i=offset; i < argc; i++) {
		j = str_length(argv[i]);
		argv[i][j] = ' ';
		if (limit + j + 1 >= buffer + BUF_SIZE) {
			fprintf(stderr, "out of buffer space\n");
			return FALSE;
			}
		strncpy((char *)(limit + 1), argv[i], j + 1);
		limit += j + 1;
		argv[i][j] = '\0';
		}
	return TRUE;
	}

void fix_date_and_time()
	{
	long	clock, begintime();
	struct tm	*tmptr;
	
	clock = begintime();
	tmptr = localtime(&clock);
//	time = 60 * tmptr->tm_hour + tmptr->tm_min;
	day = tmptr->tm_mday;
	month = tmptr->tm_mon + 1;
	year = tmptr->tm_year + 1900;
	}

#undef time
long begintime()
	{
	return (time(0));
	}

void handle_int(int num)
	{	
	signal(SIGINT, handle_int);
	interrupted = 1;
	}

void initialize ()
	{
	_chr_init();
	_error_init();
	_sym_init();
	_def_init();
	_scan_init();
	_expand_init();
	_fileio_init();
	_cond_init();
	_eval_init();
	_box_init();
	_boxlist_init();
	_math_init();
	_mathlist_init();
	_hyph_init();
	_pack_init();
	_page_init();
	_par_init();
	_mlst_hlst_init();
	_align_init();
	_dvi_init();
	_cmd_init();
	_tfm_init();
	_print_init();
	_tok_init();
	}

void initialize_once()
	{
	_print_init_once();
	_error_init_once();
	_alloc_init_once();
	_str_init_once();
	_sym_init_once();
	_tok_init_once();
	_fileio_init_once();
	_eval_init_once();
	_box_init_once();
	_boxlist_init_once();
	_math_init_once();
	_mathlist_init_once();
	_hyph_init_once();
	_pack_init_once();
	_page_init_once();
	_par_init_once();
	_mlst_hlst_init_once();
	_align_init_once();
	_dvi_init_once();
	_cmd_init_once();
	_tfm_init_once();
	}

void final_cleanup()
	{
	while(open_parens > 0) {
		print(" )");
		decr(open_parens);
		}
	if(cur_level > LEVEL_ONE) {
		std::cerr << "(\\end occurred inside a group at level " 
					 << cur_level - LEVEL_ONE << ")" << std::endl;
		}
	while(cond_ptr != null) {
		std::cerr << "(\\end occurred when ";
		print_cmd_chr(IF_TEST, cur_if);
		if (if_line != 0) {
			print(" on line ");
			print_int(if_line);
			}
		print(" was incomplete)");
		if_line = if_line_field(cond_ptr); 
		cur_if = subtype(cond_ptr);
		cond_ptr = link(cond_ptr);
		}
	close_files();
	}

void close_files()
	{
	int	k;
	
	for (k = 0; k < 16; incr(k))
		if (write_open[k])
			a_close(write_file[k]);
	fin_dvi();

	std::cout << std::flush;
	std::cerr << std::flush;
	}

bool one(str s, str t)
	{	
	int	i;
	int	j;
	int	one;

	one = 0;
	j = strlen(t) - 1;
	for (i = 0; i < j; incr(i))
	  if (strncmp(s, &t[i], 2) == 0)
		 if (++one > 1)
			return FALSE;
	if (one == 0)
	  return FALSE;
	return TRUE;
	}

