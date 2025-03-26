/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:20:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/26 16:22:46 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "libft/libft.h"
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philosopher
{
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread;
	int				id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
} t_philosopher;

typedef struct s_data
{
	int				nbr_of_philosophers;
	t_philosopher	*philosophers;
} t_data;

//closing program / memory handling

void	free_all(t_data *data);

#endif
