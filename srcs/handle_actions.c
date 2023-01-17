/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 17:46:20 by timartin          #+#    #+#             */
/*   Updated: 2023/01/17 19:29:21 by timartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_fork(t_philo *philo, pthread_mutex_t *lock, int pos)
{
	pthread_mutex_lock(lock);
	if (data()->forks[pos] == 0)
	{
		data()->forks[pos] = 1;
		philo->fork += 1;
		if (check_all(philo))
			print_action(philo, CYAN, FORK);
	}
	pthread_mutex_unlock(lock);
}

void	eat(t_philo *philo)
{
	while (philo->fork != 2 && check_all(philo))
	{
		check_fork(philo, &data()->m_fork[philo->left], philo->left);
		check_fork(philo, &data()->m_fork[philo->right], philo->right);
	}
	if (check_all(philo))
	{
		philo->last_eat = get_time(0);
		print_action(philo, GREEN, EAT);
		philo->eat_counter += 1;
		check_all(philo);
		while ((get_time(0) - philo->last_eat) < data()->time_eat)
		{
			check_all(philo);
			usleep(1);
		}
		pthread_mutex_lock(&data()->m_fork[philo->left]);
		data()->forks[philo->left] = 0;
		pthread_mutex_unlock(&data()->m_fork[philo->left]);
		pthread_mutex_lock(&data()->m_fork[philo->right]);
		data()->forks[philo->right] = 0;
		pthread_mutex_unlock(&data()->m_fork[philo->right]);
		philo->fork = 0;
		check_all(philo);
	}
}

void	zzz_sleep(t_philo *philo)
{
	if (check_all(philo))
	{
		if (data()->eat_flag < data()->nr_philo)
		{
			philo->start_sleep = get_time(data()->start_time);
			print_action(philo, BLUE, SLEEP);
			while ((get_time(data()->start_time)
					- philo->start_sleep) < data()->time_sleep)
			{
				check_all(philo);
				usleep(1);
			}
		}
	}
}

void	print_action(t_philo *philo, char *color, char *status)
{
	pthread_mutex_lock(&data()->m_print);
	if (check_all(philo))
		printf("%s%lld ms -> Philosopher %d %s\n",
			color, get_time(data()->start_time), philo->id, status);
	pthread_mutex_unlock(&data()->m_print);
}
