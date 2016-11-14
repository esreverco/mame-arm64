// license:Public Domain
// copyright-holders:Richard Outerbridge

/* d3des.h -
*
*	Headers and defines for d3des.c
*	Graven Imagery, 1992.
*
* Copyright (c) 1988,1989,1990,1991,1992 by Richard Outerbridge
*	(GEnie : OUTER; CIS : [71755,204])
*
* Modified and adapted by Xavier Leroy, 2002.
*/

#ifndef __D3DES_H__
#define __D3DES_H__

#define EN0	0	/* MODE == encrypt */
#define DE1	1	/* MODE == decrypt */

extern void d3des_cook_key(unsigned char key[8], int mode, unsigned int res[32]);
/* Sets the key register [res] according to the hexadecimal
* key contained in the 8 bytes of [key], according to the DES,
* for encryption or decryption according to [mode].
*/

extern void d3des_transform(unsigned int key[32], unsigned char from[8], unsigned char to[8]);
/* Encrypts/Decrypts (according to the key [key])
* one block of eight bytes at address 'from'
* into the block at address 'to'.  They can be the same.
*/

#endif // __D3DES_H__