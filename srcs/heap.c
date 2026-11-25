/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alikhaled1855gmail.com <alikhaled1855gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:39:43 by nahas             #+#    #+#             */
/*   Updated: 2026/11/25 03:50:57 by alikhaled18      ###   ########.fr       */
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
	return (0);
}

void	heap_push(t_heap *heap, t_coder *coder, long key)
{
	int			child;
	int			parent;
	t_request	temp;

	heap->requests[heap->size].coder = coder;
	heap->requests[heap->size].key = key;
	child = heap->size;
	heap->size++;
	while (child > 0)
	{
		parent = (child - 1) / 2;
		if (heap->requests[parent].key <= heap->requests[child].key)
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
		&& heap->requests[left].key < heap->requests[smallest].key)
		smallest = left;
	if (right < heap->size
		&& heap->requests[right].key < heap->requests[smallest].key)
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
