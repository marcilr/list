/*****************************************************
 * list.c                                            *
 * by Raymond E. Marcil (06/22/01)                   *
 *****************************************************/

/*
 * This is a generic doubly-linked list ADT written
 * to reduce the amount of duplicated code used
 * in the handling of linked lists.
 *
 * Created Fri Jun 22 11:17:17 AKDT 2001
 * by Raymond E. Marcil (marcilr@rockhounding.net)
 *
 * Last Updated: Tue Jul 17 09:59:53 AKDT 2001
 *   Added g_list_add_node_at_tail() function.
 *   Updated g_list_create() and g_list_delete_current()
 *   to update tail pointer.
 *
 * NOTES:   Please feel free to add ADT functions are required.
 *          This ADT is not completely flushed out and tested.
 *
 *          This ADT intentionally does not use any libraries other
 *          than stdio and stdlib, to make it as portable as possible.
 *          Yes, all free's are checked.
 *
 * FIXME:   We should rename g_list_node_add_node_at_tail() to
 *          g_list_add_node() and change all calls to g_list_add_node
 *          to contain the head/tail insertion parameter.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "List.h"

/*
 * g_list_create(void);
 * This function creates a new list and
 * returns a pointer to the dummy head of
 * the list.
 *
 * INPUT:
 * RETURNS:     ListPtr         Pointer to head of new list.
 */

ListPtr g_list_create(void) {
    ListPtr newHead;

    if ((newHead = (ListPtr) malloc (sizeof(ListHead))) != NULL) {
        newHead->count = 0;                         /* Initialize count */
        newHead->next = NULL;                       /* Initialize next */
        newHead->prev = NULL;                       /* Initialize tail */
        newHead->currentPtr = NULL;                 /* Initialize current pointer */
    }

  return newHead;
} /* g_list_create(void) */

/*
 * g_list_destroy()
 * This function walks the list and frees up the list's nodes.
 * Since the ADT does not know what is in the data container
 * it requires a destructor function which knows how to free the
 * data pointer.
 *
 * INPUT:       headNode        Pointer to list to destroy.
 *              destructor()    Pointer to function that knows how to
 *                              destroy the data section.
 * RETURNS:     ListPtr = NULL  List has been destroyed.
 */

ListPtr g_list_destroy(ListPtr headNode, void (*destructor)(void *)) {
    ListNodePtr tmp;

    if (headNode != NULL){
        tmp = headNode->next;
        while (tmp != NULL){
            if (tmp->prev != NULL){                 /* If previous node not NULL, ie not headNode */
                destructor(tmp->prev->data);        /* Destroy data container */
                free(tmp->prev);                    /* Free previous node */
                }
            tmp = tmp->next;
        }
        free(headNode);                             /* Free up headNode */
        headNode = NULL;                            /* Set to NULL in case free doesn't */
    }
    return headNode;
} /* g_list_destroy(ListPtr headNode, void (*destructor)(void *)) */

#if 0

/*
 * g_list_add_node()
 * This function adds a node to the linked list.
 * The new node is always inserted after the dummy head.
 *
 * INPUT:       headNode        Pointer to the head of the list.
 *              data            Pointer to data section.
 * RETURNS:     ListPtr         Pointer to head of list on success.
 *              NULL            Add node failed.
 */

ListPtr g_list_add_node(ListPtr headNode, void *data) {
    ListNodePtr newNode;

    /* printf("%s\n",__FUNCTION__); */

	if (headNode != NULL) {

        /* Create a new node and fill in data block */

    	if ((newNode = (ListNodePtr) malloc(sizeof(ListNode))) !=NULL) {
            newNode->data = data;

            /* If this is the first node inserted into the list
               set the "read" (currentPtr) to it */
            if (headNode->next == NULL)
                headNode->currentPtr = newNode;

            /* Insert newNode after dummy head of list */

            newNode->next = headNode->next;             /* Configure next pointers */
            headNode->next = newNode;

            /* Configure backpointers of newNode and NextNode */

            newNode->prev = NULL;                       /* Previous is dummy head, so set to NULL */
            if (newNode->next)
                newNode->next->prev = newNode;

            /* Increment node count in the headNode */

            headNode->count++;

            }
        return headNode;
        }
    else
        return NULL;
} /* g_list_add_node(ListPtr headNode, void *data) */

#endif

