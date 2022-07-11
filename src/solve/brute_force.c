/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   brute_force.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mvan-wij <mvan-wij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/08/31 14:13:06 by mvan-wij      #+#    #+#                 */
/*   Updated: 2022/07/07 16:00:44 by mvan-wij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "operations.h"
#include "utils.h"
#include "solve.h"
#include <stdlib.h>
#include <unistd.h>

#define PS_BF_BASE 11

typedef struct s_bf_data {
	int		node;
	int		depth;
	t_list	*done;
	t_cmd	ops[PS_BF_BASE];
}	t_bf_data;

void	undo_all(t_list **cmds, t_list **stack_a, t_list **stack_b)
{
	t_list	*next;

	while ((*cmds) != NULL)
	{
		ps_undo((t_cmd)(*cmds)->content, stack_a, stack_b);
		next = (*cmds)->next;
		free(*cmds);
		*cmds = next;
	}
}

int	power(int base, int pow)
{
	int	ans;

	ans = 1;
	while (pow != 0)
	{
		pow--;
		ans *= base;
	}
	return (ans);
}

t_status	do_setup_ops(t_bf_data *data, t_list **stack_a, t_list **stack_b)
{
	int		p;

	undo_all(&data->done, stack_a, stack_b);
	p = power(PS_BF_BASE, data->depth);
	while (p != 0)
	{
		if (ps_exec(data->ops[(data->node / p) % PS_BF_BASE], stack_a, stack_b))
		{
			if (ft_lstnew_front((void *)data->ops[
						(data->node / p) % PS_BF_BASE], &data->done) == NULL)
			{
				ft_lstclear(&data->done, NULL);
				return (ps_set_error(E_MALLOC));
			}
		}
		p /= PS_BF_BASE;
	}
	return (OK);
}

void	fill_data(t_bf_data *data)
{
	static const t_cmd	ops[PS_BF_BASE] = {PA, PB, SA, SB, SS,
		RA, RB, RR, RRA, RRB, RRR};
	int					i;

	i = 0;
	while (i < PS_BF_BASE)
	{
		data->ops[i] = ops[i];
		i++;
	}
	data->node = 0;
	data->depth = 0;
	data->done = NULL;
}

t_status	can_sort(t_bf_data *data, t_list **stack_a, t_list **stack_b,
				bool *ans)
{
	int	i;
	int	did_something;

	i = 0;
	while (i < PS_BF_BASE)
	{
		did_something = ps_exec(data->ops[i], stack_a, stack_b);
		if (did_something && ps_is_sorted(*stack_a, *stack_b))
		{
			if (ft_lstnew_front((void *)data->ops[i], &data->done) == NULL)
			{
				ft_lstclear(&data->done, NULL);
				return (ps_set_error(E_MALLOC));
			}
			*ans = true;
			return (OK);
		}
		if (did_something)
			ps_undo(data->ops[i], stack_a, stack_b);
		i++;
	}
	*ans = false;
	return (OK);
}

t_status	get_brute_force_sort(t_list **stack_a, t_list **stack_b, int depth,
				t_bf_data *data)
{
	bool	ans_can_sort;

	fill_data(data);
	while (data->depth < depth - 1)
	{
		if (can_sort(data, stack_a, stack_b, &ans_can_sort) != OK)
			return (ps_get_error());
		if (ans_can_sort)
			return (OK);
		if (do_setup_ops(data, stack_a, stack_b) != OK)
			return (ps_get_error());
		data->node++;
		if (ft_get_digit(data->node, data->depth + 1, PS_BF_BASE) == 1)
		{
			ft_set_digit(data->node, data->depth, 0, PS_BF_BASE);
			data->depth++;
		}
	}
	undo_all(&data->done, stack_a, stack_b);
	return (E_TOO_DEEP);
}

t_status	brute_force_sort(t_data *ps_data, int max_depth)
{
	t_bf_data	bf_data;

	if (get_brute_force_sort(&ps_data->stack_a, &ps_data->stack_b, max_depth,
			&bf_data) != OK)
		return (ps_get_error());
	ps_data->ops = bf_data.done;
	return (OK);
}
