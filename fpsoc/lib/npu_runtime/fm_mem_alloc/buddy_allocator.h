#ifndef BUDDY_MEMORY_ALLOCATION_H
#define BUDDY_MEMORY_ALLOCATION_H

struct buddy;

struct buddy * buddy_new(int level) ;
AL_VOID buddy_delete(struct buddy *);
int buddy_alloc(struct buddy * self, AL_U32 s);
AL_VOID buddy_free(struct buddy *, int offset);
int buddy_size(struct buddy *, int offset);
AL_VOID buddy_dump(struct buddy *);

#endif
