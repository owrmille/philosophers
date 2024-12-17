#include "philosophers.h"

int	init_simulation(t_simulation *sim, t_input *input)
{
	int i;
	sim->is_someone_dead = 0;
	sim->processed_philos = 0;
	sim->input_data = input;
	sim->is_fork_occupied = malloc(sizeof(bool) * input->num_philos);
	if (!sim->is_fork_occupied)
	{
		write(2, "Error: malloc failed.\n", 23);
		return (1);
	}
	i = -1;
	while (++i < input->num_philos)
		sim->is_fork_occupied[i] = false;
	pthread_mutex_init(&sim->forks, NULL);
	pthread_mutex_init(&sim->write, NULL);
	pthread_mutex_init(&sim->dead, NULL);
	pthread_mutex_init(&sim->meals, NULL);
	return (0);
}

t_philo	*init_philos(t_simulation *sim)
{
	t_philo	*arr_philos;
	int		i;
	size_t	start_time;

	arr_philos = malloc(sizeof(t_philo) * sim->input_data->num_philos);
	if (!arr_philos)
	{
		write(2, "Error: malloc failed.\n", 23);
		return (NULL);
	}
	start_time = get_time();
	i = -1;
	while (++i < sim->input_data->num_philos)
	{
		arr_philos[i].start_time = start_time;
		arr_philos[i].id = i + 1;
		arr_philos[i].num_finished_meals = 0;
		arr_philos[i].last_meal_time = start_time;
		arr_philos[i].sim = sim;
		arr_philos[i].first_fork_idx = i;
		arr_philos[i].second_fork_idx = (i + 1) % sim->input_data->num_philos;
	}
	return (arr_philos);
}
