/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 17:46:20 by timartin          #+#    #+#             */
/*   Updated: 2023/01/16 17:49:07 by timartin         ###   ########.fr       */
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
			printf("%s%lld ms -> Philosopher %d %s\n",
				CYAN, get_time(data()->start_time), philo->id, FORK);
	}
	pthread_mutex_unlock(lock);
}

void	eat(t_philo *philo)
{
	usleep(100);
	while (philo->fork != 2 && check_all(philo))
	{
		check_fork(philo, &data()->m_fork[philo->left], philo->left);
		check_fork(philo, &data()->m_fork[philo->right], philo->right);
	}
	if (check_all(philo))
	{
		philo->last_eat = get_time(0);
		usleep(200);
		if (check_all(philo))
			printf("%s%lld ms -> Philosopher %d %s\n",
				GREEN, get_time(data()->start_time), philo->id, EAT);
		philo->eat_counter += 1;
		check_all(philo);
		usleep(data()->time_eat * 1000);
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
	if (data()->dead_flag == 0 & data()->eat_flag != data()->nr_philo)
	{
		printf("%s%lld ms -> Philosopher %d %s\n",
			BLUE, get_time(data()->start_time), philo->id, SLEEP);
		usleep(data()->time_sleep * 1000);
	}
}
