/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:08:44 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 15:59:38 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	timestamp(long long starting_time)
{
	struct timeval	current_time;
	long long		current_time_ms;
	long long		elapsed_ms;

	gettimeofday(&current_time, NULL);
	current_time_ms = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	elapsed_ms = current_time_ms - starting_time;
	return (elapsed_ms);
}

long long init_time(void)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	return ((start.tv_sec * 1000) + (start.tv_usec / 1000));
}
