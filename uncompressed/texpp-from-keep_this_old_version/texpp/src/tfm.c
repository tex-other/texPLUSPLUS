
/*
 * $Id: tfm.c,v 1.2 2002/03/28 12:47:42 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

#include "tex.h"

file	tfm_file;

int	nfonts;
font_t	**font_info;
font_t	**font_end;
font_t	**font_ptr;

qcell	null_character;

#define abort() goto bad_tfm

#define start_font_error_message() \
{	print_err("Font "); sprint_cs(u); \
	print("="); print_file_name(nom, aire, null_str); \
	if (s >= 0) {print(" at "); print_scaled(s); print("pt");} \
	else if (s != -1000) {print(" scaled "); print_int(-s);} \
}

#define get_font_byte() \
	(font_byte = getc(tfm_file))

#define read_sixteen(X) \
{	get_font_byte(); X = font_byte; \
	if (X > 255) abort(); \
	get_font_byte(); X = (X) * 0400 + font_byte; \
}

#define store_four_quarters(Q) \
{	a = get_font_byte(); b = get_font_byte(); \
	c = get_font_byte(); d = get_font_byte(); \
	qw.b0 = a; qw.b1 = b; \
	qw.b2 = c; qw.b3 = d; \
	Q = qw; \
}

#define store_scaled(S) \
{	a = get_font_byte(); b = get_font_byte(); \
	c = get_font_byte(); d = get_font_byte(); \
	sw = ((((d * z) / 0400) + (c * z)) / 0400 + (b * z)) / beta; \
	if (a == 0) S = sw; \
	else if (a == 255) S = sw - alpha; \
	else abort(); \
}

#define check_byte_range(C) \
{	if (C < bc || C > ec) \
		abort(); \
}

#define check_existence(C) \
{	check_byte_range(C); \
	qw = char_info(f, C); \
	if (!char_exists(qw)) \
		abort(); \
}

fnt read_font_info(sym u, str nom, str aire, scal s)
	{
	int	a, b, c, d, k;
	fnt	f, g;
	int	bc, ec, lf, lh;
	int	nw, nh, nd, ni, nk, nl, ne, np;
	qcell	qw;
	scal	sw, z;
	qcell	*fp;
	qcell	*pq;
	scal	*ps;
	int	alpha;
	int	beta;
	int	bchar;
	int	bch_label;
	bool	file_opened;
	int	font_byte;
	qcell	dev_null;
/*
** 	Open tfm_file for input
*/
	g = null_font;
	file_opened = FALSE;
	pack_file_name(nom, aire, str_tfm);
	if ((tfm_file = b_open_in()) == null_file)
		abort();
	file_opened = TRUE;
/*
**	Read the TFM size fields
*/
	read_sixteen(lf);
	read_sixteen(lh);
	read_sixteen(bc);
	read_sixteen(ec);
	if (bc > ec + 1 || ec > 255)
		abort();
	if (bc > 255) {
		bc = 1;
		ec = 0;
	}
	read_sixteen(nw);
	read_sixteen(nh);
	read_sixteen(nd);
	read_sixteen(ni);
	read_sixteen(nl);
	read_sixteen(nk);
	read_sixteen(ne);
	read_sixteen(np);
	if (lf != 6 + lh + (ec - bc + 1) +
		nw + nh + nd + ni + nl + nk + ne + np) 
		abort();
