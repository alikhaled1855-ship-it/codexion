/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alikhaled1855gmail.com <alikhaled1855gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:39:59 by nahas             #+#    #+#             */
/*   Updated: 2026/11/25 03:55:44 by alikhaled18      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	log_message(t_sim *sim, int coder_number, char *message)
{
	long	elapsed;

	elapsed = get_current_time_ms() - sim->start_time;
	pthread_mutex_lock(&sim->print_lock);
	if (is_stop(sim) == 1)
		{
			pthread_mutex_unlock(&sim->print_lock);
			return;
		}
	printf("%ld %d %s\n", elapsed, coder_number, message);
	pthread_mutex_unlock(&sim->print_lock);
}
