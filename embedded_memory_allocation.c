// -------------------------------------------------------------------------------
// Name:        Memory Allocation for Embedded Systems
//
// Author:      Julian Gherghel
// -------------------------------------------------------------------------------
//
// A program to implement a specific memory allocation solution for an embedded
// system
//
// General description
// This program implements memory allocated for two specific structures, named
// structA and structB. The program does not need to allocate memory for any 
// other any other data.

// The memory available for allocation is fixed at 2048 bytes. 

// A function my_alloc() can be called to request memory for a structure or array of 
// structures. The function should return the address of the memory allocated and
// function should indicate when memory cannot be allocated by returning 0.
//
// A function my_free() releases memory previously allocated using my_alloc().
// It does not check whether it is passed a valid address as this should be the 
// responsibility of the user of the function. 

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// portable types
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define int32_t int

#define NUMBER 0xefdcba98	// checked whenever memory is freed using my_free(). 
							// If this value is incorrect, then heap is corrupted.

// the memory available for allocation
uint8_t memory[2048];

// the format of the two structures that can be allocated memory
typedef struct {
  uint32_t index;  // some index
  uint32_t value;  // some value
} structA;

typedef struct {
  uint32_t index;  // some index
  uint32_t red;  // some red value
  uint32_t green;  // some green value
  uint32_t blue;  // some blue value
} structB;

// prototypes of allocation functions
void* my_alloc(uint32_t);
void my_free(void*);

// Try some memory allocation tests
// Arguments: None
// Returns: 0 on completion
int main()
{
	structA *A1, *A10, *A40;
	structB *B1, *B80;

  A1 = (structA *)my_alloc(sizeof(structA));
  A10 = (structA *)my_alloc(10*sizeof(structA));
  A40 = (structA *)my_alloc(40*sizeof(structA));
  my_free((void *)A10);
  B1 = (structB *)my_alloc(sizeof(structB));
  B80 = (structB *)my_alloc(80*sizeof(structB));
  my_free((void *)A1);
  my_free((void *)A40);
  my_free((void *)B1);
  my_free((void *)B80);

  return 0;
}

// Allocate memory in the 'memory' array
// Arguments:
//   (1) the number of bytes required
// Returns: the first address allocated
void* my_alloc(uint32_t bytes)
{
	uint8_t* pt, * pt_end;
	static uint8_t* mL = (uint8_t*)0xffffffff; // lowest address to be returned by malloc()
	static uint8_t* mH;	// highest address (including data) to be returned by malloc()
	bytes = (bytes + 3) & ~3; // ensure alignment for 'NUMBER'
	pt = (uint8_t*)malloc(bytes + 8); // add 2x 32-bit for the memory size and 'NUMBER'
	
	if (pt == NULL)
	{
		abort();	// insufficient memory left
	}
	
	*((uint8_t*)pt) = bytes;	// memorise memory size
	*((uint32_t*)(pt + 4 + bytes)) = NUMBER; // rewrite NUMBER after user's allocation

	if (pt < mL) mL = pt;
	pt_end = pt + bytes + 8;
	if (pt_end > mH) mH = pt_end;
	memory[0] = mH - mL;
	return pt + 4;
}

// Release memory allocated in the 'memory' array
// Arguments:
//   (1) the address of the first memory location to be freed
// Returns: nothing
void my_free(void* start)
{
	uint8_t* pt = (uint8_t*)start - 4;
	int bytes = *((uint32_t*)pt);

	/*check that NUMBER has not changed, therefore that the memory is not corrupted */
	if (*((uint32_t*)(pt + 4 + bytes)) != NUMBER)
	{
		abort();	// data overflow or attempting to free memory already freed
	}
	*((uint32_t*)(pt + 4 + bytes)) = 0;	//remove NUMBER to detect attempts to free already freed memory
	free(pt);
}
