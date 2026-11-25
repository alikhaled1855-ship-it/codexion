/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaleh <alsaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 20:07:49 by alsaleh           #+#    #+#             */
/*   Updated: 2026/11/25 11:13:07 by alsaleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static void	parse_into_sim(t_sim *sim, char **argv)
{
	sim->number_of_coders = atoi(argv[1]);
	sim->time_to_burnout = atoi(argv[2]);
	sim->time_to_compile = atoi(argv[3]);
	sim->time_to_debug = atoi(argv[4]);
	sim->time_to_refactor = atoi(argv[5]);
	sim->number_of_compiles_required = atoi(argv[6]);
	sim->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "edf") == 0)
		sim->is_edf = 1;
	else
		sim->is_edf = 0;
}

static int	alloc_arrays(t_sim *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (sim->coders == NULL)
		return (1);
	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	if (sim->dongles == NULL)
	{
		free(sim->coders);
		return (1);
	}
	return (0);
}

static int	init_one(t_sim *sim, int i)
{
	sim->dongles[i].is_available = 1;
	sim->dongles[i].last_release_time = -1000000;
	pthread_mutex_init(&sim->dongles[i].lock, NULL);
	pthread_cond_init(&sim->dongles[i].cond, NULL);
	pthread_mutex_init(&sim->coders[i].time_lock, NULL);
	if (heap_init(&sim->dongles[i].waiting, sim->number_of_coders))
		return (1);
	sim->coders[i].number = i + 1;
	sim->coders[i].right = &sim->dongles[i];
	sim->coders[i].left = &sim->dongles[(i - 1 + sim->number_of_coders)
		% sim->number_of_coders];
	sim->coders[i].compiles_done = 0;
	sim->coders[i].last_compile_start = 0;
	sim->coders[i].sim = sim;
	return (0);
}

int	init_sim(t_sim *sim, char **argv)
{
	int	i;

	parse_into_sim(sim, argv);
	if (alloc_arrays(sim))
		return (1);
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (init_one(sim, i))
			return (1);
		i++;
	}
	sim->stop = 0;
	sim->start_time = get_current_time_ms();
	pthread_mutex_init(&sim->print_lock, NULL);
	pthread_mutex_init(&sim->state_lock, NULL);
	return (0);
}
