/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:17:01 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/24 15:17:49 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	lock_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
	}
}

static void	eater(t_philo *philo)
{
	long long	current_time;

	wait_for_room(philo);
	lock_forks(philo);
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->last_meal_mutex);
	current_time = timestamp(philo->data->starting_time);
	philo->last_meal_time = current_time;
	pthread_mutex_unlock(&philo->last_meal_mutex);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->data->limit_mutex);
	philo->data->limit++;
	pthread_mutex_unlock(&philo->data->limit_mutex);
}

static void	sleeper(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->stop_mutex);
	if (philo->data->stop_simulation)
	{
		pthread_mutex_unlock(&philo->data->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->stop_mutex);
	print_action(philo, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
}

static void	thinker(t_philo *philo)
{
	int	die;
	int	eat;
	int	sleep;

	pthread_mutex_lock(&philo->data->stop_mutex);
	if (philo->data->stop_simulation)
	{
		pthread_mutex_unlock(&philo->data->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->stop_mutex);
	die = philo->time_to_die;
	eat = philo->time_to_eat;
	sleep = philo->time_to_sleep;
	print_action(philo, "is thinking");
	if (philo->data->nbr_of_philos % 2 != 0)
		usleep((1000 * (die - eat - sleep) * 0.75));
	else
		usleep(1000 * (die - eat - sleep) * 0.15);
}

void	*run(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->nbr_of_philos == 1)
	{
		one_philo_case(philo);
		return (NULL);
	}
	else
	{
		while (1)
		{
			if (!increment_meals(philo))
				break ;
			if (someone_died(philo))
				break ;
			eater(philo);
			sleeper(philo);
			thinker(philo);
		}
	}
	return (NULL);
}
