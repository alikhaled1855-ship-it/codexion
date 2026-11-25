/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaleh <alsaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:40:42 by alsaleh           #+#    #+#             */
/*   Updated: 2026/11/25 11:13:23 by alsaleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static void	wake_everyone(t_sim *sim)
{
	pthread_mutex_lock(&sim->dlock);
	pthread_cond_broadcast(&sim->dcond);
	pthread_mutex_unlock(&sim->dlock);
}

static int	is_done(t_sim *sim, int i)
{
	int	done;

	pthread_mutex_lock(&sim->coders[i].time_lock);
	done = (sim->coders[i].compiles_done
			>= sim->number_of_compiles_required);
	pthread_mutex_unlock(&sim->coders[i].time_lock);
	return (done);
}

static void	check_coder(t_sim *sim, int i)
{
	long	now;
	long	since_compile;

	if (is_done(sim, i))
		return ;
	now = get_current_time_ms() - sim->start_time;
	pthread_mutex_lock(&sim->coders[i].time_lock);
	since_compile = now - sim->coders[i].last_compile_start;
	pthread_mutex_unlock(&sim->coders[i].time_lock);
	if (since_compile >= sim->time_to_burnout)
	{
		log_message(sim, sim->coders[i].number, "burned out");
		pthread_mutex_lock(&sim->state_lock);
		sim->stop = 1;
		pthread_mutex_unlock(&sim->state_lock);
		wake_everyone(sim);
	}
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	int		finished;

	sim = (t_sim *)arg;
	while (is_stop(sim) == 0)
	{
		i = 0;
		finished = 0;
		while (i < sim->number_of_coders)
		{
			if (is_stop(sim) == 1)
				break ;
			finished += is_done(sim, i);
			check_coder(sim, i);
			i++;
		}
		if (finished == sim->number_of_coders)
			break ;
		usleep(5000);
	}
	return (NULL);
}
