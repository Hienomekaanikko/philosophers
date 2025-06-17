/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:04:34 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/17 16:23:58 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_mutex(t_data *data, pthread_mutex_t *mutex)
{
	if (pthread_mutex_init(mutex, NULL) != 0)
	{
		data->msg = "Failed to initialize mutex";
		return (0);
	}
	return (1);
}

int	init_ph(t_philo *philo, t_data *data, pthread_mutex_t *right_fork)
{
	if (!init_mutex(data, &philo->last_meal_mutex))
		return (0);
	if (!init_mutex(data, &philo->meals_eaten_mutex))
		return (0);
	philo->right_fork = right_fork;
	philo->dead = 0;
	philo->data = data;
	philo->time_to_die = data->time_to_die;
	philo->time_to_sleep = data->time_to_sleep;
	philo->time_to_eat = data->time_to_eat;
	philo->last_meal_time = data->starting_time;
	philo->max_meals = data->max_meals;
	philo->meals_eaten = 0;
	return (1);
}

int	init_philos(t_data *data)
{
	pthread_mutex_t	*right_fork;
	int				i;

	i = 0;
	if (data->nbr_of_philos == 0)
	{
		data->msg = "You must have atleast 1 philo";
		return (0);
	}
	while (i < data->nbr_of_philos)
	{
		if (!init_mutex(data, &data->philo[i].left_fork))
			return (0);
		i++;
	}
	i = 0;
	while (i < data->nbr_of_philos)
	{
		right_fork = &data->philo[(i + 1) % data->nbr_of_philos].left_fork;
		if (!init_ph(&data->philo[i], data, right_fork))
			return (0);
		data->philo[i].id = i + 1;
		i++;
	}
	return (1);
}

int	init_base(t_data *data, char **argv, int argc)
{
	data->msg = NULL;
	if (!init_mutex(data, &data->stop_mutex))
		return (0);
	data->starting_time = 0;
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_sleep = ft_atol(argv[4]);
	data->time_to_eat = ft_atol(argv[3]);
	data->nbr_of_philos = ft_atol(argv[1]);
	if (argc == 6)
		data->max_meals = ft_atol(argv[5]);
	else
		data->max_meals = 0;
	data->syntax_error = 0;
	data->stop_simulation = 0;
	data->threads = NULL;
	data->philo = malloc(ft_atol(argv[1]) * sizeof(t_philo));
	if (!data->philo)
	{
		data->msg = "Malloc failed";
		return (0);
	}
	return (1);
}
