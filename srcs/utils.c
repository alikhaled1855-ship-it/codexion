/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaleh <alsaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:40:59 by alsaleh           #+#    #+#             */
/*   Updated: 2026/11/25 11:13:32 by alsaleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	is_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->state_lock);
	if (sim->stop == 1)
	{
		pthread_mutex_unlock(&sim->state_lock);
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&sim->state_lock);
		return (0);
	}
}

long	get_key(t_coder *coder)
{
	if (coder->sim->is_edf)
		return (coder->last_compile_start + coder->sim->time_to_burnout);
	return (get_current_time_ms() - coder->sim->start_time);
}
