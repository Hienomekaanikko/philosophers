/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_action.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:22:15 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/12 17:43:17 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void print_status(t_philosopher *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->action_lock);
	printf("%lld %d %s\n", timestamp(philo->starting_time), philo->id, msg);
	pthread_mutex_unlock(&philo->data->action_lock);
}


void	lock_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork);
		print_status(philo, "has taken a fork");
	}
}

void	unlock_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleep_lock(t_philosopher *philo)
{
	print_status(philo, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
}

void eat_lock(t_philosopher *philo)
{
	lock_forks(philo);
	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal = timestamp(philo->starting_time);
	pthread_mutex_unlock(&philo->last_meal_lock);
	print_status(philo, "is eating");
	usleep(philo->time_to_eat * 1000);
	unlock_forks(philo);
	philo->meals++;
}
