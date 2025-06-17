/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:20:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/17 13:34:26 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

# define ARG1	"number_of_philosophers, "
# define ARG2	"time_to_die, "
# define ARG3	"time_to_eat, "
# define ARG4	"time_to_sleep, "
# define ARG5	"optional: [number_of_times_each_philosopher_must_eat]"

typedef struct s_philosopher
{
	pthread_mutex_t		left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		last_meal_mutex;
	pthread_mutex_t		meals_eaten_mutex;
	int					id;
	int					time_to_die;
	int					time_to_sleep;
	int					time_to_eat;
	int					max_meals;
	int					meals_eaten;
	long long			last_meal_time;
	int					dead;
	struct s_data		*data;
} t_philosopher;

typedef struct s_data
{
	t_philosopher	*philo;
	pthread_t		*threads;
	pthread_mutex_t	stop_mutex;
	long long		starting_time;
	int				nbr_of_philos;
	int				error;
	int				stop_simulation;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				max_meals;
} t_data;

// Parsing
int			ft_isnum(char *num);
int			is_space(char *cmd);
long long	ft_atol(const char *s);
int			ft_isdigit(int c);
int			ft_isspace(int c);
int			arg_check(int argc);

// Starting
int		validate_input(t_data *data, char **argv);
void	run_simulation(t_data *data);
void	*monitor_routine(void *arg);

// Time
long long	timestamp(long long starting_time);
long long	init_time(void);

// Error
void	error_message(char *msg, char *arg);
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(const char *s, int fd);

#endif