/*
**	Use size fields to allocate font information
*/
	if (font_ptr == font_end) {
		start_font_error_message();
		print(" not loaded: No more room.");
		help_font();
		error();
		goto done;
	}
	k = (ec - bc + 1 + nl + ne) * sizeof(qcell) + 
		(nw + nh + nd + ni + nk + np + 1) * sizeof(scal);
	if (np < 7)
		k += (7 - np) * sizeof(scal);	
	f = null_font + font_ptr - font_info;
	*font_ptr = (font_t *)malloc(sizeof(font_t) + k);
	if (*font_ptr == (font_t *) 0) {
		start_font_error_message();
		print(" not loaded: No more room.");
		help_font();
		error();
		goto done;
	}
	font_length(f) = sizeof(font_t) + k;
	fp = (qcell *) (fmem(f) + 1);
	char_base(f) = fp - bc;
	width_base(f) = (scal *) (char_base(f) + ec + 1);
	height_base(f) = width_base(f) + nw;
	depth_base(f) = height_base(f) + nh;
	italic_base(f) = depth_base(f) + nd;
	lig_kern_base(f) = (qcell *) (italic_base(f) + ni);
	kern_base(f) = (scal *) (lig_kern_base(f) + nl);
	exten_base(f) = (qcell *) (kern_base(f) + nk);
	param_base(f) = (scal *) (exten_base(f) + ne);
/*
**	Read the TFM header
*/
	if (lh < 2)
		abort();
	store_four_quarters(font_check(f));
	read_sixteen(z);
	get_font_byte();
	z = z * 0400 + font_byte;
	get_font_byte();
	z = z * 020 + font_byte / 020;
	if (z < UNITY)
		abort();
	while (lh > 2) {
		store_four_quarters(dev_null);
		decr(lh);
	}
	font_dsize(f) = z;
	if (s != -1000) {
		if (s >= 0)
			z = s;
		else z = xn_over_d(z, -s, 1000L);
	}
	font_size(f) = z;
/*
**	Read  character data
*/
	for (pq = fp; pq < (qcell *)width_base(f); incr(pq)) {
		store_four_quarters(*pq);
		if (a >= nw || b / 020 >= nh || b % 020 >= nd || c / 4 > ni) 
			abort();
		switch (c % 4)
		{
		case LIG_TAG:
			if (d >= nl)
				abort();
			break;
		
		case EXT_TAG:
			if (d >= ne)
				abort();
			break;
		
		case LIST_TAG:
#define ccbwo ((int) (pq + bc - fp))
			check_byte_range(d);
			while (d < ccbwo) {
				qw = char_info(f, d);
				if (char_tag(qw) != LIST_TAG)
					goto not_found;
				d = rem_byte(qw);
			}
			if (d == ccbwo)
				abort();
			break;

		not_found:
			break;
		}
	}
/*
**	Read box dimensions
*/
	alpha = 16;
	while (z >= 040000000) {
		z >>= 1;
		alpha <<= 1;
	}
	beta = 256 / alpha;
	alpha *= z;
	for (ps = width_base(f); ps < (scal *)lig_kern_base(f); incr(ps))
		store_scaled(*ps);
	if (width_base(f)[0] != 0)
		abort();
	if (height_base(f)[0] != 0)
		abort();
	if (depth_base(f)[0] != 0)
		abort();
	if (italic_base(f)[0] != 0)
		abort();
/*
**	Read ligature/kern programs
*/
	bch_label = 077777;
	bchar = 256;
	if (nl > 0) {
		pq = lig_kern_base(f);
		while (pq < (qcell *) kern_base(f)) {
			store_four_quarters(*pq);
			if (a > 128) {
				if (256 * c + d >= nl)
					abort();
				if (a == 255
				&& pq == lig_kern_base(f)) {
					bchar = b;
				}
			} else {
				if (b != bchar) {
					check_existence(b);
				}
				if (c < 128) {
					check_existence(d);
				} else if (256 * (c - 128) + d >= nk) {
					abort();
				}
				if (a < 128
				&& pq - lig_kern_base(f) + a + 1 >= nl) {
					abort();
				}
			}
			if (a == 255) {
				bch_label = 256 * c + d;
			}
			incr(pq);
		}
	}
/*
**	Read kern dimensions
*/
	for (ps = kern_base(f); ps < (scal *)exten_base(f); incr(ps))
		store_scaled(*ps);
/*
**	Read extensible character recipes
*/
	for (pq = exten_base(f); pq < (qcell *)param_base(f); incr(pq)) {
		store_four_quarters(*pq);
		if (a != 0)
			check_existence(a);
		if (b != 0)
			check_existence(b);
		if (c != 0)
			check_existence(c);
		check_existence(d);
	}
