/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_action.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:22:15 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/03 14:53:16 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	lock_forks(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
}

void	unlock_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	think_lock(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->action_lock);
	printf("%lld %d is thinking\n", timestamp(philo->starting_time), philo->id);
	pthread_mutex_unlock(&philo->data->action_lock);
}

void	sleep_lock(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->action_lock);
	printf("%lld %d is sleeping\n", timestamp(philo->starting_time), philo->id);
	usleep(philo->time_to_sleep * 1000); // Simulate sleeping
	pthread_mutex_unlock(&philo->data->action_lock);
}

void	eat_lock(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->action_lock);
	lock_forks(philo);
	printf("%lld %d has taken a fork\n", timestamp(philo->starting_time), philo->id);
	printf("%lld %d is eating\n", timestamp(philo->starting_time), philo->id);
	pthread_mutex_unlock(&philo->data->action_lock);
	usleep(philo->time_to_eat * 1000); // Simulate eating
	philo->last_meal = timestamp(philo->starting_time); // Update last meal time
	unlock_forks(philo);
}

void	single_eat_lock(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->action_lock);
	pthread_mutex_lock(&philo->left_fork);
	printf("%lld %d has taken a fork\n", timestamp(philo->starting_time), philo->id);
	pthread_mutex_unlock(&philo->data->action_lock);
	usleep(philo->time_to_die * 1000);
	pthread_mutex_unlock(&philo->left_fork);
}
