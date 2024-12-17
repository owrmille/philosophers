#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

typedef struct s_input
{
	int	num_meals;
	int	num_philos;
	int	die_time;
	int	eat_time;
	int	sleep_time;
}	t_input;

struct s_philo;

typedef struct s_simulation
{
	size_t			start_time;
	int				processed_philos;
	struct s_philo	*philos;
	t_input			*input_data;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	processed_philos_lock;
}	t_simulation;

typedef struct s_philo
{
	int				id;
	t_simulation	*sim;
	pthread_t		thread;
	int				first_fork_idx;
	int				second_fork_idx;
	int				num_finished_meals;
	bool			has_eaten;
	bool			dead;
	size_t			last_meal_time;
	pthread_mutex_t	last_meal_time_lock;
	pthread_mutex_t has_eaten_lock;
	pthread_mutex_t dead_lock;
	pthread_mutex_t meal_lock;
}	t_philo;

/* init.c */
int		init_simulation(t_simulation *sim, t_input *input);
t_philo	*init_philos(t_simulation *sim);

int		ft_atoi(const char	*str);
int		process_input(t_input *data, int argc, char **argv);

/* time_utils.c */
size_t	get_time(void);
void		ft_usleep(size_t ms, t_philo *philo);
void	print_message(t_philo *philo, char *str);

/* run_simulation.c */

bool	has_died(t_philo *philo);
void	print_message(t_philo *philo, char *msg);
void	take_left_fork(t_philo *philo);
void	take_right_fork(t_philo *philo);
void	go_eat(t_philo *philo);
void	go_sleep(t_philo *philo);
void	go_think(t_philo *philo);
void	*routine(void *arg);
void	create_philosophers_threads(t_philo *arr_philos, int num_philos);
void	wait_threads(t_simulation *sim, pthread_t *dead_monitor_thread, pthread_t *meals_monitor_thread);
void	clean_up_data(t_philo *arr_philos, t_simulation *sim);
int		run_simulation(t_simulation *sim, t_input *input_data);

// void	free_philos(t_philo *arr_philos);

#endif