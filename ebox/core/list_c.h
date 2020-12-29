#ifndef __LIST_C_H
#define __LIST_C_H
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "ebox_type.h"
#define LIST_POISON1  ((void *) 0)
#define LIST_POISON2  ((void *) 0)
struct list_head {struct list_head *next, *prev;};
struct slist {struct slist *next;};

typedef struct list_head list_head_t ;
typedef struct slist slist_t ;
//#define offsetofs(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

//#define container_of(ptr, type, member) ({                      \
//    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
//    (type *)( (char *)__mptr - offsetofs(type,member) );})


// 已知一个结构的成员，获取该成员所属结构体地址
#define container_of(ptr, type, menber) \
    (type *)((char*)ptr - (char*) &(((type *)0)->menber))
        
    
#define LIST_HEAD_INIT(name) { &(name), &(name) }
 
// 新建一个list head， 初始化其指针指向自身   
#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)
            
 
    
static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next= list;
	list->prev = list;
}

// 将新节点插入到指定两个节点之间
static inline void __list_add(struct list_head *new_,
        struct list_head *prev,
        struct list_head *next)
{
    next->prev = new_;
    new_->next = next;
    new_->prev = prev;
    prev->next = new_;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */// 将新节点插入到链表头
static inline void list_add(struct list_head *new_, struct list_head *head)
{
    __list_add(new_, head, head->next);
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */// 将新节点插入到链表尾
static inline void list_add_tail(struct list_head *new_, struct list_head *head)
{
    __list_add(new_, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
//prev和next两个链表指针构成双向链表关系
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next =  next;
}
/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
#ifndef CONFIG_DEBUG_LIST
static inline void __list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = (struct list_head*)LIST_POISON1;
    entry->prev = (struct list_head*)LIST_POISON2;
}
#else
extern void __list_del_entry(struct list_head *entry);
extern void list_del(struct list_head *entry);
#endif


/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(struct list_head *entry)
{
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}


/**
 * list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 *///调整指针，使new的前后指针指向old的前后，反过来old的前后指针也指向new
//就是双向链表的指针调整
static inline void list_replace(struct list_head *old,
                                struct list_head *new_)
{
	new_->next = old->next;
	new_->next->prev = new_;
	new_->prev = old->prev;
	new_->prev->next = new_;
}
static inline void list_replace_init(struct list_head *old,
                    struct list_head *new_)
{
    list_replace(old, new_);
    INIT_LIST_HEAD(old);
}


        
/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 *///移动指定元素list到链表的头部
static inline void list_move(struct list_head *list, struct list_head *head)
{
	__list_del(list->prev, list->next);//删除list元素
	list_add(list, head);//将list节点添加到head头节点
}
 
/**
* list_move_tail - delete from one list and add as another's tail
* @list: the entry to move
* @head: the head that will follow our entry
*/
//移动指定元素list到链表的尾部
static inline void list_move_tail(struct list_head *list,
                                  struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}


/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int list_is_last(const struct list_head *list,
                const struct list_head *head)
{
    return list->next == head;
}


/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}


static inline void __list_splice(const struct list_head *list,
                 struct list_head *prev,
                 struct list_head *next)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;


    first->prev = prev;
    prev->next = first;


    last->next = next;
    next->prev = last;
}


