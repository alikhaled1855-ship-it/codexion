/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alikhaled1855gmail.com <alikhaled1855gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:39:36 by nahas             #+#    #+#             */
/*   Updated: 2026/11/23 07:42:52 by alikhaled18      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include <unistd.h>

static int	wait_for_turn(t_dongle *dongle, t_coder *coder, long key)	// change its name
{
	t_sim	*sim;

	sim = coder->sim;
	if (dongle->is_available == 0 || dongle->waiting.size > 0)	//think first condition enough
	{
		heap_push(&dongle->waiting, coder, key);
		while (dongle->is_available == 0
			|| dongle->waiting.requests[0].coder != coder)
		{
			if (sim->stop)
				return (0);
			pthread_cond_wait(&dongle->cond, &dongle->lock);
		}
		heap_pop(&dongle->waiting);
	}
	return (1);
}	 // this function to make sure that the other coder which can take the same dongle is ok

static int	acquire_one_dongle(t_dongle *dongle, t_coder *coder, long key)
{
	long	elapsed;
	t_sim	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&dongle->lock);
	if (sim->stop || !wait_for_turn(dongle, coder, key))		// stop here may make an error
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	elapsed = (get_current_time_ms() - sim->start_time)
		- dongle->last_release_time;
	if (elapsed < sim->dongle_cooldown)
		usleep((sim->dongle_cooldown - elapsed) * 1000);	// why not inside wait function
	dongle->is_available = 0;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}	// I have to check from the waiting list and the cooldown, wait is for waiting list

static void	order_dongles(t_coder *coder, t_dongle **first, t_dongle **second)
{
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
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	t_sim		*sim;
	long		key;

	sim = coder->sim;
	if (sim->is_edf)
		key = coder->last_compile_start + sim->time_to_burnout;
	else
		key = get_current_time_ms() - sim->start_time;		// why is here? (keys init)
	order_dongles(coder, &first, &second);
	if (!acquire_one_dongle(first, coder, key))
		return (0);
	if (!acquire_one_dongle(second, coder, key))
	{
		pthread_mutex_lock(&first->lock);
		first->is_available = 1;
		pthread_cond_broadcast(&first->cond);
		pthread_mutex_unlock(&first->lock);
		return (0);
	}
	return (1);
}		// I need to print x has taken a dongel

void	release_dongles(t_coder *coder)
{
	long	now;

	now = get_current_time_ms() - coder->sim->start_time; // May can I make another function for time now
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
