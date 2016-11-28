#ifndef _CYCLE_LIST_H__
#define _CYCLE_LIST_H__



struct ko_list_head {
	struct ko_list_head *next, *prev;
};


static inline void KO_INIT_LIST_HEAD(struct ko_list_head *list)
{
	list->next = list;
	list->prev = list;
}

#define KO_LIST_HEAD_INIT(name) { &(name), &(name) }
#define KO_LIST_HEAD(name) \
	struct ko_list_head name = KO_LIST_HEAD_INIT(name)


static inline void __ko_list_add(struct ko_list_head *new_t,
								struct ko_list_head *prev,
								struct ko_list_head *next)
{
	next->prev = new_t;
	new_t->next = next;
	new_t->prev = prev;
	prev->next = new_t;
}


static inline void ko_list_add(struct ko_list_head *new_t, struct ko_list_head *head)
{
	__ko_list_add(new_t, head, head->next);
}

static inline void ko_list_add_tail(struct ko_list_head *new_t, struct ko_list_head *head)
{
    __ko_list_add(new_t, head->prev, head);
}

#define ko_list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)


static inline void __ko_list_del(struct ko_list_head * prev, struct ko_list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void __ko_list_del_entry(struct ko_list_head *entry)
{
	__ko_list_del(entry->prev, entry->next);
}

static inline void ko_list_del(struct ko_list_head *entry)
{
	__ko_list_del(entry->prev, entry->next);
	entry->next = entry;
	entry->prev = entry;
}

static inline struct ko_list_head* ko_list_del_next(struct ko_list_head *entry)
{
    struct ko_list_head             *next;
    if (entry->next == entry) {
        return 0;
    }

    next = entry->next;
    ko_list_del(next);

    return next;
}

static inline int ko_list_iterator(struct ko_list_head *entry, struct ko_list_head **iterator)
{
    *iterator = 0;
    if (entry->next == entry) {
        return -1;
    }

    *iterator = entry->next;

    ko_list_del(*iterator);
    ko_list_add_tail(*iterator, entry);

    return 0;
}





#define offsetof(type, member) (size_t)&(((type*)0)->member)
#define container_of(ptr, type, member) ({                      \
         const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#endif