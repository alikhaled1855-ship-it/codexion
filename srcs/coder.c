/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaleh <alsaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:39:22 by alsaleh           #+#    #+#             */
/*   Updated: 2026/11/25 11:12:22 by alsaleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (coder->compiles_done < sim->number_of_compiles_required
		&& is_stop(sim) == 0)
	{
		if (!take_dongles(coder))
			break ;
		log_message(sim, coder->number, "is compiling");
		pthread_mutex_lock(&coder->time_lock);
		coder->last_compile_start = get_current_time_ms() - sim->start_time;
		pthread_mutex_unlock(&coder->time_lock);
		usleep(sim->time_to_compile * 1000);
		release_dongles(coder);
		pthread_mutex_lock(&coder->time_lock);
		coder->compiles_done++;
		pthread_mutex_unlock(&coder->time_lock);
		log_message(sim, coder->number, "is debugging");
		usleep(sim->time_to_debug * 1000);
		log_message(sim, coder->number, "is refactoring");
		usleep(sim->time_to_refactor * 1000);
	}
	return (NULL);
}
