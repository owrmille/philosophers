#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
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

typedef struct s_philo
{
	int			id;
	pthread_t	thread;
	int			num_finished_meals;
	int			start_time;
	int			last_meal_time;
	int			is_dead;
}	t_philo;

typedef struct s_simulation
{
	t_input		*input_data;
	pthread_t	*threads;
	int			is_one_dead;
}	t_simulation;

int		ft_atoi(const char	*str);
int		process_input(t_input *data, int argc, char **argv);
// int		run_simulation(t_simulation *sim, t_input *input_data);
size_t	get_time(void);
int		ft_usleep(size_t ms);
// void	free_philos(t_philo *arr_philos);

#endif