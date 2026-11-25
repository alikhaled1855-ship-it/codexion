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

static int	find_me(t_heap *h, t_coder *me)
{
	int	i;

	i = 0;
	while (i < h->size && h->requests[i].coder != me)
		i++;
	return (i);
}

int	has_rival(t_dongle *d, t_coder *me)
{
	t_heap	*h;
	t_coder	*w;
	int		i;
	int		mine;

	h = &d->waiting;
	mine = find_me(h, me);
	i = 0;
	while (i < h->size)
	{
		w = h->requests[i].coder;
		if (w != me && is_before(&h->requests[i], &h->requests[mine])
			&& w->left->is_available && w->right->is_available)
			return (1);
		i++;
	}
	return (0);
}
