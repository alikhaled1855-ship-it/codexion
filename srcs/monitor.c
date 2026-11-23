/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alikhaled1855gmail.com <alikhaled1855gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:40:42 by nahas             #+#    #+#             */
/*   Updated: 2026/11/23 01:05:12 by alikhaled18      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include <unistd.h>

static void	wake_everyone(t_sim *sim)
{
	int	j;

	j = 0;
	while (j < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[j].lock);
		pthread_cond_broadcast(&sim->dongles[j].cond);
		pthread_mutex_unlock(&sim->dongles[j].lock);
		j++;
	}
}

static void	check_coder(t_sim *sim, int i)
{
	long	now;
	long	since_compile;

	now = get_current_time_ms() - sim->start_time;
	since_compile = now - sim->coders[i].last_compile_start;
	if (since_compile > sim->time_to_burnout)	// why not ">=" ?
	{
		log_message(sim, sim->coders[i].number, "has burned out");
		sim->stop = 1;
		wake_everyone(sim);
	}
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (sim->stop == 0)
	{
		i = 0;
		while (i < sim->number_of_coders)
		{
			check_coder(sim, i);
			i++;
		}	// Here is the issue.(It keeps checking for all coders even if one has bourned out)
		usleep(5000);		// why sleep?
	}
	return (NULL);
}
