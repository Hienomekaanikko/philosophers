/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:20:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/02 17:27:44 by msuokas          ###   ########.fr       */
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

# define ARG1	"number_of_philosophers, "
# define ARG2	"time_to_die, "
# define ARG3	"time_to_eat, "
# define ARG4	"time_to_sleep, "
# define ARG5	"optional: [number_of_times_each_philosopher_must_eat]"

typedef struct s_philosopher
{
	pthread_mutex_t		left_fork;
	pthread_mutex_t		*right_fork;
	pthread_t			thread;
	pthread_mutex_t		mutex;
	time_t				left_fork_pickup_time;
	time_t				right_fork_pickup_time;
	time_t				time_to_die;
	time_t				time_to_eat;
	time_t				time_to_sleep;
	time_t				curr_time;
	time_t				starting_time;
	time_t				last_meal;
	int					dead_status;
	int					max_meals;
	int					id;
	int					dead;
	struct s_data		*data;
} t_philosopher;

typedef struct s_data
{
	int					nbr_of_philosophers;
	int					error;
	t_philosopher		*philosophers;
	int					simulation_running;
} t_data;

// Function declarations

void	free_all(t_data *data);
void	destroy_left_forks(t_data *data);
void	error_message(char *msg, char *arg);
int		validate_input(t_data *data, char **argv);
#endif

