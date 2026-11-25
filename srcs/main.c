/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaleh <alsaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:40:12 by alsaleh           #+#    #+#             */
/*   Updated: 2026/11/25 11:13:18 by alsaleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static void	start_threads(t_sim *sim)
{
	int	i;

	pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim);
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_create(&sim->coders[i].thread, NULL, coder_routine,
			&sim->coders[i]);
		i++;
	}
}

static void	join_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (validate_args(argc, argv))
		return (1);
	if (init_sim(&sim, argv))
	{
		printf("Error\n");
		return (1);
	}
	start_threads(&sim);
	pthread_join(sim.monitor_thread, NULL);
	join_coders(&sim);
	sim.stop = 1;
	cleanup_sim(&sim);
	return (0);
}
