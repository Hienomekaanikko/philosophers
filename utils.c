/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:10:21 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/17 16:44:04 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	thread_failure_join(t_data *data, int i)
{
	int	j;

	j = 0;
	while (j <= i)
	{
		if (pthread_join(data->threads[j], NULL) != 0)
			data->msg = "Thread joining failed";
		j++;
	}
}

int	arg_check(int argc)
{
	if (argc > 6 || argc < 5)
	{
		ft_putendl_fd("ERROR!", 2);
		ft_putstr_fd("Input arguments like this: ", 2);
		ft_putstr_fd(ARG1, 2);
		ft_putstr_fd(ARG2, 2);
		ft_putstr_fd(ARG3, 2);
		ft_putstr_fd(ARG4, 2);
		ft_putendl_fd(ARG5, 2);
		return (0);
	}
	return (1);
}

int	is_space(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_isspace(cmd[i]))
			i++;
		else
			return (0);
	}
	return (1);
}

long long	ft_atol(const char *s)
{
	long long	result;
	long long	sign;

	result = 0;
	sign = 1;
	while (*s == ' ' || *s == '\t' || *s == '\n'
		|| *s == '\r' || *s == '\f' || *s == '\v')
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (ft_isdigit(*s))
	{
		if (result > (LLONG_MAX - (*s - '0')) / 10)
			return (result * sign);
		result = result * 10 + (*s++ - '0');
	}
	return (result * sign);
}

void	error_message(char *msg, char *arg)
{
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(msg, 2);
}