/**
 * list_splice - join two lists, this is designed for stacks
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice(const struct list_head *list,
                struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head, head->next);
}


/**
 * list_splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice_tail(struct list_head *list,
                struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head->prev, head);
}


/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(struct list_head *list,
                    struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head, head->next);
        INIT_LIST_HEAD(list);
    }
}


/**
 * list_splice_tail_init - join two lists and reinitialise the emptied list
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
static inline void list_splice_tail_init(struct list_head *list,
                     struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head->prev, head);
        INIT_LIST_HEAD(list);
    }
}

static inline int list_len(struct list_head *list)
{
    int i = 0; 
    struct list_head *head = list;
    while(head->next != list)
    {
        head = head->next;
        i++;
    }
    return i;
}





//获取type类型结构体的起始指针
#define list_entry(ptr, type, menber) \
    container_of(ptr, type, menber)
    
/**
 * list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)
    
    
    
// 正序遍历宏， 
//从head节点开始（不包括head节点！）遍历它的每一个节点！
#define list_for_each(pos, head) \
    for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)
    
    
// 逆序遍历宏
//它也是从head节点开始（不包括head节点）向前遍历每一个节点！即从链表的尾部开始遍历！
#define list_for_each_prev(pos, head) \
    for ((pos) = (head)->prev; (pos) != (head); (pos) = (pos)->prev)
    
    
/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:    the &struct list_head to use as a loop cursor.
 * @n:      another &struct list_head to use as temporary storage
 * @head:   the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

/**
 * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
 * @pos:    the &struct list_head to use as a loop cursor.
 * @n:      another &struct list_head to use as temporary storage
 * @head:   the head for your list.
 */
#define list_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev; \
         pos != (head); \
         pos = n, n = pos->prev)
         
         
/**
 * list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)              \
    for (pos = list_entry((head)->next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = list_entry(pos->member.next, typeof(*pos), member))


/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

    
    
    

#define SLIST_HEAD_INIT(name) { NULL }
 
// 新建一个slist head， 初始化其指针指向NULL   
#define SLIST_HEAD(name) \
	struct slist_head name = SLIST_HEAD_INIT(name)
            
 
    
static inline void INIT_SLIST_HEAD(struct list_head *list)
{
	list->next= NULL;
}

static inline void slist_append(slist_t *l, slist_t *n)
{
    struct slist *node;

    node = l;
    while (node->next) node = node->next;

    /* append the node to the tail */
    node->next = n;
    n->next = NULL;
}

static inline void slist_insert(slist_t *l, slist_t *n)
{
    n->next = l->next;
    l->next = n;
}

static inline unsigned int slist_len(const slist_t *l)
{
    unsigned int len = 0;
    const slist_t *list = l->next;
    while (list != NULL)
    {
        list = list->next;
        len ++;
    }

    return len;
}

static inline slist_t *slist_del(slist_t *l, slist_t *n)
{
    /* remove slist head */
    struct slist *node = l;
    while (node->next && node->next != n) node = node->next;

    /* remove node */
    if (node->next != (slist_t *)0) node->next = node->next->next;

    return l;
}

static inline slist_t *slist_first(slist_t *l)
{
    return l->next;
}

static inline slist_t *slist_tail(slist_t *l)
{
    while (l->next) l = l->next;

    return l;
}

static inline slist_t *slist_next(slist_t *n)
{
    return n->next;
}

static inline int slist_isempty(slist_t *l)
{
    return l->next == NULL;
}


/**
 * @brief get the struct for this single list node
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define slist_entry(node, type, member) \
    container_of(node, type, member)


/**
 * slist_for_each - iterate over a single list
 * @pos:    the slist_t * to use as a loop cursor.
 * @head:   the head for your single list.
 */
#define slist_for_each(pos, head) \
    for (pos = (head)->next; pos != NULL; pos = pos->next)

/**
 * slist_for_each_entry  -   iterate over single list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your single list.
 * @member: the name of the list_struct within the struct.
 */
#define slist_for_each_entry(pos, head, member) \
    for (pos = slist_entry((head)->next, typeof(*pos), member); \
         &pos->member != (RT_NULL); \
         pos = slist_entry(pos->member.next, typeof(*pos), member))

/**
 * slist_first_entry - get the first element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define slist_first_entry(ptr, type, member) \
    slist_entry((ptr)->next, type, member)

/**
 * slist_tail_entry - get the tail element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define slist_tail_entry(ptr, type, member) \
    slist_entry(slist_tail(ptr), type, member)

#ifdef __cplusplus
}
#endif // __cplusplus    
#endif