/*
**	Read font parameters
*/
	for (k = 1; k <= np; incr(k)) {
		if (k == 1) {
			get_font_byte();
			sw = font_byte;
			if (sw > 127)
				sw -= 256;
			get_font_byte();
			sw = sw * 0400 + font_byte;
			get_font_byte();
			sw = sw * 0400 + font_byte;
			get_font_byte();
			param(f, 0) = sw * 020 + font_byte / 020;
		} else {
			store_scaled(param(f, k - 1));
		}
	}
	if (feof(tfm_file))
		abort();

	for (k = np + 1; k <= 7; incr(k))
		param(f, k - 1) = 0;
/*
**	Make final adjustments and done
*/
	decr(param_base(f));
	font_params(f) = (np >= 7) ? np : 7;
	hyphen_char(f) = default_hyphen_char;
	skew_char(f) = default_skew_char;
	if (bch_label < nl) {
		bchar_label(f) = lig_kern_base(f) + bch_label;
	} else {
		bchar_label(f) = NON_ADDRESS;
	}
	font_bchar(f) = bchar;
	font_false_bchar(f) = bchar;
	if (bchar <= ec && bchar >= bc) {
		qw = char_info(f, bchar);
		if (char_exists(qw))
			font_false_bchar(f) = NON_CHAR;
	}
	font_name(f) = nom;
	font_area(f) = aire;
	font_bc(f) = bc;
	font_ec(f) = ec;
	font_glue(f) = null;
	font_used(f) = FALSE;
	incr(font_ptr);
	g = f;
	goto done;

bad_tfm:
	start_font_error_message();
	if (file_opened) {
		print(" not loadable: Bad metric (TFM) file");
	} else {
		print(" not loadable: Metric (TFM) file not found");
	}
	help_tfm();
	error();

done:
	if (file_opened)
		b_close(tfm_file);
	return g;
}

void
scan_font_ident ()
{	
	int	m;
	fnt	f;

	get_nbx_token();
	if (cur_cmd == DEF_FONT) {
		f = cur_font;
	} else if (cur_cmd == SET_FONT) {
		f = cur_chr;
	} else if (cur_cmd == DEF_FAMILY) {
		m = cur_chr;
		scan_four_bit_int();
		f = reg_equiv(fnt_reg[1 + m + cur_val]);
	} else {
		print_err("Missing font identifier");
		help_font_cs();
		back_error();
		f = null_font;
	}
	cur_val = f;
}

scal *find_font_dimen(bool writing)
	{
	fnt	f;
	int	i, l, m, n, x;
	char	*fp, *nfp;

	scan_int();
	n = cur_val;
	scan_font_ident();
	f = cur_val;
	if (n <= 0) {
		goto err;
	} else {
		if (writing &&
			n <= SPACE_SHRINK_CODE &&
			n >= SPACE_CODE &&
			font_glue(f) != null) {
			delete_glue_ref(font_glue(f));
			font_glue(f) = null;
		}
		if (n > font_params(f)) {
			if (f < null_font + font_ptr - font_info - 1) {
				goto err;
			}
			i = font_params(f);
			l = font_length(f);
			m = l + (n - font_params(f)) * sizeof(scal);
			fp = (char *)fmem(f);
			nfp = (char *)realloc((void *)fp, m);
			x = nfp - fp;
			if (fp == (char *) 0)
				confusion("font mem");
			if (nfp == (char *) 0)
				overflow("font mem", m);
			fmem(f) = (font_t *) nfp;
			adjust_bases(f, x);
			font_length(f) = m;
			font_params(f) = n;
			while (i++ < n)
				param(f, i) = 0;
			return (param_base(f) + font_params(f));
		} else {
			return (param_base(f) + n);
		}
	}

err:
	print_nl("! Font ");
	print_esc(font_id_text(f));
	print(" has only ");
	print_int(font_params(f));
	print(" fontdimen parameters");
	help_font_param();
	error();
	return ((scal *) 0);
}
		
