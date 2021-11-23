#include <stdio.h>
#include "memallocator.h"

typedef struct MemBlock {
	struct MemBlock* next;
	struct MemBlock* prev;
	int size;
} MemBlock;

int memgetminimumsize() {
	return sizeof(MemBlock) + sizeof(int);
}

int memgetblocksize() {
	return sizeof(MemBlock);
}

MemBlock* startBlock = NULL;
MemBlock* lastBlock = NULL;
int MemFragmentSize = 0;
int MemAllocated = 0;

int meminit(void* pMemory, int size) {
	if (size < memgetminimumsize() || !pMemory) return -1;

	startBlock = (MemBlock*)pMemory;
	
	startBlock->prev = NULL;
	startBlock->next = NULL;
	startBlock->size = size - memgetblocksize();

	lastBlock = startBlock;
	MemFragmentSize = size;

	return 0;
}

void* memalloc(int size) {
	
	MemBlock* newlastBlock = NULL;

	if (!startBlock || (MemAllocated + (memgetblocksize() + size) > MemFragmentSize)) return NULL;
	
	if (MemAllocated == 0) {
		lastBlock->size = size;
	}
	else {
		lastBlock->next = (MemBlock*)((char*)lastBlock + memgetblocksize() + lastBlock->size);
		newlastBlock = lastBlock->next;

		newlastBlock->prev = lastBlock;
		newlastBlock->next = NULL;
		newlastBlock->size = size;

		lastBlock = newlastBlock;
	}
	MemAllocated += (memgetblocksize() + size);

	return (void*)((char*)lastBlock + memgetblocksize());
}

void memfree(void* p) {
	if (MemAllocated > 0) {
		MemAllocated -= (memgetblocksize() + lastBlock->size);
		lastBlock = lastBlock->prev;
	}
}

void memdone() {
	int checkAllocatedSize = 0;
	MemBlock* currBlock = startBlock;
	while (currBlock != NULL) {
		checkAllocatedSize += (memgetblocksize() + currBlock->size);
		currBlock = currBlock->next;
		if (checkAllocatedSize > MemFragmentSize) { 
			fprintf(stderr, "Error! Memory leak detected\n");
			break; 
		}
	}
}