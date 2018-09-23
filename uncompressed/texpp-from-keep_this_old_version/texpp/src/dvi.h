
/*
 * $Id: dvi.h,v 1.3 2002/04/18 09:50:43 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */

extern	file	dvi_file;
extern	str	dvi_name;

#define SET_CHAR_0	0
#define SET1		128
#define SET_RULE	132
#define PUT1		133
#define PUT_RULE	137
#define NOP		138
#define BOP		139
#define EOP		140
#define PUSH		141
#define POP		142
#define RIGHT1		143
#define W0		147
#define W1		148
#define X0		152
#define X1		153
#define DOWN1		157
#define Y0		161
#define Y1		162
#define Z0		166
#define Z1		167
#define FNT_NUM_0	171
#define FNT1		235
#define XXX1		239
#define XXX4		242
#define FNT_DEF1	243
#define PRE		247
#define POST		248
#define POST_POST	249
#define ID_BYTE		2

extern	int	dead_cycles;
extern	bool	doing_leaders;
extern	int	last_bop;
extern	scal	max_h;
extern	scal	max_v;
extern	int	max_push;
extern	scal	rule_ht;
extern	scal	rule_dp;
extern	scal	rule_wd;

extern	fnt	dvi_f;
extern	scal	dvi_h;
extern	scal	dvi_v;
extern	scal	cur_h;
extern	scal	cur_v;
extern	int	cur_s;

extern	ptr	down_ptr;
extern	ptr	right_ptr;

extern	byte	*dvi_buf;
extern	ptr	dvi_limit; 
extern	ptr	dvi_ptr;
extern	int	dvi_offset;
extern	int	dvi_gone;
extern	bool	output_active;
extern	int	total_pages;

struct move_t {
	mcell	node_field;
	scal	movement_field;
	int	location_field;
};
typedef struct move_t move_t;

#define MOVEMENT_NODE_SIZE	sizeof(move_t)
#define move_amount(P)		((move_t *) (P))->movement_field
#define location(P)		((move_t *) (P))->location_field

#define synch_h() \
	{if (cur_h != dvi_h) {movement(cur_h - dvi_h, RIGHT1); dvi_h = cur_h;}}

#define synch_v() \
	{if (cur_v != dvi_v) {movement(cur_v - dvi_v, DOWN1); dvi_v = cur_v;}}

#define dvi_out(B) \
	{dvi_buf[dvi_ptr++] = B; if (dvi_ptr == dvi_limit) dvi_swap();}

#define write_dvi(a, b) \
	{fwrite((char *)&dvi_buf[a], sizeof(dvi_buf[0]), b - a, dvi_file);}

void	ship_out(ptr p);
void	hlist_out(ptr p);
void	hskip_out(ptr p, ptr this_box, scal left_edge, scal base_line);
void	hrule_out(str this_box, int base_line);
void	vlist_out(ptr this_box);
void	vskip_out(ptr p, ptr this_box, scal left_edge, scal top_edge);
void	vrule_out(ptr this_box);
void	fnt_out(fnt f);
void	dvi_font_def(fnt f);
void	dvi_swap();
void	dvi_four(int x);
void	dvi_pop(int);
void	movement(scal m, int o);
void	prune_movements(int l);
void	start_dvi();
void	fin_dvi();
void  help_huge_page();
void	_dvi_init();
void	_dvi_init_once();
