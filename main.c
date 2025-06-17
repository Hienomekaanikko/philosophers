/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:56:18 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/17 13:34:39 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	init_philosopher(t_philosopher *philo, t_data *data, pthread_mutex_t *right_fork)
{
	pthread_mutex_init(&philo->last_meal_mutex, NULL);
	pthread_mutex_init(&philo->meals_eaten_mutex, NULL);
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


static int	init_philos(t_data *data)
{
	pthread_mutex_t	*right_fork;
	int				i;

	i = 0;
	if (data->nbr_of_philos == 0)
	{
		printf("You must have atleast 1 philo!");
		return (0);
	}
	while (i < data->nbr_of_philos)
	{
		if (pthread_mutex_init(&data->philo[i].left_fork, NULL) != 0)
		{
			printf("Failed to initialize fork mutex\n");
			return (0);
		}
		i++;
	}
	i = 0;
	while (i < data->nbr_of_philos)
	{
		right_fork = &data->philo[(i + 1) % data->nbr_of_philos].left_fork;
		if (!init_philosopher(&data->philo[i], data, right_fork))
			return (0);
		data->philo[i].id = i + 1;
		i++;
	}
	return (1);
}

static int	init_base(t_data *data, char **argv, int argc)
{
	pthread_mutex_init(&data->stop_mutex, NULL);
	data->starting_time = 0;
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_sleep = ft_atol(argv[4]);
	data->time_to_eat = ft_atol(argv[3]);
	data->nbr_of_philos = ft_atol(argv[1]);
	if (argc == 6)
		data->max_meals = ft_atol(argv[5]);
	else
		data->max_meals = 0;
	data->error = 0;
	data->stop_simulation = 0;
	data->philo = malloc(ft_atol(argv[1]) * sizeof(t_philosopher));
	if (!data->philo)
		return (-1);
	return (1);
}

void	free_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		pthread_mutex_destroy(&data->philo[i].left_fork);
		pthread_mutex_destroy(&data->philo[i].last_meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->stop_mutex);
	free(data->philo);
	free(data->threads);
}


int	main(int argc, char **argv)
{
	t_data	data;

	if (!arg_check(argc) || !validate_input(&data, argv))
		return (1);
	if (!init_base(&data, argv, argc))
		return (1);
	if (!init_philos(&data))
		return (1);
	data.starting_time = init_time();
	run_simulation(&data);
	free_all(&data);
	return (0);
}
