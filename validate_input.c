/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:07:08 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/17 16:00:12 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	check_limits(t_data *data, char *arg)
{
	long long	nbr;

	nbr = ft_atol(arg);
	if (nbr > INT_MAX)
	{
		error_message(" <- No larger values than INT MAX are allowed!", arg);
		data->syntax_error = 1;
	}
	else if (nbr < INT_MIN)
	{
		error_message(" <- No negative numbers allowed!", arg);
		data->syntax_error = 1;
	}
	return (1);
}

void	check_if_valid(t_data *data, char **argv, int i, int *j)
{
	while (is_space(&argv[i][*j]))
		(*j)++;
	if (argv[i][*j] == '+')
		(*j)++;
	else if (argv[i][*j] == '-')
	{
		error_message(" <- Only positive values are allowed!", argv[i]);
		data->syntax_error = 1;
	}
	while (argv[i][*j])
	{
		if (!ft_isnum(&argv[i][*j]))
		{
			error_message(" <- Only digits are allowed!", argv[i]);
			data->syntax_error = 1;
			break ;
		}
		(*j)++;
	}
	if (!check_limits(data, argv[i]))
		data->syntax_error = 1;
}

int	validate_input(t_data *data, char **argv)
{
	int	i;
	int	j;

	i = 1;
	data->syntax_error = 0;
	while (argv[i])
	{
		j = 0;
		check_if_valid(data, argv, i, &j);
		i++;
	}
	if (!data->syntax_error)
		return (1);
	else
		return (0);
}
