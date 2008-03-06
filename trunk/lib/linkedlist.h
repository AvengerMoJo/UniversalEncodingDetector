#ifndef __UED__LINKEDLIST_H__
#define __UED__LINKEDLIST_H__

/*
   (c) Copyright 2007  AvengerGear.com

   All rights reserved.

   This utility reference lib/direct/list.h file in directfb source
   Please reference http://www.directfb.org

   Modified by Alex Lau <avengermojo@gmail.com> 

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <types.h>
#include <debug.h>

struct __UED_LinkedList {
    //int         magic;

    LinkedList *next;
    LinkedList *prev; 
};

static __inline__ void
list_prepend( LinkedList **list, LinkedList *link )
{
    UED_ASSERT( link != NULL );
    LinkedList *first = *list;
    link->next = first;

    if (first) {
        link->prev = first->prev;
        first->prev = link;
    } else
        link->prev = link;

    *list = link;
}

static __inline__ void
list_append( LinkedList **list, LinkedList *link )
{
    UED_ASSERT( link != NULL );
    LinkedList *first = *list;
    link->next = NULL;

    if (first) {
        LinkedList *last = first->prev;
        link->prev = last;
        last->next = first->prev = link;
    } else
        *list = link->prev = link;
}

static __inline__ bool
list_contains_element_EXPENSIVE( LinkedList *list, LinkedList *link )
{
    UED_ASSERT( link != NULL );
    if (!link->prev && !link->next) return false;

    while (list) {
        if (list == link)
            return true;
        list = list->next;
    }
    return false;
}

static __inline__ int
list_count_elements_EXPENSIVE( LinkedList *list )
{
    int count = 0;

    while (list) {
        count++;
        list = list->next;
    }
    return count;
}

static __inline__ void
list_remove( LinkedList **list, LinkedList *link )
{
    LinkedList *next;
    LinkedList *prev;

    UED_ASSERT( list != NULL );
    UED_ASSERT( list_contains_element_EXPENSIVE( *list, link ) );

    next = link->next;
    prev = link->prev;

    if (next) {
        next->prev = prev;
    } else
        (*list)->prev = prev;

    if (link == *list)
        *list = next;
    else {
        prev->next = next;
    }
    link->next = link->prev = NULL;
}

static __inline__ void
list_move_to_front( LinkedList **list, LinkedList *link )
{
    LinkedList *next;
    LinkedList *prev;
    LinkedList *first;

    UED_ASSERT( list != NULL );
    first = *list;
    UED_ASSERT( list_contains_element_EXPENSIVE( first, link ) );

    if (first == link)
        return;

    next = link->next;
    prev = link->prev;

    if (next) {
        next->prev = prev;
        link->prev = first->prev;
    } else
        link->prev = prev;

    prev->next = next;
    link->next = first;
    first->prev = link;
    *list = link;
}

static __inline__ bool
list_check_link( const LinkedList *link )
{
    return link != NULL;
}

#define list_foreach(elem, list)                               \
    for (elem = (__typeof__(elem))(list);                      \
    list_check_link( (LinkedList*)(elem) );                    \
    elem = (__typeof__(elem))(((LinkedList*)(elem))->next))

#define list_foreach_safe(elem, temp, list)                                             \
    for (elem = (__typeof__(elem))(list), temp = ((__typeof__(temp))(elem) ? (__typeof__(temp))(((LinkedList*)(elem))->next) : NULL); \
    direct_list_check_link( (LinkedList*)(elem) );                                       \
    elem = (__typeof__(elem))(temp), temp = ((__typeof__(temp))(elem) ? (__typeof__(temp))(((LinkedList*)(elem))->next) : NULL))

#endif // __UED__LINKEDLIST_H__
