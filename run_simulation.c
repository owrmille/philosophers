#include "philosophers.h"

void	print_message(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&(philo->sim->write));
	if (!philo->sim->is_someone_dead)
		printf("%ld %d %s\n", get_time() - philo->start_time, philo->id, msg);
	pthread_mutex_unlock(&(philo->sim->write));
}

bool	are_philos_done_with_meals(t_simulation *sim)
{
	int		num_philos;
	int		processed_philos;
	bool	res;

	pthread_mutex_lock(&sim->meals);
	num_philos = sim->input_data->num_philos;
	processed_philos = sim->processed_philos;
	if (processed_philos == num_philos)
		res = true;
	else
		res = false;
	pthread_mutex_unlock(&sim->meals);
	return (res);
}

bool	check_if_someone_is_dead(t_simulation *sim)
{
	bool	res;

	pthread_mutex_lock(&(sim->dead));
	if (sim->is_someone_dead)
		res = true;
	else
		res = false;
	pthread_mutex_unlock(&(sim->dead));
	return (res);
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
		pthread_mutex_lock(&(philo->sim->dead));
		if (!philo->sim->is_someone_dead)
		{
			print_message(philo, "died");
			philo->sim->is_someone_dead = 1;
		}
		pthread_mutex_unlock(&(philo->sim->dead));
		return (true);
	}
	return (false);
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
				return (false);
		}
		else
		{
			pthread_mutex_lock(fork);
			sim->is_fork_occupied[fork_idx] = 1;
			print_message(philo, "has taken a fork");
			return (true);
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
	if (!check_if_someone_is_dead(philo->sim))
	{
		print_message(philo, "is eating");
		philo->last_meal_time = get_time();
		ft_usleep(philo->sim->input_data->eat_time, philo);
		if (philo->num_finished_meals != -1)
			philo->num_finished_meals++;

		// NEW
		pthread_mutex_lock(&philo->sim->meals);
		if (philo->num_finished_meals != -1
			&& philo->num_finished_meals == philo->sim->input_data->num_meals)
			philo->sim->processed_philos++;
		pthread_mutex_unlock(&philo->sim->meals);
		// NEW
	}
	return_fork(philo, philo->second_fork_idx);
	return_fork(philo, philo->first_fork_idx);
}

void	go_sleep(t_philo *philo)
{
	print_message(philo, "is sleeping");
	ft_usleep(philo->sim->input_data->sleep_time, philo);
}

void	go_think(t_philo *philo)
{
	print_message(philo, "is thinking");
	// if (philo->sim->input_data->num_philos % 2 != 0)
	// 	ft_usleep(10, philo);
}

void	*routine(void *arg)
{
	t_philo			*philo;
	t_simulation	*sim;

	philo = (t_philo *)arg;
	sim = philo->sim;
	if (philo->id % 2 == 0)
		ft_usleep(sim->input_data->eat_time / 2, philo);
	while (1)
	{
		go_eat(philo);
		if (are_philos_done_with_meals(sim) || check_if_someone_is_dead(sim))
			break ;
		go_sleep(philo);
		if (are_philos_done_with_meals(sim) || check_if_someone_is_dead(sim))
			break ;
		go_think(philo);
		if (are_philos_done_with_meals(sim))
			break ;
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
	pthread_mutex_destroy(&sim->meals);
	while (++i < sim->input_data->num_philos)
		pthread_mutex_destroy(&sim->forks[i]);
	if (sim->forks)
		free(sim->forks);
	if (arr_philos)
		free(arr_philos);
	if (sim->is_fork_occupied)
		free(sim->is_fork_occupied);
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
