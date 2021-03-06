/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mvan-wij <mvan-wij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/24 17:41:46 by mvan-wij      #+#    #+#                 */
/*   Updated: 2022/07/14 12:14:03 by mvan-wij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "libft.h"
# include "error.h"
# include "operations.h"

int			ps_atoi(char *num);
t_status	ps_init_stack(int size, char **nums, t_list **stack);
int			ps_is_sorted(t_list *stack_a, t_list *stack_b);

bool		either(t_cmd first, t_cmd second, t_cmd a, t_cmd b);

#endif
