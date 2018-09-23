
/*
 * $Id: tfm.h,v 1.3 2002/04/18 09:50:43 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

extern	file	tfm_file;

struct font_struct {
	int	font_length_field;
	qcell	font_check_field;
	scal	font_size_field;
	scal	font_dsize_field;
	int	font_params_field;
	str	font_name_field;
	str	font_area_field;
	int	font_bc_field;
	int	font_ec_field;
	ptr	font_glue_field;
	bool	font_used_field;
	int	hyphen_char_field;
	int	skew_char_field;
	int	font_bchar_field;
	int	font_false_bchar_field;
	qcell	*char_base_field;
	scal	*width_base_field;
	scal	*height_base_field;
	scal	*depth_base_field;
	scal	*italic_base_field;
	qcell	*lig_kern_base_field;
	qcell	*bchar_label_field;
	scal	*kern_base_field;
	qcell	*exten_base_field;
	scal	*param_base_field;
};
typedef struct font_struct font_t;

extern	int	nfonts;
extern	font_t	**font_info;
extern	font_t	**font_end;
extern	font_t	**font_ptr;

#define FONT_BASE	32
#define FONT_MAX	1250
#define null_font	FONT_BASE

extern	qcell	null_character;

#define fmem(F)	(font_info[(F) - null_font])

#define font_length(F)		fmem(F)->font_length_field
#define font_check(F)		fmem(F)->font_check_field
#define font_size(F)		fmem(F)->font_size_field
#define font_dsize(F)		fmem(F)->font_dsize_field
#define font_params(F)		fmem(F)->font_params_field
#define font_name(F)		fmem(F)->font_name_field
#define font_area(F)		fmem(F)->font_area_field
#define font_bc(F)		fmem(F)->font_bc_field
#define font_ec(F)		fmem(F)->font_ec_field
#define font_glue(F)		fmem(F)->font_glue_field
#define font_used(F)		fmem(F)->font_used_field
#define hyphen_char(F)		fmem(F)->hyphen_char_field
#define skew_char(F)		fmem(F)->skew_char_field
#define font_bchar(F)		fmem(F)->font_bchar_field
#define font_false_bchar(F)	fmem(F)->font_false_bchar_field
#define bchar_label(F)		fmem(F)->bchar_label_field
#define char_base(F)		fmem(F)->char_base_field
#define width_base(F)		fmem(F)->width_base_field
#define height_base(F)		fmem(F)->height_base_field
#define depth_base(F)		fmem(F)->depth_base_field
#define italic_base(F)		fmem(F)->italic_base_field
#define lig_kern_base(F)	fmem(F)->lig_kern_base_field
#define kern_base(F)		fmem(F)->kern_base_field
#define param_base(F)		fmem(F)->param_base_field
#define exten_base(F)		fmem(F)->exten_base_field

#define NON_CHAR		256
#define NON_ADDRESS		0

#define char_info(F, C)		char_base(F)[C]
#define char_exists(CI)		((CI).b0 > 0)
#define char_width(F, CI)	width_base(F)[(CI).b0]
#define height_depth(CI)	(CI).b1
#define char_height(F, CB)	height_base(F)[(CB) / 16]
#define char_depth(F, CB)	depth_base(F)[(CB) % 16]
#define char_italic(F, CI)	italic_base(F)[(CI).b2 / 4]
#define char_tag(CI)		((CI).b2 % 4)

#define NO_TAG			0
#define LIG_TAG			1
#define LIST_TAG		2
#define EXT_TAG			3

#define skip_byte(LK)		(LK).b0
#define next_char(LK)		(LK).b1
#define op_byte(LK)		(LK).b2
#define rem_byte(LK)		(LK).b3
#define lig_offset(LK)		((op_byte(LK) * 256) + rem_byte(LK))
#define kern_offset(LK)		(((op_byte(LK) - 128) * 256) + rem_byte(LK))
#define char_kern(F, LK)	kern_base(F)[kern_offset(LK)]
#define lig_kern_start(F, LK)	(lig_kern_base(F) + rem_byte(LK))
#define lig_kern_restart(F, LK)	(lig_kern_base(F) + lig_offset(LK))

#define STOP_FLAG		128
#define KERN_FLAG		128

#define ext_top(M)		M.b0
#define ext_mid(M)		M.b1
#define ext_bot(M)		M.b2
#define ext_rep(M)		M.b3

#define SLANT_CODE		1
#define SPACE_CODE		2
#define SPACE_STRETCH_CODE	3
#define SPACE_SHRINK_CODE	4
#define X_HEIGHT_CODE		5
#define QUAD_CODE		6
#define EXTRA_SPACE_CODE	7

#define param(F, CODE)		param_base(F)[CODE]
#define slant(F)		param(F, SLANT_CODE)
#define space(F)		param(F, SPACE_CODE)
#define space_stretch(F)	param(F, SPACE_STRETCH_CODE)
#define space_shrink(F)		param(F, SPACE_SHRINK_CODE)
#define x_height(F)		param(F, X_HEIGHT_CODE)
#define quad(F)			param(F, QUAD_CODE)
#define extra_space(F)		param(F, EXTRA_SPACE_CODE)

fnt	read_font_info(sym u, str nom, str aire, scal s);
void	scan_font_ident();
scal	*find_font_dimen(bool writing);
void  adjust_bases(fnt f, int x);
ptr	find_font_glue(fnt f);
void	char_warning(fnt f, int c);
ptr	new_character(fnt f, int c);
void  help_font();
void  help_tfm();
void  help_font_param();
void  help_font_cs();
void	_tfm_init();
void	_tfm_init_once();
