
/*
 * $Id: fileio.h,v 1.8 2002/04/18 09:50:43 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include <string>

extern	int	name_length;
extern	str	name_of_file;

extern	str	cur_name;
extern	str	cur_area;
extern	str	cur_ext;

extern	int	area_delimiter;
extern	int	ext_delimiter;
extern	int	path_delimiter;

extern	bool	name_in_progress;

extern	str	job_name;
extern	str	job_area;

extern	bool	log_opened;

#define null_file ((FILE *) 0)

#define a_close(FP)		{fflush(FP);fclose(FP);}
#define b_close(FP)		{fflush(FP);fclose(FP);}

#define prompt_input(S)		{print(S); term_input();}

#define term_in			stdin
#define term_out		   stderr

extern	file	read_file[];
extern	int	read_open[];

#define OPENED			1
#define CLOSED			2

#define READ_ACCESS		4
#define WRITE_ACCESS 		2

extern	char	*input_path;
extern	char	*format_path;
extern	char	*font_path;


#define default_font_path	":/usr/local/lib/tex/fonts/tfm"
#define default_input_path 	":/usr/local/lib/tex/inputs"

#define NO_FILE_PATH 		0
#define INPUT_FILE_PATH 	1
#define FONT_FILE_PATH 		2

extern	str	str_dvi;
extern	str	str_tex;
extern	str	str_log;
extern	str	str_tfm;
extern	str	str_texput;

void  begin_name();
bool  more_name(int c);
void  end_name();
void	scan_file_name();
void	pack_file_name(str n, str a, str e);
void	pack_job_name(str s);
void	print_file_name(str n, str a, str e);
str	make_name_str();
void	prompt_job_name();
void	start_input(const std::string&);
void	start_input_prepare(const std::string&);
void	start_input_do();
FILE	*a_open_in();
FILE	*a_open_out();
FILE	*b_open_in();
FILE	*b_open_out();
bool	input_ln(file f);
void	term_input();
void	open_log_file();
bool	test_access(int amode, int file_path);
void	set_paths();
int   get_real_name();
void	_fileio_init();
void	_fileio_init_once();
