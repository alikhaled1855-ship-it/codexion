/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alikhaled1855gmail.com <alikhaled1855gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:40:51 by nahas             #+#    #+#             */
/*   Updated: 2026/11/25 03:51:26 by alikhaled18      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	is_positive_number(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
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
		if (!is_positive_number(argv[i]))
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
