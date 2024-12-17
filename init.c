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
		arr_philos[i].id = i + 1;
		arr_philos[i].sim = sim;
		arr_philos[i].num_finished_meals = 0;
		arr_philos[i].has_eaten = false;
		arr_philos[i].dead = false;
		arr_philos[i].last_meal_time = sim->start_time;
		pthread_mutex_init(&arr_philos[i].last_meal_time_lock, NULL);
		pthread_mutex_init(&arr_philos[i].num_finished_meals_lock, NULL);
		pthread_mutex_init(&arr_philos[i].has_eaten_lock, NULL);
		pthread_mutex_init(&arr_philos[i].dead_lock, NULL);
		pthread_mutex_init(&arr_philos[i].meal_lock, NULL);
		if ((i == sim->input_data->num_philos - 1) && (sim->input_data->num_philos % 2 != 0))
		{
			arr_philos[i].first_fork_idx = 0;
			arr_philos[i].second_fork_idx = sim->input_data->num_philos - 1;
		}
		else
		{
			arr_philos[i].first_fork_idx = i;
			arr_philos[i].second_fork_idx = (i + 1) % sim->input_data->num_philos;
		}
	}
	sim->philos = arr_philos;
	return (0);
}