void adjust_bases(fnt f, int x)
	{
	char_base(f) = (qcell *) ((char *)char_base(f) + x);
	width_base(f) = (scal *) ((char *)width_base(f) + x);
	height_base(f) = (scal *) ((char *)height_base(f) + x);
	depth_base(f) = (scal *) ((char *)depth_base(f) + x);
	italic_base(f) = (scal *) ((char *)italic_base(f) + x);
	lig_kern_base(f) = (qcell *) ((char *)lig_kern_base(f) + x);
	bchar_label(f) = (qcell *) ((char *)bchar_label(f) + x);
	kern_base(f) = (scal *) ((char *)kern_base(f) + x);
	exten_base(f) = (qcell *) ((char *)exten_base(f) + x);
	param_base(f) = (scal *) ((char *)param_base(f) + x);
	}

ptr find_font_glue(fnt f)
	{
	ptr	p;
	scal	*q;

	p = font_glue(f);
	if (p == null) {
		p = new_spec(zero_glue);
		q = param_base(cur_font) + SPACE_CODE;
		glue_width(p) = *q++;
		stretch(p) = *q++;
		shrink(p) = *q++; 
		font_glue(f) = p;
	}
	return p;
	}

void char_warning(fnt f, int c)
	{
	if (tracing_lost_chars > 0) {
		begin_diagnostic();
		print_nl("Missing character: There is no ");
		print_ASCII(c);
		print(" in font ");
		print(font_name(f));
		print("!");
		end_diagnostic(FALSE);
		}
	}

ptr new_character(fnt f, int c)
	{
	ptr	p;

	if (font_bc(f) <= c && font_ec(f) >= c
	&& char_exists(char_info(f, c))) {
		p = new_avail();
		font(p) = f;
		character(p) = c;
		return p;
		}
	char_warning(f, c);
	return null;
	}

void
_tfm_init ()
{
}

void
_tfm_init_once ()
{
	int	l;

	nfonts = FONT_MAX-FONT_BASE+1;
	font_info = (font_t **)malloc(nfonts * sizeof(font_t *));
	if (font_info == (font_t **) 0)
		overflow("font info", nfonts);
	font_end = font_info + nfonts;
	font_ptr = font_info + 1;
	l = sizeof(font_t) + 7 * sizeof(scal);
	font_info[0] = (font_t *)malloc(l);
	if (font_info[0] == (font_t *) 0)
		overflow("nullfont", l);
	memset(fmem(null_font), 0, l);
	font_length(null_font) = l;
	param_base(null_font) = (scal *) (fmem(null_font) + 1);
	font_params(null_font) = 7;
	font_area(null_font) = null_str;
	font_name(null_font) = "nullfont";
	font_bc(null_font) = 1;
	font_ec(null_font) = 0;
	font_glue(null_font) = zero_glue;
	hyphen_char(null_font) = '-';
	skew_char(null_font) = -1;
	bchar_label(null_font) = NON_ADDRESS;
	font_bchar(null_font) = NON_CHAR;
	font_false_bchar(null_font) = NON_CHAR;
}

/*
**	Help text
*/

void help_font()
	{
	help4("I'm afraid I won't be able to make use of this font,",
	"because my memory for character-size data is too small.",
	"If you're really stuck, ask a wizard to enlarge me.",
	"Or maybe try `I\\font<same font id>=<name of loaded font>'.");
	}

void help_tfm()
	{
	help5("I wasn't able to read the size data for this font,",
	"so I will ignore the font specification.",
	"[Wizards can fix TFM files using TFtoPL/PLtoTF.]",
	"You might try inserting a different font spec;",
	"e.g., type `I\\font<same font id>=<substitute font name>'.");
	}

void help_font_param()
	{
	help2("To increase the number of font parameters, you must",
	"use \\fontdimen immediately after the \\font is loaded.");
	}

void help_font_cs()
	{
	help2("I was looking for a control sequence whose",
	"current meaning has been defined by \\font.");
	}
