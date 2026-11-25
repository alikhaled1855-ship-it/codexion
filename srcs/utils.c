/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alikhaled1855gmail.com <alikhaled1855gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:40:59 by nahas             #+#    #+#             */
/*   Updated: 2026/11/25 03:49:01 by alikhaled18      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int		is_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->state_lock);
	if (sim->stop == 1)
	{
		pthread_mutex_unlock(&sim->state_lock);
		return 1;
	}
	else
	{
		pthread_mutex_unlock(&sim->state_lock);
		 return 0;
	}
}

// #include <stddef.h>
// #include <sys/time.h>
// #include <stdio.h>

// long	get_current_time_ms(void)
// {
// 	struct timeval	tv;

// 	gettimeofday(&tv, NULL);
// 	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
// }

// int main()
// {
// 	printf("%ld\n", get_current_time_ms());
	
// 	return 0;
// }