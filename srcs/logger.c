/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nahas <nahas@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:39:59 by nahas             #+#    #+#             */
/*   Updated: 2026/09/01 22:40:03 by nahas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
#include <stdio.h>

void	log_message(t_sim *sim, int coder_number, char *message)
{
	long	elapsed;

	elapsed = get_current_time_ms() - sim->start_time;
	pthread_mutex_lock(&sim->print_lock);
	printf("%ld %d %s\n", elapsed, coder_number, message);
	pthread_mutex_unlock(&sim->print_lock);
}
