/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaleh <alsaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:39:43 by alsaleh           #+#    #+#             */
/*   Updated: 2026/11/25 11:13:02 by alsaleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	heap_init(t_heap *heap, int capacity)
{
	heap->requests = malloc(sizeof(t_request) * capacity);
	if (heap->requests == NULL)
		return (1);
	heap->size = 0;
	heap->capacity = capacity;
	heap->next_seq = 0;
	return (0);
}

int	is_before(t_request *a, t_request *b)
{
	if (a->key != b->key)
		return (a->key < b->key);
	return (a->seq < b->seq);
}

void	heap_push(t_heap *heap, t_coder *coder, long key)
{
	int			child;
	int			parent;
	t_request	temp;

	heap->requests[heap->size].coder = coder;
	heap->requests[heap->size].key = key;
	heap->requests[heap->size].seq = heap->next_seq;
	heap->next_seq++;
	child = heap->size;
	heap->size++;
	while (child > 0)
	{
		parent = (child - 1) / 2;
		if (!is_before(&heap->requests[child], &heap->requests[parent]))
			break ;
		temp = heap->requests[parent];
		heap->requests[parent] = heap->requests[child];
		heap->requests[child] = temp;
		child = parent;
	}
}

static int	get_smallest(t_heap *heap, int parent)
{
	int	left;
	int	right;
	int	smallest;

	left = (2 * parent) + 1;
	right = (2 * parent) + 2;
	smallest = parent;
	if (left < heap->size
		&& is_before(&heap->requests[left], &heap->requests[smallest]))
		smallest = left;
	if (right < heap->size
		&& is_before(&heap->requests[right], &heap->requests[smallest]))
		smallest = right;
	return (smallest);
}

t_coder	*heap_pop(t_heap *heap)
{
	t_coder		*result;
	int			parent;
	int			smallest;
	t_request	temp;

	result = heap->requests[0].coder;
	heap->size--;
	heap->requests[0] = heap->requests[heap->size];
	parent = 0;
	while (1)
	{
		smallest = get_smallest(heap, parent);
		if (smallest == parent)
			break ;
		temp = heap->requests[parent];
		heap->requests[parent] = heap->requests[smallest];
		heap->requests[smallest] = temp;
		parent = smallest;
	}
	return (result);
}
