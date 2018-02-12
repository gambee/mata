/* buffer.h   
 * 			    
 * Max Gambee
 * Copyright 2017
 *
 * Description: Buffer data structure for i/o in C.
 */


#ifndef BUFFER_H
#	define BUFFER_H //defend against duplicate includes


/* ===================================== *
 * Section: Includes					 *
 * ===================================== */

#include <stdlib.h>			//needed for memory management 
#include <ctype.h>
#include <stdio.h>


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


/* ===================================== *
 * Section: Functions					 *
 * ===================================== */


/* ------------------------------------- * 
 * Function: BUF_append_node
 * ------------------------------------- */
void BUF_append_node(struct BUF_buffer *buf)
{
	struct BUF_node *tmp;

	tmp = (struct BUF_node*) malloc(sizeof(struct BUF_node));
	tmp->next = NULL;
	buf->back_index = 0;

	buf->back = buf->back ? (buf->back->next = tmp) : tmp;
	buf->front ? 0 : (buf->front = buf->back);
}

/* ------------------------------------- * 
 * Function: BUF_init
 * ------------------------------------- */
void BUF_init(struct BUF_buffer *to_init)
{
	to_init->front_index = 0;
	to_init->back_index = 0;
	to_init->front = NULL;
	to_init->back = NULL;
}

/* ------------------------------------- * 
 * Function: BUF_line_len
 * ------------------------------------- */
int BUF_line_len(struct BUF_buffer *buf)
{
	/* returns number of characters until, but not including, the next
	 * end-of-line character, null-terminating character, or back of stream.
	 * This integer is positive if the end-of-line character is reached, and
	 * negative in the other two cases. It returns zero if the line len is zero
	 * or if buf is a NULL pointer.
	 */

	struct BUF_node *cur;
	int i, count = 0;

	if(!buf)
		return 0; //exit immediately if invalid NULL pointer is passed

	cur = buf->front;
	if(cur == buf->back)
	{
		count = buf->back_index - buf->front_index;
	//	for(i = 0; i < buf->back_index && cur->data[i] != '\n' && cur->data[i]; i++, count++);
	}
	else
	{
		//for(i = buf->front_index; i < BUF_SIZE && cur->data[i] != '\n' && cur->data[i]; i++, count++);
		count = BUF_SIZE + buf->back_index - buf->front_index;
		for(cur=cur->next; cur != buf->back; cur=cur->next)
		{
			count += BUF_SIZE;
			//for(i = 0; i < BUF_SIZE && cur->data[i] != '\n' && cur->data[i]; i++, count++);
		}
		//for(i = 0; i < buf->back_index && cur->data[i] != '\n' && cur->data[i]; i++, count++);

	}
	return count;
	//return (cur->data[i] == '\n') ? count : -count;
}

/* ------------------------------------- * 
 * Function: BUF_getc
 * ------------------------------------- */
int BUF_getc(struct BUF_buffer *buf)
{
	struct BUF_node *tmp;
	int ret = 0;
	
	if(buf && buf->front)
	{
		if(buf->front == buf->back)
		{
			if(buf->front_index < buf->back_index && buf->front_index < BUF_SIZE)
				ret = buf->front->data[buf->front_index++];
		}
		else
		{
			if(buf->front_index < BUF_SIZE)
				ret = buf->front->data[buf->front_index++];
			else
			{
				tmp = buf->front;
				buf->front = buf->front->next;
				buf->front_index = 0;
				ret = buf->front ? buf->front->data[buf->front_index++] : 0;
				free(tmp);
			}
		}
	}

	return ret;
}

/* -------------------------------------------------------------------------- *
 * Function: BUF_putc
 *		Description: Enqueues character 'c' into the buffer pointed to by 'buf'.
 *		Arguments:
 *			buf	: Address of the buffer to enqueue to.
 *			c	: Character to enqueue.
 *		Return Value: int- 
 *			-1		: Failure :: Because buf == NULL.
 *			(int) c	: Success :: Returns the value of the enqueued character
 *								 for convenient use of BUF_putc in expressions.
 * -------------------------------------------------------------------------- */
int BUF_putc(struct BUF_buffer *buf, char c)
{
	/* Returns zero on success, non-zero otherwise. */
	if(!buf)
		return -1; // NULL ptr passed; exit in error immediately

	(!buf->back || (buf->back_index >= BUF_SIZE)) ? BUF_append_node(buf) : 0;
	buf->back->data[buf->back_index++] = c;
	return (int) c;
}

/* ------------------------------------- * 
 * Function: BUF_puts
 * ------------------------------------- */
int BUF_puts(struct BUF_buffer *buf, char *str)
{
	int i = 0;
	char *cur = str;

	if(!buf || !str)
		return -1; //NULL ptr(s) passed, exit in error immediately

	for(i = 0, cur = str; (*cur) ? BUF_putc(buf, *cur) : 0; i++, cur++);

	return i;
}

/* ------------------------------------- * 
 * Function: BUF_print_all
 * ------------------------------------- */
int BUF_print_all(struct BUF_buffer *buf)
{
	struct BUF_node *cur = NULL;
	struct BUF_node *back = NULL;
	int chars = 0;
	int nodes = 0;
	int i;

	if(buf)
	{
		back = buf->back;
		cur = buf->front;
		if(cur && buf->front_index < BUF_SIZE && buf->back_index <= BUF_SIZE)
		{
			if(cur == back)
			{
				printf("Node %d:\n", nodes++);
				for(i = 0; i <= buf->front_index && i < BUF_SIZE; i++)
					printf("\tdata[%d] : %c : %d %s\n", i 
						, isprint(cur->data[i]) ? cur->data[i] : '@'
						, cur->data[i]
						, i == buf->front_index ? "<----Front Element" : "");
			
				for(NULL; i <= buf->back_index && i < BUF_SIZE; i++)
					printf("\tdata[%d] : %c : %d %s\n", i 
						, isprint(cur->data[i]) ? cur->data[i] : '@'
						, cur->data[i]
						, i + 1 == buf->back_index ? "<----Back Element" : "");
				for(NULL; i < BUF_SIZE; i++)
					printf("\tdata[%d] : %c : %d\n", i
					, isprint(cur->data[i]) ? cur->data[i] : '@'
					, cur->data[i]);
			}
			else
			{
				/* front node: */
				printf("Node %d:\n", nodes++);
				for(i = 0; i < BUF_SIZE; i++)
					printf("\tdata[%d] : %c : %d %s\n", i 
						, isprint(cur->data[i]) ? cur->data[i] : '@'
						, cur->data[i]
						, i == buf->front_index ? "<----Front Element" : "");

				/* set up for tail iteration */
				cur = cur->next;
				printf("Node %d:\n", nodes++);

				/* middle nodes: */
				while(cur && cur != buf->back)
				{
					for(i = 0; i < BUF_SIZE; i++)
						printf("\tdata[%d] : %c : %d\n", i
						, isprint(cur->data[i]) ? cur->data[i] : '@'
						, cur->data[i]);
					cur = cur->next;
					printf("Node %d:\n", nodes++);
				}

				/* back node: */
				for(i = 0; i < BUF_SIZE; i++)
					printf("\tdata[%d] : %c : %d %s\n", i 
						, isprint(cur->data[i]) ? cur->data[i] : '@'
						, cur->data[i]
						, i + 1 == buf->back_index ? "<----Back Element" : "");
			}
		}
		else
		{
			return -1;
		}
	}
	else 
	{
		return -2;
	}

	return 0;
}

#endif //ifndef BUFFER_H
