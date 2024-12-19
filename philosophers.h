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

struct	s_philo;

typedef struct s_simulation
{
	size_t			start_time;
	int				processed_philos;
	struct s_philo	*philos;
	t_input			*input_data;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	lock;
}	t_simulation;

typedef struct s_philo
{
	int				id;
	t_simulation	*sim;
	pthread_t		thread;
	int				num_finished_meals;
	size_t			last_meal_time;
	int				first_fork_idx;
	int				second_fork_idx;
	bool			has_eaten;
	bool			dead;
	pthread_mutex_t dead_lock;
	pthread_mutex_t meal_lock;
}	t_philo;

/* eat.c */
void	go_eat(t_philo *philo);
int 	take_forks(t_philo *philo);
bool	take_fork(t_philo *philo, int fork_idx);
void	return_forks(t_philo *philo);
void	return_fork(t_philo *philo, int fork_idx);

/* init.c */
int		init_simulation(t_simulation *sim, t_input *input);
int		init_philos(t_simulation *sim);
int		init_philo(t_philo *philo, t_simulation *sim, int i);

/* input_utils.c */
int		ft_atoi(const char	*str);

/* monitor.c */
void	monitor_philos(t_simulation *sim);
bool	should_stop(t_simulation *sim, t_philo *philo);
bool	has_just_eaten(t_philo *philo);

/* process_input.c */
void	save_input_data(t_input *data, int argc, char **argv);
void	init_input_data(t_input *data);
int		process_input(t_input *data, int argc, char **argv);

/* routine.c */
void	*routine(void *arg);
void	handle_one_fork(t_philo *philo);
void	go_sleep(t_philo *philo);
void	go_think(t_philo *philo);

/* run_simulation.c */
int		run_simulation(t_simulation *sim, t_input *input_data);
void	create_philosophers_threads(t_philo *arr_philos, int num_philos);
void	wait_threads(t_simulation *sim);
void	clean_up_data(t_simulation *sim);

/* state.c */
bool	has_died(t_philo *philo);
void	set_dead(t_philo *philos, int num_philos);
bool	is_dead(t_philo *philo);

/* time_utils.c */
size_t	get_time(void);
void	ft_usleep(size_t ms, t_philo *philo);

/* utils.c */
void	print_message(t_philo *philo, char *str);

#endif