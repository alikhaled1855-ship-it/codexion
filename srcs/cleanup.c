/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaleh <alsaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 11:11:54 by alsaleh           #+#    #+#             */
/*   Updated: 2026/11/25 11:12:38 by alsaleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	cleanup_sim(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->coders[i].time_lock);
		free(sim->dongles[i].waiting.requests);
		i++;
	}
	pthread_mutex_destroy(&sim->print_lock);
	pthread_mutex_destroy(&sim->state_lock);
	pthread_mutex_destroy(&sim->dlock);
	pthread_cond_destroy(&sim->dcond);
	free(sim->dongles);
	free(sim->coders);
}
