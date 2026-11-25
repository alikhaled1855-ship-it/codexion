/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaleh <alsaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:39:59 by alsaleh           #+#    #+#             */
/*   Updated: 2026/11/25 11:13:11 by alsaleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	log_message(t_sim *sim, int coder_number, char *message)
{
	long	elapsed;

	pthread_mutex_lock(&sim->print_lock);
	if (is_stop(sim) == 1)
	{
		pthread_mutex_unlock(&sim->print_lock);
		return ;
	}
	elapsed = get_current_time_ms() - sim->start_time;
	printf("%ld %d %s\n", elapsed, coder_number, message);
	pthread_mutex_unlock(&sim->print_lock);
}
