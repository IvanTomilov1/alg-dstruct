#include <stdio.h>
#include "memallocator.h"

#define TRUE 1
#define FALSE 0

typedef struct block {
	struct block* next;
	struct block* prev;
	char is_allocated;
	int size;
} MemBlock;

int memgetminimumsize() {
	return sizeof(MemBlock);// +sizeof(int);
}

int memgetblocksize() {
	return sizeof(MemBlock);
}

MemBlock* startBlock = NULL;
MemBlock* lastBlock = NULL;
int MemFragmentSize = 0;
int MemAllocated = 0;

int meminit(void* pMemory, int size) {
	if ((size < memgetminimumsize()) || (pMemory == NULL)) return -1;

	startBlock = (MemBlock*)pMemory;

	startBlock->prev = NULL;
	startBlock->next = NULL;
	startBlock->is_allocated = FALSE;
	startBlock->size = size - memgetblocksize();

	lastBlock = startBlock;
	MemFragmentSize = size;

	return 0;
}

void* memalloc(int size) {

	MemBlock* iterBlock = startBlock;
	MemBlock* suitBlock = NULL;

    while (iterBlock != NULL) {
		if ((!iterBlock->is_allocated) && (iterBlock->size >= size)) {
			suitBlock = iterBlock;
			break;
		}
		iterBlock = iterBlock->next;
	}

	if (suitBlock == NULL) return NULL;

	if (suitBlock->size > size + memgetblocksize()) {
		iterBlock = (MemBlock*)((char*)suitBlock + memgetblocksize() + size);
		
		iterBlock->is_allocated = FALSE;
		iterBlock->prev = suitBlock;
		iterBlock->next = suitBlock->next;
		iterBlock->size = suitBlock->size - (size + memgetblocksize());
		if (iterBlock->next != NULL) (iterBlock->next)->prev = iterBlock;

		suitBlock->next = iterBlock;
		suitBlock->size = size;
	}

	lastBlock = suitBlock;
	lastBlock->is_allocated = TRUE;
	MemAllocated += (memgetblocksize() + size);

	return (void*)((char*)lastBlock + memgetblocksize());
}

void memfree(void* p) {
	if (p == NULL) return;

	MemBlock* delBlock = (MemBlock*)((char*)p - memgetblocksize());

	if (!delBlock->is_allocated) return;

	MemAllocated -= (memgetblocksize() + delBlock->size);

	delBlock->is_allocated = FALSE;

	if (delBlock->next != NULL){
		if (!(delBlock->next)->is_allocated) {
			delBlock->size += (delBlock->next)->size + memgetblocksize();
			delBlock->next = (delBlock->next)->next;
			if (delBlock->next != NULL) (delBlock->next)->prev = delBlock;
		}
	}

	if (delBlock->prev != NULL){
		if (!(delBlock->prev)->is_allocated) {
			(delBlock->prev)->size += delBlock->size + memgetblocksize();
			(delBlock->prev)->next = delBlock->next;
			if (delBlock->next != NULL) (delBlock->next)->prev = delBlock->prev;
			delBlock = delBlock->prev;
		}
	}
}

void memdone() {
	int checkAllocatedSize = 0;
	MemBlock* currBlock = startBlock;
	while (currBlock->next != NULL) {
		checkAllocatedSize += memgetblocksize() + currBlock->size;
		currBlock = currBlock->next;
	}
	if (checkAllocatedSize != MemFragmentSize) fprintf(stderr, "ERROR: Memory leak");
}