#include "philosophers.h"

int	run_simulation(t_simulation *sim, t_input *input_data)
{
	if (init_simulation(sim, input_data) == 1)
		return (1);

	if (init_philos(sim) == 1)
	{
		clean_up_data(sim);
		return (1);
	}
	create_philosophers_threads(sim->philos, input_data->num_philos);
	monitor_philos(sim);
	wait_threads(sim);
	clean_up_data(sim);
	return (0);
}


void	create_philosophers_threads(t_philo *arr_philos, int num_philos)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < num_philos)
	{
		philo = &(arr_philos[i]);
		pthread_create(&(philo->thread), NULL, routine, (void *)philo);
	}
}


void	wait_threads(t_simulation *sim)
{
	int	i;
	int	num_philos;

	num_philos = sim->input_data->num_philos;
	i = -1;
	while (++i < num_philos)
		pthread_join(sim->philos[i].thread, NULL);
}

void	clean_up_data(t_simulation *sim)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&sim->write_lock);
	while (++i < sim->input_data->num_philos)
	{
		pthread_mutex_destroy(&sim->forks[i]);
	}
	if (sim->forks)
		free(sim->forks);
	if (sim->philos)
		free(sim->philos);
}
