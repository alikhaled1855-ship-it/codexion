/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alikhaled1855gmail.com <alikhaled1855gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 15:51:51 by nahas             #+#    #+#             */
/*   Updated: 2026/11/18 18:11:02 by alikhaled18      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>

typedef struct s_coder	t_coder;

typedef struct s_request
{
	t_coder				*coder;
	long				key;
}						t_request;

typedef struct s_heap
{
	t_request			*requests;
	int					size;
	int					capacity;
}						t_heap;

typedef struct s_dongle
{
	int					is_available;
	pthread_mutex_t		lock;
	long				last_release_time;
	t_heap				waiting;
	pthread_cond_t		cond;
}						t_dongle;

typedef struct s_sim	t_sim;

typedef struct s_coder
{
	int					number;
	t_dongle			*left;
	t_dongle			*right;
	long				last_compile_start;
	int					compiles_done;
	pthread_t			thread;
	t_sim				*sim;
}						t_coder;

typedef struct s_sim
{
	int					number_of_coders;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	int					number_of_compiles_required;
	long				dongle_cooldown;
	int					is_edf;
	long				start_time;
	int					stop;
	pthread_mutex_t		print_lock;
	t_coder				*coders;
	t_dongle			*dongles;
	pthread_t			monitor_thread;
}						t_sim;

int						is_positive_number(char *str);
int						validate_args(int argc, char **argv);
int						init_sim(t_sim *sim, char **argv);
long					get_current_time_ms(void);
void					cleanup_sim(t_sim *sim);
void					*coder_routine(void *arg);
void					log_message(t_sim *sim, int coder_number,
							char *message);
int						take_dongles(t_coder *coder);
void					release_dongles(t_coder *coder);
int						heap_init(t_heap *heap, int capacity);
void					heap_push(t_heap *heap, t_coder *coder, long key);
t_coder					*heap_pop(t_heap *heap);
void					*monitor_routine(void *arg);

#endif
