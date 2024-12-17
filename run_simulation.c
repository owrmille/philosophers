#include "philosophers.h"

void	print_message(t_philo *philo, char *msg)
{
	bool dead;
	pthread_mutex_lock(&(philo->dead_lock));
	dead = philo->dead;
	pthread_mutex_unlock(&(philo->dead_lock));
	if (!dead) {
		pthread_mutex_lock(&(philo->sim->write_lock));
		printf("%ld %d %s\n", get_time() - philo->sim->start_time, philo->id, msg);
		pthread_mutex_unlock(&(philo->sim->write_lock));
	}
}


bool	has_died(t_philo *philo)
{
	pthread_mutex_lock(&(philo->dead_lock));
	if (philo->dead) {
		pthread_mutex_unlock(&(philo->dead_lock));
		return (true);
	}
	pthread_mutex_unlock(&(philo->dead_lock));

	size_t	curtime;
	size_t	last_meal_time;
	size_t	die_time;

	curtime = get_time();
	pthread_mutex_lock(&(philo->last_meal_time_lock));
	last_meal_time = philo->last_meal_time;
	pthread_mutex_unlock(&(philo->last_meal_time_lock));
	die_time = philo->sim->input_data->die_time;

	if (curtime - last_meal_time > die_time)
	{
		pthread_mutex_lock(&(philo->dead_lock));
		if (!philo->dead)
		{
			print_message(philo, "died");
			philo->dead = true;
		}
		pthread_mutex_unlock(&(philo->dead_lock));
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
	if (has_died(philo))
		return (false);
	pthread_mutex_lock(fork);
	print_message(philo, "has taken a fork");
	return (true);
}

void	return_fork(t_philo *philo, int fork_idx)
{
	t_simulation	*sim;

	sim = philo->sim;
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
	if (!has_died(philo))
	{
		print_message(philo, "is eating");
		pthread_mutex_lock(&(philo->last_meal_time_lock));
		philo->last_meal_time = get_time();
		pthread_mutex_unlock(&(philo->last_meal_time_lock));
		ft_usleep(philo->sim->input_data->eat_time, philo);
		philo->num_finished_meals++;
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
	// 	ft_usleep(1, philo);
}

void	*routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(philo->sim->input_data->eat_time / 2, philo);
	while (1)
	{
		go_eat(philo);
		if (has_died(philo))
			break ;
		go_sleep(philo);
		if (has_died(philo))
			break ;
		go_think(philo);
		if (has_died(philo))
			break ;
	}
	return (NULL);
}

void	create_philosophers_threads(t_philo *arr_philos, int num_philos)
{
	int	i;

	i = -1;
	while (++i < num_philos)
		pthread_create(&arr_philos[i].thread, NULL, routine, (void *)&arr_philos[i]);
}


void set_dead(t_philo *philos, int num_philos)
{
	int i;

	i = -1;
	while (++i < num_philos)
	{
		pthread_mutex_lock(&philos[i].dead_lock);
		philos[i].dead = true;
		pthread_mutex_unlock(&philos[i].dead_lock);
	}
}


void *dead_monitor(void *arg)
{
	int i;
	int num_philos;
	t_simulation *sim;

	sim = (t_simulation *)arg;
	num_philos = sim->input_data->num_philos;
	while (1)
	{
		i = -1;
		while (++i < num_philos)
		{
			if (has_died(&sim->philos[i]))
			{
				set_dead(sim->philos, num_philos);
				return (NULL);
			}
		}
	}
	return (NULL);
}


void *meals_monitor(void* arg)
{
	int i;
	int num_philos;
	int has_eaten;
	int processed_philos;
	t_philo *philo;
	t_simulation *sim;

	sim = (t_simulation *)arg;
	num_philos = sim->input_data->num_philos;
	while (1)
	{
		i = -1;
		while (++i < num_philos)
		{
			philo = &(sim->philos[i]);
			if (philo->has_eaten)
				continue ;
			// pthread_mutex_lock(&sim->write_lock);
			// printf("%d %d %d\n", philo->id, philo->num_finished_meals, philo->has_eaten);
			// pthread_mutex_unlock(&sim->write_lock);

			has_eaten = philo->num_finished_meals >= sim->input_data->num_meals;
			if (!has_eaten)
				continue ;
			pthread_mutex_lock(&philo->has_eaten_lock);
			philo->has_eaten = 1;
			pthread_mutex_unlock(&philo->has_eaten_lock);

			// print_message(philo, "has eaten all meals");
			// pthread_mutex_lock(&sim->write_lock);
			// printf("%d %d\n", philo->id, philo->num_finished_meals);
			// pthread_mutex_unlock(&sim->write_lock);

			processed_philos = ++sim->processed_philos;
			if (processed_philos == num_philos)
			{
				// pthread_mutex_lock(&sim->write_lock);
				// printf("All philosophers have eaten %d meals\n", sim->input_data->num_meals);
				// pthread_mutex_unlock(&sim->write_lock);
				set_dead(sim->philos, num_philos);
				return (NULL);
			}
		}
	}
	return (NULL);
}


void	create_monitor_threads(pthread_t *dead_monitor_thread, pthread_t *meals_monitor_thread, t_simulation *sim)
{
	pthread_create(dead_monitor_thread, NULL, dead_monitor, (void *)sim);
	if (sim->input_data->num_meals != -1)
		pthread_create(meals_monitor_thread, NULL, meals_monitor, (void *)sim);
}

void	wait_threads(t_simulation *sim, pthread_t *dead_monitor_thread, pthread_t *meals_monitor_thread)
{
	int	i;
	int	num_philos;
	num_philos = sim->input_data->num_philos;
	i = -1;
	while (++i < num_philos)
		pthread_join(sim->philos[i].thread, NULL);
	pthread_join(*dead_monitor_thread, NULL);
	if (sim->input_data->num_meals != -1)
		pthread_join(*meals_monitor_thread, NULL);
}

void	clean_up_data(t_simulation *sim)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&sim->write_lock);
	while (++i < sim->input_data->num_philos) {
		pthread_mutex_destroy(&sim->forks[i]);
		pthread_mutex_destroy(&sim->philos[i].last_meal_time_lock);
		pthread_mutex_destroy(&sim->philos[i].has_eaten_lock);
		pthread_mutex_destroy(&sim->philos[i].dead_lock);
		pthread_mutex_destroy(&sim->philos[i].meal_lock);
	}
	if (sim->forks)
		free(sim->forks);
	if (sim->philos)
		free(sim->philos);
}

int	run_simulation(t_simulation *sim, t_input *input_data)
{
	pthread_t	dead_monitor_thread;
	pthread_t	meals_monitor_thread;

	if (init_simulation(sim, input_data) == 1)
		return (1);
	
	if (init_philos(sim) == 1)
	{
		clean_up_data(sim);
		return (1);
	}
	create_philosophers_threads(sim->philos, input_data->num_philos);
	create_monitor_threads(&dead_monitor_thread, &meals_monitor_thread, sim);
	wait_threads(sim, &dead_monitor_thread, &meals_monitor_thread);
	clean_up_data(sim);
	return (0);
}
