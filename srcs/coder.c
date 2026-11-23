/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alikhaled1855gmail.com <alikhaled1855gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:39:22 by nahas             #+#    #+#             */
/*   Updated: 2026/11/22 19:18:50 by alikhaled18      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include <unistd.h>

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (coder->compiles_done < sim->number_of_compiles_required
		&& sim->stop == 0)
	{
		if (!take_dongles(coder))
			break ;
		log_message(sim, coder->number, "is compiling");
		coder->last_compile_start = get_current_time_ms() - sim->start_time;
		usleep(sim->time_to_compile * 1000);
		release_dongles(coder);		// do i need to store the time here?
		coder->compiles_done++;
		log_message(sim, coder->number, "is debugging");
		usleep(sim->time_to_debug * 1000);
		log_message(sim, coder->number, "is refactoring");
		usleep(sim->time_to_refactor * 1000);
	}
	return (NULL);
}
