#ifndef _LIST_H_
#define _LIST_H_

typedef struct list_node_s{
	struct list_node_s * prev;
	struct list_node_s * next;
}list_node_t, list_t;

#define container_of(ptr, type, member)		((type *)((char *)(ptr) - (unsigned long)(&(((type *)0)->member))))

#define list_entry(ptr, type, member)  container_of(ptr, type, member)

static inline void list_init(list_t * l)
{
	l->prev = l->next = l;
}

static inline int list_isempty(list_t * l)
{
	return l->next == l;
}

static inline int list_len(list_t * l)
{
	int len = 0;
	list_node_t * p = l;
	while(p->next != l){
		p = p->next;
		len++;
	}

	return len;
}

static inline void _list_add(list_node_t * _new, list_node_t * prev, list_node_t * next)
{
	_new->prev = prev;
	_new->next = next;

	next->prev = _new;
	prev->next = _new;
}

static inline void list_add_tail(list_t * l, list_node_t * n)
{
	_list_add(n, l->prev, l);

	/*
	l->prev->next = n;
	n->prev = l->prev;

	l->prev = n;
	n->next = l;
	*/
}

static inline void list_add_head(list_t * l, list_node_t * n)
{
	_list_add(n, l, l->next);

	/*
	l->next->prev = n;
	n->next = l->next;

	l->next = n;
	n->prev = l;
	*/
}

static inline void _list_del(list_node_t * prev, list_node_t * next)
{
	prev->next = next;
	next->prev = prev;
}

static inline void list_del(list_t * l, list_node_t * n)
{
	_list_del(n->prev, n->next);
	n->prev = n->next = n;
}

#define list_first(l)	((l)->next == (l)? NULL : (l)->next)
#define list_last(l)	((l)->prev == (l)? NULL : (l)->prev)
#define list_first_entry(l, type, member)	((l)->next == (l)? NULL : list_entry((l)->next, type, member))
#define list_last_entry(l, type, member)	((l)->prev == (l)? NULL : list_entry((l)->prev, type, member))

/**
 * iterate over a list
 * @p:	the loop cursor in type of (list_node_t *).
 * @l:	the list in type of (list_t *).
 */
#define list_for_each(p, l)	\
	for(p = (l)->next; p != (l); p = p->next)


/**
 * safe version of list_for_each(p,l)  against deleting operation
 * @p:	the loop cursor in type of (list_node_t *).
 * @n:	a temp storage in type of (list_node_t *).
 * @l:	the list in type of (list_t *).
 */
#define list_for_each_safe(p, n, l)	\
	for(p = (l)->next, n = p->next;  p != (l); p = n, n = p->next) 

/**
 * iterate over a list of given type 
 * @p:	the loop cursor in type of (type *).
 * @l:	the list in type of (list_t *).
 * @member: the name of the list_t member within the given type.
 */
#define list_for_each_entry(p, l, member) \
	for(p = list_entry((l)->next, typeof(*p), member);	\
			&(p->member) != (l);	\
			p = list_entry(p->member.next; typeof(*p), member))

/**
 * safe version of list_for_each_entry(p,l, member)  against deleting operation
 * @p:	the loop cursor in type of (type *).
 * @n:	a temp storage in type of (type *).
 * @l:	the list in type of (list_t *).
 * @member: the name of the list_t member within the given type.
 */
#define list_for_each_entry_safe(p, n, l, member) \
	for(p = list_entry((l)->next, typeof(*p), member),	\
			n = list_entry(p->member.next, typeof(*p), member);	\
			&(p->member) != (l);	\
			p = n, n = list_entry(n->member.next; typeof(*p), member))

typedef struct slist_node_s{
	struct slist_node_s * next;
}slist_node_t, slist_t;

#define slist_entry(ptr, type, member)  container_of(ptr, type, member)

static inline void slist_init(slist_t * l)
{
	l->next = NULL;
}

static inline int slist_isempty(slist_t * l)
{
	return l->next == NULL;
}

static inline int slist_len(slist_t * l)
{
	int len = 0;
	slist_node_t * p = l;
	while(p->next){
		p = p->next;
		len++;
	}

	return len;
}

static inline void slist_add_tail(slist_t * l, slist_node_t * n)
{
	slist_node_t * p = l;
	while(p->next){
		p = p->next;
	}

	p->next = n;
	n->next = NULL;
}

static inline void slist_add_head(slist_t * l, slist_node_t * n)
{
	n->next = l->next;
	l->next = n;
}

static inline void slist_del(slist_t * l, slist_node_t * n)
{
	slist_node_t * p = l;
	while(p->next && p->next != n){
		p = p->next;
	}

	if(p->next){
		p->next = n->next;
		n->next = NULL;
	}
}


#define slist_first(l)	((l)->next)
#define slist_last(l)	({slist_node_t * node = (l); whlie(node->next){node = node->next}; node == (l)? NULL:node;})	// FIXME: note this usage of macro
#define slist_first_entry(l, type, member)	((l)->next == NULL? NULL : slist_entry((l)->next, type, member))
#define slist_last_entry(l, type, member)	({slist_node_t * node = (l); whlie(node->next){node = node->next}; node == (l)?NULL: slist_entry(node, type, member);})

/**
 * iterate over a slist
 * @p:	the loop cursor in type of (slist_node_t *).
 * @l:	the slist in type of (slist_t *).
 */
#define slist_for_each(p, l)	\
	for(p = (l)->next; p != (NULL); p = p->next)

/**
 * safe version of slist_for_each(p,l)  against deleting operation
 * @p:	the loop cursor in type of (slist_node_t *).
 * @n:	a temp storage in type of (slist_node_t *).
 * @l:	the slist in type of (slist_t *).
 */
#define slist_for_each_safe(p, n, l)	\
	for(p = (l)->next, n = p->next;  p != (NULL); p = n, n = p->next) 

/**
 * iterate over a slist of given type 
 * @p:	the loop cursor in type of (type *).
 * @l:	the slist in type of (slist_t *).
 * @member: the name of the slist_t member within the given type.
 */
#define slist_for_each_entry(p, l, member) \
	for(p = slist_entry((l)->next, typeof(*p), member);	\
			&(p->member) != (NULL);	\
			p = slist_entry(p->member.next; typeof(*p), member))

/**
 * safe version of slist_for_each_entry(p,l, member)  against deleting operation
 * @p:	the loop cursor in type of (type *).
 * @n:	a temp storage in type of (type *).
 * @l:	the slist in type of (slist_t *).
 * @member: the name of the slist_t member within the given type.
 */
#define slist_for_each_entry_safe(p, n, l, member) \
	for(p = slist_entry((l)->next, typeof(*p), member),	\
			n = slist_entry(p->member.next, typeof(*p), member);	\
			&(p->member) != (NULL);	\
			p = n, n = slist_entry(n->member.next; typeof(*p), member))
						
#endif
