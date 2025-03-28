/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 13:16:33 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/28 15:08:31 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_status(pthread_mutex_t *msg_mutex, char *msg, long long time, int id)
{
	pthread_mutex_lock(msg_mutex);
	printf("Time: %lld, Philosopher %d is %s\n", time, id, msg);
	pthread_mutex_unlock(msg_mutex);
}
