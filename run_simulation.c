#include "philosophers.h"

void	print_message(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&(philo->sim->write));
	printf("%ld %d %s\n", get_time() - philo->start_time, philo->id, msg);
	pthread_mutex_unlock(&(philo->sim->write));
}

void	set_someone_is_dead(t_simulation *sim)
{
	pthread_mutex_lock(&(sim->dead));
	sim->someone_is_dead = 1;
	pthread_mutex_unlock(&(sim->dead));
}

bool	has_died(t_philo *philo)
{
	size_t	curtime;
	size_t	last_meal_time;
	size_t	die_time;

	curtime = get_time();
	last_meal_time = philo->last_meal_time;
	die_time = philo->sim->input_data->die_time;
	if (curtime - last_meal_time > die_time)
	{
		// printf("start_time: %ld\ncurtime: %ld\nlast_meal_time: %ld\ndie_time: %ld\n", philo->start_time, curtime, last_meal_time, die_time);
		print_message(philo, "died");
		set_someone_is_dead(philo->sim);
		return (1);
	}
	return (0);
}

bool	take_fork(t_philo *philo, int fork_idx)
{
	pthread_mutex_t	*fork;
	t_simulation	*sim;

	sim = philo->sim;
	fork = &(sim->forks[fork_idx]);
	while (1)
	{
		if (sim->is_fork_occupied[fork_idx])
		{
			if (has_died(philo))
				return (0);
		}
		else
		{
			pthread_mutex_lock(fork);
			sim->is_fork_occupied[fork_idx] = 1;
			print_message(philo, "has taken a fork");
			return (1);
		}
	}
}

void	return_fork(t_philo *philo, int fork_idx)
{
	t_simulation	*sim;

	sim = philo->sim;
	sim->is_fork_occupied[fork_idx] = 0;
	pthread_mutex_unlock(&(sim->forks[fork_idx]));
}

void	go_eat(t_philo *philo)
{
	if (!take_fork(philo, philo->first_fork_idx))
	{
		return ;
	}
	if (!take_fork(philo, philo->second_fork_idx))
	{
		return_fork(philo, philo->first_fork_idx);
		return ;
	}
	if (!philo->sim->someone_is_dead)
	{
		print_message(philo, "is eating");
		if (philo->num_finished_meals != -1)
			philo->num_finished_meals++;
		ft_usleep(philo->sim->input_data->eat_time);
		philo->last_meal_time = get_time();
	}
	return_fork(philo, philo->second_fork_idx);
	return_fork(philo, philo->first_fork_idx);
}

void	go_sleep(t_philo *philo)
{
	print_message(philo, "is sleeping");
	ft_usleep(philo->sim->input_data->sleep_time);
}

void	go_think(t_philo *philo)
{
	print_message(philo, "is thinking");
}

void	*routine(void *arg)
{
	t_philo			*philo;
	t_simulation	*sim;

	philo = (t_philo *)arg;
	sim = philo->sim;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (1)
	{
		go_eat(philo);
		if (sim->someone_is_dead)
			break ;
		go_sleep(philo);
		if (sim->someone_is_dead)
			break ;
		go_think(philo);
	}
	return (NULL);
}

void	create_threads(t_philo *arr_philos, int num_philos)
{
	int	i;

	i = -1;
	while (++i < num_philos)
		pthread_create(&arr_philos[i].thread, NULL, routine, (void *)&arr_philos[i]);
}

void	wait_threads(t_philo *arr_philos, int num_philos)
{
	int	i;

	i = -1;
	while (++i < num_philos)
		pthread_join(arr_philos[i].thread, NULL);
}

void	clean_up_data(t_philo *arr_philos, t_simulation *sim)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&sim->write);
	pthread_mutex_destroy(&sim->dead);
	while (++i < sim->input_data->num_philos)
		pthread_mutex_destroy(&sim->forks[i]);
	if (sim->forks)
		free(sim->forks);
	if (arr_philos)
		free(arr_philos);
}

int	run_simulation(t_simulation *sim, t_input *input_data)
{
	t_philo	*arr_philos;

	if (init_simulation(sim, input_data) == 1)
		return (1);
	arr_philos = init_philos(sim);
	if (arr_philos == NULL)
	{
		clean_up_data(arr_philos, sim);
		return (1);
	}
	create_threads(arr_philos, input_data->num_philos);
	wait_threads(arr_philos, input_data->num_philos);
	clean_up_data(arr_philos, sim);
	return (0);
}
