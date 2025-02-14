#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "npu_defines.h"
#include "buddy_allocator.h"

#define NODE_UNUSED 0
#define NODE_USED 1	
#define NODE_SPLIT 2
#define NODE_FULL 3

struct buddy {
	int level; // 7, 2**7 * 32k = 4M
	AL_U8 tree[1];
};

struct buddy * 
buddy_new(int level) {
	int size = 1 << level;
	struct buddy * self = malloc(sizeof(struct buddy) + sizeof(AL_U8) * (size * 2 - 2));
	self->level = level;
	memset(self->tree , NODE_UNUSED , size*2-1);

	return self;
}

AL_VOID
buddy_delete(struct buddy * self) {
	free(self);
}

static inline int
is_pow_of_2(AL_U32 x) {
	return !(x & (x-1));
}

static inline AL_U32
next_pow_of_2(AL_U32 x) {
	if ( is_pow_of_2(x) )
		return x;
	x |= x>>1;
	x |= x>>2;
	x |= x>>4;
	x |= x>>8;
	x |= x>>16;
	return x+1;
}

static inline int
_index_offset(int index, int level, int max_level) {
	return ((index + 1) - (1 << level)) << (max_level - level);
}

static AL_VOID 
_mark_parent(struct buddy * self, int index) {
	for (;;) {
		int buddy = index - 1 + (index & 1) * 2;
		if (buddy > 0 && (self->tree[buddy] == NODE_USED ||	self->tree[buddy] == NODE_FULL)) {
			index = (index + 1) / 2 - 1;
			self->tree[index] = NODE_FULL;
		} else {
			return;
		}
	}
}

int buddy_alloc(struct buddy * self , AL_U32 s) {
	AL_U32 normalized_size = 0 ;
	if (s & FM_SIZE_MASK) {
		normalized_size = (s >> 15) + 1 ;
	} else {
		normalized_size = s >> 15 ;
	}

	int size = 0;
	if (normalized_size==0) {
		size = 1;
	} else {
		size = (int)next_pow_of_2(normalized_size);
	}
	int length = 1 << self->level;

	if (size > length)
		return -1;

	int index = 0;
	int level = 0;

	while (index >= 0) {
		if (size == length) {
			if (self->tree[index] == NODE_UNUSED) {
				self->tree[index] = NODE_USED;
				_mark_parent(self, index);
				return _index_offset(index, level, self->level);
			}
		} else {
			// size < length
			switch (self->tree[index]) {
			case NODE_USED:
			case NODE_FULL:
				break;
			case NODE_UNUSED:
				// split first
				self->tree[index] = NODE_SPLIT;
				self->tree[index*2+1] = NODE_UNUSED;
				self->tree[index*2+2] = NODE_UNUSED;
			default:
				index = index * 2 + 1;
				length /= 2;
				level++;
				continue;
			}
		}
		if (index & 1) {
			++index;
			continue;
		}
		for (;;) {
			level--;
			length *= 2;
			index = (index+1)/2 -1;
			if (index < 0)
				return -1;
			if (index & 1) {
				++index;
				break;
			}
		}
	}

	return -1;
}

static AL_VOID 
_combine(struct buddy * self, int index) {
	for (;;) {
		int buddy = index - 1 + (index & 1) * 2;
		if (buddy < 0 || self->tree[buddy] != NODE_UNUSED) {
			self->tree[index] = NODE_UNUSED;
			while (((index = (index + 1) / 2 - 1) >= 0) &&  self->tree[index] == NODE_FULL){
				self->tree[index] = NODE_SPLIT;
			}
			return;
		}
		index = (index + 1) / 2 - 1;
	}
}

AL_VOID
buddy_free(struct buddy * self, int offset) {
	assert( offset < (1<< self->level));
	int left = 0;
	int length = 1 << self->level;
	int index = 0;

	for (;;) {
		switch (self->tree[index]) {
		case NODE_USED:
			assert(offset == left);
			_combine(self, index);
			return;
		case NODE_UNUSED:
			assert(0);
			return;
		default:
			length /= 2;
			if (offset < left + length) {
				index = index * 2 + 1;
			} else {
				left += length;
				index = index * 2 + 2;
			}
			break;
		}
	}
}

int
buddy_size(struct buddy * self, int offset) {
	assert( offset < (1<< self->level));
	int left = 0;
	int length = 1 << self->level;
	int index = 0;

	for (;;) {
		switch (self->tree[index]) {
		case NODE_USED:
			assert(offset == left);
			return length;
		case NODE_UNUSED:
			assert(0);
			return length;
		default:
			length /= 2;
			if (offset < left + length) {
				index = index * 2 + 1;
			} else {
				left += length;
				index = index * 2 + 2;
			}
			break;
		}
	}
}

static AL_VOID
_dump(struct buddy * self, int index , int level) {
	switch (self->tree[index]) {
	case NODE_UNUSED:
		printf("(%d:%d)", _index_offset(index, level, self->level) , 1 << (self->level - level));
		break;
	case NODE_USED:
		printf("[%d:%d]", _index_offset(index, level, self->level) , 1 << (self->level - level));
		break;
	case NODE_FULL:
		printf("{");
		_dump(self, index * 2 + 1 , level+1);
		_dump(self, index * 2 + 2 , level+1);
		printf("}");
		break;
	default:
		printf("(");
		_dump(self, index * 2 + 1 , level+1);
		_dump(self, index * 2 + 2 , level+1);
		printf(")");
		break;
	}
}

AL_VOID
buddy_dump(struct buddy * self) {
	_dump(self, 0 , 0);
	printf("\n");
}

