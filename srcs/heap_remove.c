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

static void	swap_req(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_up(t_heap *h, int i)
{
	while (i > 0 && is_before(&h->requests[i], &h->requests[(i - 1) / 2]))
	{
		swap_req(&h->requests[i], &h->requests[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

static void	sift_down(t_heap *h, int i)
{
	int	s;
	int	l;
	int	r;

	while (1)
	{
		l = (2 * i) + 1;
		r = (2 * i) + 2;
		s = i;
		if (l < h->size && is_before(&h->requests[l], &h->requests[s]))
			s = l;
		if (r < h->size && is_before(&h->requests[r], &h->requests[s]))
			s = r;
		if (s == i)
			return ;
		swap_req(&h->requests[i], &h->requests[s]);
		i = s;
	}
}

void	heap_remove(t_heap *h, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < h->size && h->requests[i].coder != coder)
		i++;
	if (i == h->size)
		return ;
	h->size--;
	h->requests[i] = h->requests[h->size];
	if (i < h->size)
	{
		sift_up(h, i);
		sift_down(h, i);
	}
}