/*
 * g_list_add_node()
 * This function was written at the request of Jason
 * which needed a list add node function that could
 * do tail insertion as well as head insertion.
 * "..." argument was used to add head or tail argument
 * without breaking previous use of g_list_add_node().
 * 
 * INPUT:       headNode        Pointer to head of the list.
 *              data            Pointer to data structure.
 *              pos             head or tail, determines which end of
 *                              the list to add the new node to.
 * RETURNS:     ListPtr         Add node succeeded.
 *              NULL            Adde node failed.
 *
 * NOTES:       This should probably replace g_list_add_node()
 *              at some point.
 */


ListPtr g_list_add_node(ListPtr headNode, void *data, ...){
    ListPtr         result = NULL;
    ListNodePtr     newNode;

    HeadOrTail      pos;
    va_list         ap;

    /* printf("%s\n",__FUNCTION__); */

    /* Get head or tail argument if present*/
    va_start(ap,data);

    if ((pos = va_arg(ap, HeadOrTail)) != tail)             /* Get head or tail argument of present */
      pos = head;                                           /* Default to head insertion */

	if (headNode != NULL) {

        /* Create a new node and fill in data block */

    	if ((newNode = (ListNodePtr) malloc(sizeof(ListNode))) !=NULL) {
            newNode->data = data;

             /* Always set currentPtr to newly added not */
             headNode->currentPtr = newNode;

            /* Check if head or tail insertion */
            if (pos == tail){

                /* Insert at tail of list */

                newNode->next = NULL;                       /* Set next to NULL since end of list */
                newNode->prev = headNode->prev;             /* Set prev pointer to old tail (could be NULL) */
                if (headNode->prev != NULL)                 /* Set old tails next pointer */
                    headNode->prev->next=newNode;
                headNode->prev = newNode;                   /* Save new tail */
                if (headNode->next == NULL)                 /* If inserting 1st node */
                  headNode->next = newNode;                 /* Point head next to newNode */

            }

            else { /* Default to head insertion */

                newNode->next = headNode->next;             /* Configure next pointers (could be NULL */
                newNode->prev = NULL;                       /* Previous is dummy head, so set to NULL */
                if (newNode->next)
                    newNode->next->prev = newNode;
                headNode->next = newNode;
                if (headNode->prev == NULL)                 /* If inserting 1st node */
                    headNode->prev = newNode;               /* Set tail pointer to newNode */

            } /* if (pos == tail) */

            headNode->count++;                              /* Increment node count in the headNode */
            result = headNode;                              /* Return success */

        } /* if ((newNode = (ListNodePtr) malloc(sizeof(ListNode))) !=NULL) */

    } /* if (pos == tail) */

    return (result);

} /* ListPtr g_list_add_node_at_tail(ListPtr headNode, void *data, HeadOrTail pos) */



/*
 * list_get_count()
 * This simple function returns the number of nodes in the list,
 * which could be zero if the list was not initialized.
 *
 * INPUT:       head            Pointer to the head of the list
 * RETURNS:                     Number of nodes in the list, not including dummy head.
 */

int g_list_get_count(ListPtr head) {
    if (head != NULL)
        return head->count;
    else
        return 0;
} /* g_list_get_count(ListPtr head) */

/*
 * g_list_get()
 * This function returns a pointer to the current data section, but
 * does not advance the "read" pointer to the next node.
 *
 * INPUT:       headNode        Pointer to dummy head of the list.
 * RETURNS:                     Pointer to data of the current node.
 */

void *g_list_get(ListPtr headNode) {
    if (headNode) {
        if (headNode->currentPtr)
            return headNode->currentPtr->data;
    }

    if (headNode->currentPtr->next != NULL) printf("Next !=NULL=n");
    return NULL;
} /* g_list_get(ListPtr headNode) */

/*
 * g_list_get_head()
 * This function sets the currentPtr, like a read pointer,
 * to NULL, indicating to other functions to read from the first node.
 * Also, a pointer to the data section of the first node is returned.
 *
 * INPUT:       headNode        Pointer to the head of the list.
 * RETURNS:                     Pointer to data of the first node in the list.
 */
void *g_list_get_head(ListPtr headNode) {
    if (headNode) {
        headNode->currentPtr = NULL;
        if (headNode->next)
            return headNode->next->data;
    }
    return NULL;
} /* g_list_get_head(ListPtr headNode) */

/*
 * g_list_get_tail()
 * This function sets the CurrentPtr, like a read pointer,
 * to last node in the list.  A pointer to the data section
 * of the last node is returned.
 * 
 * INPUT:       headNode        Pointer to the dummy head of the list.
 * RETURNS:     Pointer to data section of the tail of the list.
 */
