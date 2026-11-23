/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahas <nahas@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:38:22 by nahas             #+#    #+#             */
/*   Updated: 2026/09/01 22:38:27 by nahas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include <stdlib.h>

void	cleanup_sim(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		free(sim->dongles[i].waiting.requests);
		i++;
	}
	pthread_mutex_destroy(&sim->print_lock);
	free(sim->dongles);
	free(sim->coders);
}
