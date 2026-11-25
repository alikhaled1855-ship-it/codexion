/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaleh <alsaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:39:36 by alsaleh           #+#    #+#             */
/*   Updated: 2026/11/25 11:12:54 by alsaleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static long	ready_in(t_coder *c)
{
	long	now;
	long	w;
	long	w2;

	if (!c->left->is_available || !c->right->is_available
		|| c->left->waiting.requests[0].coder != c
		|| c->right->waiting.requests[0].coder != c)
		return (-1);
	now = get_current_time_ms() - c->sim->start_time;
	w = c->sim->dongle_cooldown - (now - c->left->last_release_time);
	w2 = c->sim->dongle_cooldown - (now - c->right->last_release_time);
	if (w2 > w)
		w = w2;
	if (w < 0)
		w = 0;
	return (w);
}

static void	wait_for(t_sim *sim, long ms)
{
	struct timespec	ts;
	long			target;

	if (ms < 0)
	{
		pthread_cond_wait(&sim->dcond, &sim->dlock);
		return ;
	}
	target = get_current_time_ms() + ms;
	ts.tv_sec = target / 1000;
	ts.tv_nsec = (target % 1000) * 1000000;
	pthread_cond_timedwait(&sim->dcond, &sim->dlock, &ts);
}

static int	claim(t_coder *coder)
{
	t_sim	*sim;
	long	w;
	long	key;

	sim = coder->sim;
	pthread_mutex_lock(&sim->dlock);
	key = get_key(coder);
	heap_push(&coder->left->waiting, coder, key);
	heap_push(&coder->right->waiting, coder, key);
	w = ready_in(coder);
	while (w != 0 && !is_stop(sim))
	{
		wait_for(sim, w);
		w = ready_in(coder);
	}
	if (w == 0)
	{
		heap_pop(&coder->left->waiting);
		heap_pop(&coder->right->waiting);
		coder->left->is_available = 0;
		coder->right->is_available = 0;
	}
	pthread_mutex_unlock(&sim->dlock);
	return (w == 0);
}

int	take_dongles(t_coder *coder)
{
	if (coder->left == coder->right)
		return (lone_coder(coder));
	if (!claim(coder))
		return (0);
	log_message(coder->sim, coder->number, "has taken a dongle");
	log_message(coder->sim, coder->number, "has taken a dongle");
	return (1);
}

void	release_dongles(t_coder *coder)
{
	long	now;

	now = get_current_time_ms() - coder->sim->start_time;
	pthread_mutex_lock(&coder->sim->dlock);
	coder->left->is_available = 1;
	coder->left->last_release_time = now;
	coder->right->is_available = 1;
	coder->right->last_release_time = now;
	pthread_cond_broadcast(&coder->sim->dcond);
	pthread_mutex_unlock(&coder->sim->dlock);
}
