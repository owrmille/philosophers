#include "philosophers.h"

int	init_simulation(t_simulation *sim, t_input *input)
{
	int	i;

	sim->start_time = get_time();
	sim->processed_philos = 0;
	sim->input_data = input;
	sim->forks = malloc(sizeof(pthread_mutex_t) * input->num_philos);
	if (!sim->forks)
	{
		write(2, "Error: malloc failed.\n", 23);
		return (1);
	}
	i = -1;
	pthread_mutex_init(&sim->write_lock, NULL);
	pthread_mutex_init(&sim->lock, NULL);
	while (++i < input->num_philos)
	{
		pthread_mutex_init(&sim->forks[i], NULL);
	}
	return (0);
}

int	init_philos(t_simulation *sim)
{
	t_philo	*arr_philos;
	int		i;

	arr_philos = malloc(sizeof(t_philo) * sim->input_data->num_philos);
	if (!arr_philos)
	{
		write(2, "Error: malloc failed.\n", 23);
		return (1);
	}
	i = -1;
	while (++i < sim->input_data->num_philos)
	{
		init_philo(&arr_philos[i], sim, i);
	}
	sim->philos = arr_philos;
	return (0);
}

int	init_philo(t_philo *philo, t_simulation *sim, int i)
{
	philo->id = i + 1;
	philo->sim = sim;
	philo->num_finished_meals = 0;
	philo->last_meal_time = sim->start_time;
	philo->dead = false;
	philo->has_eaten = false;
	pthread_mutex_init(&philo->lock, NULL);
	if ((i == sim->input_data->num_philos - 1) && (sim->input_data->num_philos % 2 != 0))
	{
		philo->first_fork_idx = 0;
		philo->second_fork_idx = sim->input_data->num_philos - 1;
	}
	else
	{
		philo->first_fork_idx = i;
		philo->second_fork_idx = (i + 1) % sim->input_data->num_philos;
	}
	return (0);
}
