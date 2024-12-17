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

typedef struct s_simulation
{
	bool			is_someone_dead;
	int				processed_philos;
	t_input			*input_data;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write;
	pthread_mutex_t	dead;
	pthread_mutex_t	meals;
}	t_simulation;

typedef struct s_philo
{
	size_t			start_time;
	int				id;
	int				num_finished_meals;
	size_t			last_meal_time;
	t_simulation	*sim;
	pthread_t		thread;
	int				first_fork_idx;
	int				second_fork_idx;
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
void	create_threads(t_philo *arr_philos, int num_philos);
void	wait_threads(t_philo *arr_philos, int num_philos);
void	clean_up_data(t_philo *arr_philos, t_simulation *sim);
int		run_simulation(t_simulation *sim, t_input *input_data);

// void	free_philos(t_philo *arr_philos);

#endif