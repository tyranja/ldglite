/*
 *  LDLITE, a program for viewing *.dat files.
 *  Copyright (C) 1998  Paul J. Gyugyi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "ldliteVR.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"

#undef GREYSCALE  // Define to force everything to 2-bit 4-level grey for Gameboy & WinCE devices

int zcolor_unalias(int index, char *name);
int zcolor_alias(int index, char *name);


ZCOLOR_DEF_TABLE_ENTRY zcolor_table_default[ZCOLOR_TABLE_DEFAULT_SIZE] = {
	{"Black",             8,{0x22,0x22,0x22,0xff},{0x22,0x22,0x22,0xff}}, // 0
	{"Blue",              9,{0x00,0x33,0xb2,0xff},{0x00,0x33,0xb2,0xff}}, // 1
	{"Green",            10,{0x00,0x7f,0x33,0xff},{0x00,0x7f,0x33,0xff}}, // 2
	{"Dark-Cyan",        11,{0x00,0xaa,0xaa,0xff},{0x00,0xaa,0xaa,0xff}}, // 3
	{"Red",              12,{0xcc,0x00,0x00,0xff},{0xcc,0x00,0x00,0xff}}, // 4
	{"Magenta",          13,{0xff,0x33,0x99,0xff},{0xff,0x33,0x99,0xff}}, // 5
	{"Brown",             0,{0x66,0x33,0x00,0xff},{0x66,0x33,0x00,0xff}}, // 6
	{"Light-Gray",        8,{0xaa,0xaa,0xaa,0xff},{0xaa,0xaa,0xaa,0xff}}, // 7
	{"Dark-Gray",         0,{0x66,0x66,0x58,0xff},{0x66,0x66,0x58,0xff}}, // 8
	{"Light-Blue",        1,{0x00,0x80,0xff,0xff},{0x00,0x80,0xff,0xff}}, // 9
	{"Light-Green",       2,{0x33,0xff,0x66,0xff},{0x33,0xff,0x66,0xff}}, // 10
	{"Cyan",              3,{0x55,0xaa,0xff,0xff},{0x55,0xaa,0xff,0xff}}, // 11
	{"Light-Red",         4,{0xff,0x55,0x55,0xff},{0xff,0x55,0x55,0xff}}, // 12
	{"Pink",              5,{0xff,0xb0,0xcc,0xff},{0xff,0xb0,0xcc,0xff}}, // 13
	{"Yellow",            8,{0xff,0xe5,0x00,0xff},{0xff,0xe5,0x00,0xff}}, // 14
	{"White",             8,{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff}}, // 15
	{"Main-Color",        0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0xff}}, // 16
	{"Pastel-Green",      2,{0x66,0xf0,0x99,0xff},{0x66,0xf0,0x99,0xff}}, // 17
	{"Light-Yellow",     14,{0xff,0xff,0x80,0xff},{0xff,0xff,0x80,0xff}}, // 18
	{"Tan",               6,{0xcc,0xaa,0x66,0xff},{0xcc,0xaa,0x66,0xff}}, // 19
	{"Light-Purple",      8,{0xe0,0xcc,0xf0,0xff},{0xe0,0xcc,0xf0,0xff}}, // 20
	{"Glow-In-The-Dark",  0,{0xe0,0xff,0xb0,0xff},{0xe0,0xff,0xb0,0xff}}, // 21
	{"Purple",            8,{0x99,0x33,0x99,0xff},{0x99,0x33,0x99,0xff}}, // 22
	{"Purple-Blue",       8,{0x4c,0x00,0xcc,0xff},{0x4c,0x00,0xcc,0xff}}, // 23
	{"Edge-Color",        0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0xff}}, // 24
	{"Orange-Solid",      8,{0xff,0x66,0x00,0xff},{0xff,0x66,0x00,0xff}}, // 25
	{"Dark-Pink",         0,{0xff,0x33,0x99,0xff},{0xff,0x33,0x99,0xff}}, // 26
	{"Lime-Green",        0,{0xad,0xdd,0x50,0xff},{0xad,0xdd,0x50,0xff}}, // 27
	{"Tan-Solid",         0,{0xcc,0xaa,0x66,0xff},{0xcc,0xaa,0x66,0xff}}, // 28
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0xff}}, // 29
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0xff}}, // 30
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0xff}}, // 31
	{"Trans-Black",       8,{0x22,0x22,0x22,0xff},{0x7f,0x7f,0x7f,0x00}}, // 32
	{"Trans-Blue",        9,{0x00,0x00,0x99,0xff},{0x7f,0x7f,0x7f,0x00}}, // 33
	{"Trans-Green",      10,{0x00,0x50,0x18,0xff},{0x7f,0x7f,0x7f,0x00}}, // 34
	{"Trans-Dark_Cyan",  11,{0x00,0xaa,0xaa,0xff},{0x7f,0x7f,0x7f,0x00}}, // 35
	{"Trans-Red",        12,{0xcc,0x00,0x00,0xff},{0x7f,0x7f,0x7f,0x00}}, // 36
	{"Trans-Magenta",    13,{0xff,0x33,0x99,0xff},{0x7f,0x7f,0x7f,0x00}}, // 37  
	{"Trans-Brown",       0,{0x66,0x33,0x00,0xff},{0x7f,0x7f,0x7f,0x00}}, // 38
	{"Trans-Light-Gray",  8,{0x99,0x99,0x99,0xff},{0x7f,0x7f,0x7f,0x00}}, // 39
	{"Trans-Dark-Gray",   0,{0x66,0x66,0x58,0xff},{0x7f,0x7f,0x7f,0x00}}, // 40
	{"Trans-Light-Blue",  1,{0x99,0xc0,0xf0,0xff},{0x7f,0x7f,0x7f,0x00}}, // 41
	{"Trans-Light-Green", 2,{0xcc,0xff,0x00,0xff},{0x7f,0x7f,0x7f,0x00}}, // 42
	{"Trans-Cyan",        3,{0x55,0xaa,0xff,0xff},{0x7f,0x7f,0x7f,0x00}}, // 43
	{"Trans-Light-Red",   4,{0xff,0x55,0x55,0xff},{0x7f,0x7f,0x7f,0x00}}, // 44
	{"Trans-Pink",        5,{0xff,0xb0,0xcc,0xff},{0x7f,0x7f,0x7f,0x00}}, // 45
	{"Trans-Yellow",      8,{0xf0,0xc4,0x00,0xff},{0x7f,0x7f,0x7f,0x00}}, // 46
	{"Trans-White",       8,{0xff,0xff,0xff,0xff},{0x7f,0x7f,0x7f,0x00}}, // 47
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 48
	{"unused",            2,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 49
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 50
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 51
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 52
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 53
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 54
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 55
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 56
	{"Trans-Orange",      8,{0xff,0x66,0x00,0xff},{0x7f,0x7f,0x7f,0x00}}, // 57
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 58
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 59
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 60
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 61
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}, // 62
	{"unused",            0,{0x7f,0x7f,0x7f,0xff},{0x7f,0x7f,0x7f,0x00}}  // 63
};

ZCOLOR_TABLE_ENTRY *zcolor_table_stack[MAX_ZCOLOR_DEPTH];
ZCOLOR_TABLE_ENTRY *zcolor_table;
ZCOLOR_TABLE_ENTRY *zcolor_prev_table;

typedef struct zcolor_namelist_entry_type {
	char *name;
	int color_value;
	struct zcolor_namelist_entry_type *next;
}	ZCOLOR_NAMELIST_ENTRY;

ZCOLOR_NAMELIST_ENTRY *zcolor_namelist_stack[MAX_ZCOLOR_DEPTH];

typedef struct zpoint_namelist_entry_type {
	char *name;
	vector3d point;
	struct zpoint_namelist_entry_type *next;
}	ZPOINT_NAMELIST_ENTRY;

ZPOINT_NAMELIST_ENTRY *zpoint_namelist_stack[MAX_ZCOLOR_DEPTH];

typedef struct zmatrix_namelist_entry_type {
	char *name;
	matrix3d matrix;
	struct zmatrix_namelist_entry_type *next;
}	ZMATRIX_NAMELIST_ENTRY;

ZMATRIX_NAMELIST_ENTRY *zmatrix_namelist_stack[MAX_ZCOLOR_DEPTH];

int znamelist_stack_index=0;

void zcolor_init()
{
	ZCOLOR_TABLE_ENTRY defcolor;
	int i;

	defcolor.inverse_index=0;
	defcolor.primary.r=0x7f;
	defcolor.primary.g=0x7f;
	defcolor.primary.b=0x7f;
	defcolor.primary.a=0xff;
	defcolor.dither.r=0x7f;
	defcolor.dither.g=0x7f;
	defcolor.dither.b=0x7f;
	defcolor.dither.a=0xff;


	znamelist_stack_index = 0;
	for(i=0; i<MAX_ZCOLOR_DEPTH; i++) {
		zcolor_table_stack[i] = NULL;
		zcolor_namelist_stack[i] = NULL;
		zpoint_namelist_stack[i] = NULL;
		zmatrix_namelist_stack[i] = NULL;
	}

	zcolor_table_stack[znamelist_stack_index] = 
		(ZCOLOR_TABLE_ENTRY *)malloc(ZCOLOR_TABLE_SIZE*sizeof(ZCOLOR_TABLE_ENTRY));
	if (zcolor_table_stack[znamelist_stack_index] == NULL) {
		zWrite("Fatal error: cannot malloc zcolor table\n");
		exit(-1);
	}
	zcolor_table = zcolor_table_stack[znamelist_stack_index];
	zcolor_prev_table = NULL;
	for (i=0; i<ZCOLOR_TABLE_SIZE; i++) {
		if (i<ZCOLOR_TABLE_DEFAULT_SIZE) {
			zcolor_table[i].inverse_index = zcolor_table_default[i].inverse_index;
			zcolor_table[i].primary = zcolor_table_default[i].primary;
			zcolor_table[i].dither = zcolor_table_default[i].dither;
			// add zcolor_table_default[i].name
			if (strcmp("unused",zcolor_table_default[i].name)) {
				// Store name in namelist
				zcolor_alias(i, zcolor_table_default[i].name);
			}
		} else {
			zcolor_table[i] = defcolor;
		}
	}
	// some standard names
	zcolor_alias(383,"Chrome");
	zcolor_alias(494,"Electrical-Contacts");
	zcolor_alias(334,"Gold");
	zcolor_alias(495,"Light-Yellow-Belville");
	zcolor_alias(431,"Mint-Green-Belville");
	zcolor_alias(462,"Orange");
	zcolor_alias(382,"Tan");

#ifdef USE_OPENGL
	zcolor_alias(256,"Matte-Black"); // rubber for tires.
#endif  
}

// Call this when a file finishes.
// If a color change caused a new table to be malloc'd, free it.
void znamelist_pop()
{
	int i,j;

	if (znamelist_stack_index >= 0) {
		if (zcolor_namelist_stack[znamelist_stack_index] != NULL) {
			ZCOLOR_NAMELIST_ENTRY *znep;
			ZCOLOR_NAMELIST_ENTRY *zneq;

			znep = zcolor_namelist_stack[znamelist_stack_index];
			while(znep != NULL) {
				zneq = znep->next;
				if (ldraw_commandline_opts.debug_level>0) {
					char buf[256];
					sprintf(buf,"Removing zcolor namelist entry %s",znep->name);
					zWrite(buf);
				}
				free(znep->name);
				free(znep);
				znep = zneq;
			}
		}
		if (zpoint_namelist_stack[znamelist_stack_index] != NULL) {
			ZPOINT_NAMELIST_ENTRY *znep;
			ZPOINT_NAMELIST_ENTRY *zneq;

			znep = zpoint_namelist_stack[znamelist_stack_index];
			while(znep != NULL) {
				zneq = znep->next;
				if (ldraw_commandline_opts.debug_level>0) {
					char buf[256];
					sprintf(buf,"Removing zpoint namelist entry %s",znep->name);
					zWrite(buf);
				}
				free(znep->name);
				free(znep);
				znep = zneq;
			}
		}
		if (zmatrix_namelist_stack[znamelist_stack_index] != NULL) {
			ZMATRIX_NAMELIST_ENTRY *znep;
			ZMATRIX_NAMELIST_ENTRY *zneq;

			znep = zmatrix_namelist_stack[znamelist_stack_index];
			while(znep != NULL) {
				zneq = znep->next;
				if (ldraw_commandline_opts.debug_level>0) {
					char buf[256];
					sprintf(buf,"Removing zmatrix namelist entry %s",znep->name);
					zWrite(buf);
				}
				free(znep->name);
				free(znep);
				znep = zneq;
			}
		}
		if (zcolor_table_stack[znamelist_stack_index] != NULL) {
			if (ldraw_commandline_opts.debug_level>0) {
				char buf[256];
				sprintf(buf,"Removing color table index %d",znamelist_stack_index);
				zWrite(buf);
			}
			free(zcolor_table_stack[znamelist_stack_index]);
			zcolor_table_stack[znamelist_stack_index] = NULL;
			// Set zcolor_table to point to previous valid table.
			znamelist_stack_index--;
			for(i=znamelist_stack_index; i>=0; i--) {
				if (zcolor_table_stack[i]!=NULL) {
					zcolor_table = zcolor_table_stack[i];
					if (zcolor_table != zcolor_prev_table) {
						zWrite("Fatal error in zcolor table stack");
						exit(-1);
					}
					break;
				}
			}
			// Set zcolor_prev_table to point to previous previous valid table.
			if (i == 0) {
				zcolor_prev_table = NULL;
			} else {
				for(j=(i-1); j >=0 ; j++) {
					if (zcolor_table_stack[j]!=NULL) {
						zcolor_prev_table = zcolor_table_stack[j];
						break;
					}
				}
			}
		} else {
			znamelist_stack_index--;
		}
	} else {
		zWrite("Stack error in zcolor routines");
		exit(-1);
	}
}
	
// Call this before a color change statement to make sure
// the current scope has a table malloc'd.
void zcolor_copy_table_if_needed()
{
	ZCOLOR_TABLE_ENTRY *new_table;
	int i;

	if(zcolor_table == zcolor_table_stack[znamelist_stack_index]) {
		return;
	}
	if (ldraw_commandline_opts.debug_level>0) {
		char buf[256];
		sprintf(buf,"Creating new color table index %d",znamelist_stack_index);
		zWrite(buf);
	}
	new_table = 
		(ZCOLOR_TABLE_ENTRY *)malloc(ZCOLOR_TABLE_SIZE*sizeof(ZCOLOR_TABLE_ENTRY));
	if (new_table == NULL) {
		zWrite("Fatal error: cannot malloc zcolor table\n");
		exit(-1);
	}
	for (i=0; i<ZCOLOR_TABLE_SIZE; i++) {
			new_table[i] = zcolor_table[i];
	}
	zcolor_table_stack[znamelist_stack_index] = new_table;
	zcolor_prev_table = zcolor_table;
	zcolor_table = zcolor_table_stack[znamelist_stack_index];
}

// Do every time the scoping of color tables could change.
// To avoid unneeded work, we only malloc and copy the table
// if the colors are changed.
// Call this when a new file starts.
void znamelist_push()
{
	if (znamelist_stack_index < (MAX_ZCOLOR_DEPTH-1)) {
		znamelist_stack_index++;
		zcolor_table_stack[znamelist_stack_index] = NULL; // probably redundant
		zcolor_namelist_stack[znamelist_stack_index] = NULL; // probably redundant
		zpoint_namelist_stack[znamelist_stack_index] = NULL; // probably redundant
		zmatrix_namelist_stack[znamelist_stack_index] = NULL; // probably redundant
	} else {
		zWrite("Stack error in zcolor routines");
		exit(-1);
	}
}

int zcolor_lookup(char *name)
{
	int i;
	ZCOLOR_NAMELIST_ENTRY *znep;

	// See if it is in the namelist
	for(i=znamelist_stack_index; i>=0; i--) {
		znep = zcolor_namelist_stack[i];
		while (znep != NULL) {
			if (!strcmp(znep->name, name)) {
				return znep->color_value;
			}
			znep= znep->next;
		}
	}
	{
		char buf[256];
		sprintf(buf,"Warning, color \"%s\" not defined, using default instead",
			name);
		zWrite(buf);
	}
	return ldraw_commandline_opts.C;  // default if name does not exist
}

vector3d* zpoint_lookup(char *name)
{
	int i;
	ZPOINT_NAMELIST_ENTRY *znep;

	// See if it is in the namelist
	for(i=znamelist_stack_index; i>=0; i--) {
		znep = zpoint_namelist_stack[i];
		while (znep != NULL) {
			if (!strcmp(znep->name, name)) {
				// make a copy
				return copyvec(&(znep->point));
			}
			znep= znep->next;
		}
	}
	{
		char buf[256];
		sprintf(buf,"Warning, point \"%s\" not defined, using default instead",
			name);
		zWrite(buf);
	}
	return savevec(0.0,0.0,0.0);  // default if name does not exist
}

matrix3d* zmatrix_lookup(char *name)
{
	int i;
	ZMATRIX_NAMELIST_ENTRY *znep;

	// See if it is in the namelist
	for(i=znamelist_stack_index; i>=0; i--) {
		znep = zmatrix_namelist_stack[i];
		while (znep != NULL) {
			if (!strcmp(znep->name, name)) {
				// make a copy
				return copymat(&(znep->matrix));
			}
			znep= znep->next;
		}
	}
	{
		char buf[256];
		sprintf(buf,"Warning, matrix \"%s\" not defined, using default instead",
			name);
		zWrite(buf);
	}
	return savemat(1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0);  // default if name does not exist
}

int zcolor_alias(int index, char *name) 
{
	ZCOLOR_NAMELIST_ENTRY *znep;

	if (name == NULL) {
		return 0;
	}
	// Store name in namelist
	znep = (ZCOLOR_NAMELIST_ENTRY *) malloc(sizeof(ZCOLOR_NAMELIST_ENTRY));
	znep->name = malloc(strlen(name)+1);
	strcpy(znep->name, name);
	znep->color_value = index;
	znep->next = zcolor_namelist_stack[znamelist_stack_index];
	zcolor_namelist_stack[znamelist_stack_index] = znep;
	return 0;
}

int zpoint_alias(char *name, vector3d *point) 
{
	ZPOINT_NAMELIST_ENTRY *zpep;

	// Store name in namelist
	zpep = (ZPOINT_NAMELIST_ENTRY *) malloc(sizeof(ZPOINT_NAMELIST_ENTRY));
	zpep->name = malloc(strlen(name)+1);
	strcpy(zpep->name, name);
	zpep->point = *point;
	zpep->next = zpoint_namelist_stack[znamelist_stack_index];
	zpoint_namelist_stack[znamelist_stack_index] = zpep;
	return 0;
}

int zmatrix_alias(char *name, matrix3d *matrix) 
{
	ZMATRIX_NAMELIST_ENTRY *zmep;

	if (matrix == NULL) {
		return 0;
	}
	// Store name in namelist
	zmep = (ZMATRIX_NAMELIST_ENTRY *) malloc(sizeof(ZMATRIX_NAMELIST_ENTRY));
	zmep->name = malloc(strlen(name)+1);
	strcpy(zmep->name, name);
	zmep->matrix = *matrix;
	zmep->next = zmatrix_namelist_stack[znamelist_stack_index];
	zmatrix_namelist_stack[znamelist_stack_index] = zmep;
	return 0;
}

int zcolor_modify(int index, char *name, int inverse_index,
				  int p_r, int p_g, int p_b, int p_a,
				  int d_r, int d_g, int d_b, int d_a)
{
	if ((index >= 0)&&(index < ZCOLOR_TABLE_SIZE)) {
		zcolor_copy_table_if_needed();
		zcolor_table[index].inverse_index = inverse_index;
		zcolor_table[index].primary.r = p_r;
		zcolor_table[index].primary.g = p_g;
		zcolor_table[index].primary.b = p_b;
		zcolor_table[index].primary.a = p_a;
		zcolor_table[index].dither.r = d_r;
		zcolor_table[index].dither.g = d_g;
		zcolor_table[index].dither.b = d_b;
		zcolor_table[index].dither.a = d_a;
		zcolor_alias(index,name);
		if (ldraw_commandline_opts.debug_level>0){
			char buf[256];
			sprintf(buf,"Added color index %d name %s",
				index, name);
			zWrite(buf);
		}
	}
	return 0;
}

//
// Returns ldraw edge (i.e. contrasting) color.
// Note that complementing is not commutative.
// c != complement_color(complement_color(c)), for example when c = 7
int edge_color(int c)
{
	if ((c>=0) && (c<ZCOLOR_TABLE_SIZE)) {
		// Pallette Entries
		return zcolor_table[c].inverse_index;
	} else if ((c >= 256)&&(c < 512)) {
		// dithered colors get complement of higher color
		return edge_color( ((c-256)/16) );
	} else if ((c >= 0x4000000)&&(c<=0x7ffffff)) {
		// Numbers of 0x4000000 to 0x7ffffff are hard coded color values. 
		return (0x4000000); // black
	} else {
		// anything else
		return 0;
	}
}

//
// zcp is the primary color
// zcs is the secondary color used for dithering
void translate_color(int c, ZCOLOR *zcp, ZCOLOR *zcs)
{
	if ((c>=0) && (c<ZCOLOR_TABLE_SIZE)) {
		// Pallette Entries
		*zcp = zcolor_table[c].primary;
		*zcs = zcolor_table[c].dither;
	} else if ((c >= 256)&&(c<512)) {
		// Dithered colors
		*zcp = zcolor_table[(c & 0xf0)>>4].primary;
		*zcs = zcolor_table[c & 0xf].primary;
#ifdef USE_OPENGL
		// use colors borrowed from ldview
		if ( c == 334) // Gold
		{
		  zcp->r = 240; 
		  zcp->g = 176;
		  zcp->b = 51;
		}
		else if (( c == 383) // Chrome silver
			 || (c == 494)) // Electrical Contacts
		{
		  zcp->r = 180; //204; // I prefer my plastic chrome a bit darker with 
		  zcp->g = 180; //204; // bright but fuzzy  specular highlights.
		  zcp->b = 180; //204;
		}
		else
		{
		  // No dithering, just average the numbers  
		  zcp->r = (unsigned char) (((int)zcp->r + (int)zcs->r) / 2);
		  zcp->g = (unsigned char) (((int)zcp->g + (int)zcs->g) / 2);
		  zcp->b = (unsigned char) (((int)zcp->b + (int)zcs->b) / 2);
		  zcs = zcp;
		}
#endif
	} else if ((c >= 0x4000000)&&(c<=0x7ffffff)) {
		// Numbers of 0x4000000 to 0x7ffffff are hard coded color values. 
			// Numbers of 0x4000000 to 0x7ffffff are hard coded color values. 
			zcp->r = 16*((c & 0x00000f00) >> 8);
			zcp->g = 16*((c & 0x000000f0) >> 4);
			zcp->b = 16*((c & 0x0000000f) >> 0);
			if (c & 0x1000000) {
				zcp->a = 0x0;
			} else {
				zcp->a = 0xff;
			}
			zcs->r = 16*((c & 0x00f00000) >> 20);
			zcs->g = 16*((c & 0x000f0000) >> 16);
			zcs->b = 16*((c & 0x0000f000) >> 12);
			if (c & 0x2000000) {
				zcs->a = 0x0;
			} else {
				zcs->a = 0xff;
			}
	} else {
		// anything else - solid grey
			zcp->r = 0x7f;
			zcp->g = 0x7f;
			zcp->b = 0x7f;
		    zcp->a = 0xff;
			zcs->r = 0x7f;
			zcs->g = 0x7f;
			zcs->b = 0x7f;
		    zcs->a = 0xff;
	}
}

