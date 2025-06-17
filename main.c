/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:56:18 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/17 16:38:20 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_all(t_data *data)
{
	int	i;

	i = 0;
	if (data->msg)
		ft_putendl_fd(data->msg, 2);
	while (i < data->nbr_of_philos)
	{
		pthread_mutex_destroy(&data->philo[i].left_fork);
		pthread_mutex_destroy(&data->philo[i].last_meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->stop_mutex);
	if (data->philo)
		free(data->philo);
	if (data->threads)
		free(data->threads);
}

int	error(t_data *data)
{
	if (data->msg)
		ft_putendl_fd(data->msg, 2);
	if (data->philo)
		free(data->philo);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!arg_check(argc) || !validate_input(&data, argv))
		return (1);
	if (!init_base(&data, argv, argc))
		return (error(&data));
	if (!init_philos(&data))
		return (error(&data));
	run_simulation(&data);
	free_all(&data);
	return (0);
}
