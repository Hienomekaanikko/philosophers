/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:19:55 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/26 16:47:46 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

	//arg1 number_of_philosophers (same as the  amount of forks)
	//arg2 time_to_die (in ms)
	//arg3 time_to_eat (in ms)
	//arg4 time_to_sleep (in ms)
	//arg5 optional argument [number_of_times_each_philosopher_must_eat]

void	*run_philo(void* arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher*)arg;
	printf("Philosopher %d sat down\n", philo->id);

	return (NULL);
}

int	init_threads(t_data *data, char **argv)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].left_fork = malloc(sizeof(pthread_mutex_t));
		if (!data->philosophers[i].left_fork)
		{
			printf("Malloc fail!\n");
			return (0);
		}
		pthread_mutex_init(data->philosophers[i].left_fork, NULL);
		if (i + 1 < data->nbr_of_philosophers)
		{
			printf("Assigning the left fork of philosopher %d 'fork address: %p' to be the right fork of philosopher %d\n", data->philosophers[i + 1].id, data->philosophers[i + 1].left_fork, data->philosophers[i].id);
			data->philosophers[i].right_fork = data->philosophers[i + 1].left_fork;
		}
		else
			data->philosophers[i].right_fork = data->philosophers[0].left_fork;
		data->philosophers[i].time_to_die = ft_atoi(argv[2]);
		data->philosophers[i].time_to_eat = ft_atoi(argv[3]);
		data->philosophers[i].time_to_sleep = ft_atoi(argv[4]);
		if (pthread_create(&data->philosophers[i].thread, NULL, run_philo, &data->philosophers[i]) != 0)
		{
			printf("Thread creation failed\n");
			return (0);
		}
		i++;
	}
	return (1);
}

void	wait_for_finish(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
}

void	init_data(t_data *data)
{
	data->nbr_of_philosophers = 0;
	data->philosophers = NULL;
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc == 6)
	{
		init_data(&data);
		data.nbr_of_philosophers = ft_atoi(argv[1]);
		data.philosophers = malloc(data.nbr_of_philosophers * sizeof(t_philosopher));
		if (!data.philosophers)
		{
			printf("Malloc fail!\n");
			return (1);
		}
		init_threads(&data, argv);
		wait_for_finish(&data);
		//hopohopohopohehehe
		free_all(&data);
	}
	return (0);
}
