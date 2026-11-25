/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsaleh <alsaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:40:51 by alsaleh           #+#    #+#             */
/*   Updated: 2026/11/25 12:18:43 by alsaleh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	is_number(char *str, long min)
{
	int		i;
	long	value;

	i = 0;
	value = 0;
	if (str[0] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		value = (value * 10) + (str[i] - '0');
		if (value > INT_MAX)
			return (0);
		i++;
	}
	return (value >= min);
}

int	validate_args(int argc, char **argv)
{
	int	i;

	if (argc != 9)
	{
		printf("Error\n");
		return (1);
	}
	i = 1;
	while (i <= 7)
	{
		if (!is_number(argv[i], i != 7))
		{
			printf("Error\n");
			return (1);
		}
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
	{
		printf("Error\n");
		return (1);
	}
	return (0);
}
