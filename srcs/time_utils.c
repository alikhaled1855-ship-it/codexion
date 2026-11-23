/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alikhaled1855gmail.com <alikhaled1855gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:40:59 by nahas             #+#    #+#             */
/*   Updated: 2026/11/07 20:01:25 by alikhaled18      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <sys/time.h>

long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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