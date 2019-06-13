/*****************************************************
 * list.h                                            *
 * by Raymond E. Marcil (06/22/01)                   *
 *****************************************************/

#ifndef CFG_LIST_H
#define CFG_LIST_H

typedef enum _HeadOrTail{
    head = 0,
    tail = 1,
} HeadOrTail;

typedef struct _ListNode {
     void               *data;       /* Pointer to data block */
     struct _ListNode   *next;       /* Pointer to next node */
     struct _ListNode   *prev;       /* Pointer to previous node */
     struct _ListNode   *currentPtr; /* Pointer to current "read" pointer */
                                     /* Only used by the head pointer */
     int                 count;      /* Number of nodes in the list */
     } ListNode;

typedef ListNode *ListNodePtr;

/*
 * Following is the structure for the dummy head
 * of the linked list.  The dummy head doesn't actually
 * store any list data.  It does however have a pointer
 * to the first node in the list (*next), a "read"
 * pointer (*currentPtr), and a count of how many
 * nodes are in the list not including the head node.
 */

typedef struct _ListHead {
     int            count;	     /* Number of nodes in the list */
     ListNodePtr    next;	     /* Pointer to next node */
     ListNodePtr    prev;            /* Pointer to the tail node of the list */
     ListNodePtr    currentPtr;	     /* Current "read" position */
     } ListHead;

typedef ListHead *ListPtr;

/* function prototypes */
extern ListPtr  g_list_create(void);
extern ListPtr  g_list_destroy(ListPtr headNode, void (*destructor)(void *));
extern ListPtr  g_list_add_node(ListPtr headNode, void *data, ...);
extern int      g_list_get_count(ListPtr head);
extern void    *g_list_get(ListPtr headNode);
extern void    *g_list_get_head(ListPtr headNode);
extern void    *g_list_get_tail(ListPtr headNode);
extern void    *g_list_get_next(ListPtr headNode);
extern void    *g_list_get_node(ListPtr headNode, int index);
extern void     g_list_delete_current(ListPtr headNode, void (*destructor)(void *));
extern void     g_list_del_node(ListPtr headNode, int index, void (*destructor)(void *));

#endif   /* CFG_LIST_H */
