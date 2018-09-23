
/*
 * $Id: tex.h,v 1.14 2002/08/26 15:15:09 t16 Exp $
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 * Copyright 2002           Kasper Peeters
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 * 
 */


#include <stdint.h>
#include	<stdio.h>
#include	<string.h>
#include	<signal.h>
#include	<time.h>
#include	<malloc.h>

struct override_t {
		const char *dimname;
		int32_t     value;
		int32_t     reg;
	   bool        override_always;
		bool        addsubtoo;
};
struct override_internal_t {
		int32_t   code;
		int32_t   value;
	   bool      done_once;
	   bool      override_always;
};

extern override_t override[];
extern override_internal_t override_internal[];
extern const unsigned int num_override;
extern const unsigned int num_override_internal;
extern bool append_to_vlist_called;

extern bool display_debug;
extern bool display_warnings;
extern bool display_progress;
extern bool display_messages;
extern bool compatible;

/*
 * constants in the outer block
 */

#define NUL			'\0'
#define EOLN			'\n'
#define FALSE			0
#define TRUE			1
#define EMPTY			0

#define MIN_QUARTERWORD		0
#define MAX_QUARTERWORD		255
#define MIN_HALFWORD		0
#define MAX_HALFWORD		65535

/*
 *	types in the outer block
 */

typedef uint8_t	 byte;
typedef int32_t	 tok;
typedef int32_t    fnt;
typedef float		 ratio;
typedef int32_t 	 scal;
typedef char*		 str;
typedef FILE*		 file;
typedef int        ptr;
//typedef int32_t*   ptr;

struct qcell {
	short	b0;
	short	b1;
	short	b2;
	short	b3;
};
typedef struct qcell qcell;

union hcell { 
	int	i;   
	ptr	p;
	struct {
		short	b0;
		short	b1;
	} qq;
};
typedef union hcell hcell;

class mcell { 
	public:
//		ptr& link();

		hcell	h1;
		hcell	h2;
};
//typedef struct mcell mcell;

#define rinfo(P)	   ((mcell *) (P))->h1.i
#define link(P)		((mcell *) (P))->h1.p
#define info(P)		((mcell *) (P))->h2.i
#define llink(P)	   ((mcell *) (P))->h2.p
#define type(P)		((mcell *) (P))->h2.qq.b0
#define subtype(P)	((mcell *) (P))->h2.qq.b1
#define	q0(P)		   ((mcell *) (P))->h1.qq.b0
#define	q1(P)		   ((mcell *) (P))->h1.qq.b1
#define	q2(P)		   ((mcell *) (P))->h2.qq.b0
#define	q3(P)		   ((mcell *) (P))->h2.qq.b1

#ifdef NOLCASTS
#define mcopy(x,y)	memcpy((void *)x, (void *)y, sizeof(mcell))
#else
#define mcopy(x,y)	*(mcell *)x = *(mcell *)y
#endif
#define mzero(m)	memset((void *)m,0,sizeof(mcell))

/*
 *	variables in the outer block
 */

extern	char	*banner;

/*
 *	functions in the outer block
 */

void	initialize();
void	initialize_once();
bool	decode_args(int, int, char **);
void	fix_date_and_time();
void	handle_int(int);
void	final_cleanup();
void	close_files();

/*
 * some common programming idioms
 */

#define incr(i)		++(i)
#define decr(i)	 	--(i)
#define odd(i)		((i) & 1)
// #define round(x)	(int) ((x) > 0.0 ? ((x) + 0.5) : ((x) - 0.5))
#define negate(x)	(x) = -(x)
#define loop		while (1)

#include "align.h"
#include "alloc.h"
#include "arith.h"
#include "box.h"
#include "boxlist.h"
#include "chr.h"
#include "cmd.h"
#include "cond.h"
#include "def.h"
#include "dvi.h"
#include "sym.h"
#include "error.h"
#include "eval.h"
#include "expand.h"
#include "fileio.h"
#include "hyph.h"
#include "math.h"
#include "mathlist.h"
#include "mlst-hlst.h"
#include "pack.h"
#include "page.h"
#include "par.h"
#include "print.h"
#include "scan.h"
#include "str.h"
#include "tfm.h"
#include "tok.h"
#include "toklist.h"
#include "texext.h"
