/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alikhaled1855gmail.com <alikhaled1855gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:38:22 by nahas             #+#    #+#             */
/*   Updated: 2026/11/25 03:49:37 by alikhaled18      ###   ########.fr       */
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
	free(sim->dongles);
	free(sim->coders);
}
