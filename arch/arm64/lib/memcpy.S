/*
 * Copyright (c) 2014, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/linkage.h>
#include <asm/assembler.h>

/* Parameters */
dstin	.req	x0
src	.req	x1
count	.req	x2

/* Return value */
ret_val	.req	x0	/* Aliased with dstin */

/* Local version of dstin */
dst	.req	x3
ENTRY(memcpy)
	prfm    pldl1strm, [src, #(1*L1_CACHE_BYTES)]
	mov	dst, dstin
	cmp	count, #16
	/*When memory length is less than 16, the accessed are not aligned.*/
	b.lo	.Ltiny15


#define LOAD_USER( offset, x...) x
#define STORE_USER(offset, x...) x

ENTRY(memcpy)
	mov	dst, dstin
#include "memcpy_base.h"
.Lexitfunc:
	ret

.Lcpy_over64:
	subs	count, count, #128
	b.ge	.Lcpy_body_large
	/*
	* Less than 128 bytes to copy, so handle 64 here and then jump
	* to the tail.
	*/
	ldp	A_l, A_h, [src],#16
	stp	A_l, A_h, [dst],#16
	ldp	B_l, B_h, [src],#16
	ldp	C_l, C_h, [src],#16
	stp	B_l, B_h, [dst],#16
	stp	C_l, C_h, [dst],#16
	ldp	D_l, D_h, [src],#16
	stp	D_l, D_h, [dst],#16

	tst	count, #0x3f
	b.ne	.Ltail63
	ret

	/*
	* Critical loop.  Start at a new cache line boundary.  Assuming
	* 64 bytes per line this ensures the entire loop is in one line.
	*/
	.p2align	L1_CACHE_SHIFT
.Lcpy_body_large:
	/* pre-get 64 bytes data. */
	ldp	A_l, A_h, [src],#16
	ldp	B_l, B_h, [src],#16
	ldp	C_l, C_h, [src],#16
	ldp	D_l, D_h, [src],#16
1:
	/*
	* interlace the load of next 64 bytes data block with store of the last
	* loaded 64 bytes data.
	*/
	stp	A_l, A_h, [dst],#16
	ldp	A_l, A_h, [src],#16
	stp	B_l, B_h, [dst],#16
	ldp	B_l, B_h, [src],#16
	stp	C_l, C_h, [dst],#16
	ldp	C_l, C_h, [src],#16
	stp	D_l, D_h, [dst],#16
	ldp	D_l, D_h, [src],#16
	prfm    pldl1strm, [src, #(4*L1_CACHE_BYTES)]
	subs	count, count, #64
	b.ge	1b
	stp	A_l, A_h, [dst],#16
	stp	B_l, B_h, [dst],#16
	stp	C_l, C_h, [dst],#16
	stp	D_l, D_h, [dst],#16

	tst	count, #0x3f
	b.ne	.Ltail63
	ret
ENDPROC(memcpy)