void *g_list_get_tail(ListPtr headNode){
    if (headNode){
        headNode->currentPtr = headNode->prev;
        if (headNode->currentPtr)
            return headNode->currentPtr->data;
    }
    return NULL;
}

/*
 * g_list_get_next()
 * This function sets the currentPtr to the next node and returns the
 * a pointer to the data container of the next node.  If the data pointer
 * is NULL or the end of the list is reached NULL is returned.
 *
 * INPUT:       headNode        Pointer to dummy head of the list
 * RETURNS:                     Pointer to data of the next node.
 */
void *g_list_get_next(ListPtr headNode) {
    ListNodePtr tmp;

    if (headNode) {

        /* If headNode->currentPtr is NULL, then return first node's data
           This allows us to just call list_get_head(), then repeated call
           to list_get_next() to traverse the list */

        if (headNode->currentPtr == NULL) {
            if (headNode->next) {
                headNode->currentPtr = headNode->next;
                return headNode->next->data;
                }
            }

        else if (headNode->currentPtr->next) {
            tmp = headNode->currentPtr->next;
            headNode->currentPtr = tmp;
            return headNode->currentPtr->data;
        }
    }
    return NULL;
} /* g_list_get_next(ListPtr headNode) */

/*
 * g_list_get_node()
 * This function accesses a list like it is an array.
 * A pointer to the data record of the specified index
 * is returned.
 *
 * INPUT:       headNode        Pointer to dummy head of the list
 *              index           index of data node requested.
 * RETURNS:                     Pointer to data of the requested node, or NULL if no such node.
*/
void *g_list_get_node(ListPtr headNode, int index) {
    int current = 0;
    void *data = NULL;

    data = g_list_get_head(headNode);
    while ((data = g_list_get_next(headNode)) && (current < index )) current++;
    if (index != current) data = NULL;

    return data;
} /* g_list_get_node(ListPtr headNode, int index) */

/*
 * list_delete_current()
 * This function deletes the current node from within the
 * doubly-linked list, ie when a node is read it can
 * be deleted.
 *
 * INPUT:       headNode        Pointer the dummy head of the list
 *              destructor      Pointer to function that knows how to
 *                              deallocate the data container.
 */
void g_list_delete_current(ListPtr headNode, void (*destructor)(void *)) {
    ListNodePtr tmp;

    if (headNode->currentPtr != NULL) {                 /* Remove node from the list */

        /* Set previous node's next pointer to next node */
        if (headNode->currentPtr->prev != NULL)
            headNode->currentPtr->prev->next = headNode->currentPtr->next;
        else /* Deleting 1st data node, so reset head node's next pointer (cound be NULL) */
            headNode->next = headNode->currentPtr->next;

        /* Check if we're at the end of the list */
        if (headNode->currentPtr->next !=NULL ){
            /* No, set next node's previous pointer to previous node */
            headNode->currentPtr->next->prev = headNode->currentPtr->prev;

            /* New currentPtr will be next node */
            tmp = headNode->currentPtr->next;
            }
        else { /* Yes, we're deleting the last node in the list */
            /* Reset head node's tail pointer to previous node (could be NULL) */
            headNode->prev = headNode->currentPtr->prev;

            /* New currentPtr will be previous node */
            tmp = headNode->currentPtr->prev;
            }

        free(headNode->currentPtr);                     /* Free currentPtr node */
        headNode->currentPtr = tmp;                     /* Save new currentPtr */
        headNode->count--;                              /* reduce the number of nodes remaining */

    } /* if (headNode->currentPtr == NULL) */

} /* g_list_delete_current(ListPtr headNode, void (*destructor)(void *)) */

/*
 * g_list_del_node()
 * This function deletes the specified node from within the
 * doubly-linked list.
 *
 * INPUT:       headNode        Pointer the dummy head of the list
 *              index           index of data node to delete.
 *              destructor      Pointer to function that knows how to 
 *                              deallocate the data container.
 *  USES:       g_list_get_node()
 *              g_list_delete_current()
 *  RETURNS:    NONE
 */
void g_list_del_node(ListPtr headNode, int index, void (*destructor)(void *)) {

    /* Check if index within bounds */
    if ((index >= 0) && (index < g_list_get_count(headNode))){
        g_list_get_node(headNode,index);                    /* Get current node */
        g_list_delete_current(headNode, destructor);        /* Remove node from the list */
    }

} /* g_list_del_node(ListPtr headNode, int index, void (*destructor)(void *)) */
