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

static int	wait_for_turn(t_dongle *dongle, t_coder *coder, long key)
{
	t_sim	*sim;

	sim = coder->sim;
	if (dongle->is_available == 0 || dongle->waiting.size > 0)
	{
		heap_push(&dongle->waiting, coder, key);
		while (dongle->is_available == 0
			|| dongle->waiting.requests[0].coder != coder)
		{
			if (is_stop(sim))
				return (0);
			pthread_cond_wait(&dongle->cond, &dongle->lock);
		}
		heap_pop(&dongle->waiting);
	}
	return (1);
}

static int	acquire_one_dongle(t_dongle *dongle, t_coder *coder, long key)
{
	long	elapsed;
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&dongle->lock);
	if (is_stop(sim) || !wait_for_turn(dongle, coder, key))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	elapsed = (get_current_time_ms() - sim->start_time)
		- dongle->last_release_time;
	dongle->is_available = 0;
	pthread_mutex_unlock(&dongle->lock);
	if (elapsed < sim->dongle_cooldown)
		usleep((sim->dongle_cooldown - elapsed) * 1000);
	return (1);
}

static void	order_dongles(t_coder *coder, t_dongle **first,
	t_dongle **second, long *key)
{
	t_sim	*sim;

	sim = coder->sim;
	if (coder->left < coder->right)
	{
		*first = coder->left;
		*second = coder->right;
	}
	else
	{
		*first = coder->right;
		*second = coder->left;
	}
	if (sim->is_edf)
		*key = coder->last_compile_start + sim->time_to_burnout;
	else
		*key = get_current_time_ms() - sim->start_time;
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	t_sim		*sim;
	long		key;

	sim = coder->sim;
	order_dongles(coder, &first, &second, &key);
	if (!acquire_one_dongle(first, coder, key))
		return (0);
	log_message(sim, coder->number, "has taken a dongle");
	if (!acquire_one_dongle(second, coder, key))
	{
		pthread_mutex_lock(&first->lock);
		first->is_available = 1;
		pthread_cond_broadcast(&first->cond);
		pthread_mutex_unlock(&first->lock);
		return (0);
	}
	log_message(sim, coder->number, "has taken a dongle");
	return (1);
}

void	release_dongles(t_coder *coder)
{
	long	now;

	now = get_current_time_ms() - coder->sim->start_time;
	pthread_mutex_lock(&coder->left->lock);
	coder->left->is_available = 1;
	coder->left->last_release_time = now;
	pthread_cond_broadcast(&coder->left->cond);
	pthread_mutex_unlock(&coder->left->lock);
	pthread_mutex_lock(&coder->right->lock);
	coder->right->is_available = 1;
	coder->right->last_release_time = now;
	pthread_cond_broadcast(&coder->right->cond);
	pthread_mutex_unlock(&coder->right->lock);
}
