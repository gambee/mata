/* buffer.tab.h   
 * 			    
 * Max Gambee
 * Copyright 2017
 *
 *	This file provides the necessary declarations (but no definitions)
 *	of extern/funct/struct etc. so that buffer.h can be used by multiple 
 *	compilation units without duplicating the file with excessive renaming.
 */


/* ===================================== *
 * Section: Defines 					 *
 * ===================================== */

#ifdef BUF_DEBUG
#	ifndef BUF_SIZE
#	define BUF_SIZE 8		//buffer node sizes for debugging
#	endif					//Note: node size should be 16 in x64
#else
#	ifndef BUF_SIZE
#	define BUF_SIZE 1016	//buffer node sizes in performance level code 
#	endif					//Note: node size should be 1024 in x64
#endif

/* ===================================== *
 * Section: Structures 					 *
 * ===================================== */

/* ------------------------------------- * 
 * Structure: BUF_node
 * ------------------------------------- */
struct BUF_node
{
	struct BUF_node* next; //placed in front for struct packing and alignment
	char data[BUF_SIZE];
};

/* ------------------------------------- * 
 * Structure: BUF_buffer
 * ------------------------------------- */
struct BUF_buffer
{
	struct BUF_node *front, *back;
	unsigned int front_index, back_index;
};


extern void BUF_append_node(struct BUF_buffer *buf);
extern void BUF_init(struct BUF_buffer *to_init);
extern int BUF_line_len(struct BUF_buffer *buf);
extern int BUF_len(struct BUF_buffer *buf);
extern int BUF_getc(struct BUF_buffer *buf);
extern int BUF_putc(struct BUF_buffer *buf, char c);
extern int BUF_puts(struct BUF_buffer *buf, char *str);
extern int BUF_print_all(struct BUF_buffer *buf);
