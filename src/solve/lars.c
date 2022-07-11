/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lars.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mvan-wij <mvan-wij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/07 17:49:52 by mvan-wij      #+#    #+#                 */
/*   Updated: 2022/07/11 17:12:59 by mvan-wij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "operations.h"
#include "lars_util/lars_util.h"
#include "libft.h"
#include "utils.h"
#include <stdlib.h>

t_status	split_to_three(t_lars_data *data)
{
	int	top_value_group;
	int	i;

	i = 0;
	while (i < data->total_size)
	{
		top_value_group = which_group((long)(*data->stack_a)->content, data);
		if (anywhere_in_family(top_value_group, &data->families[0]))
			ps_exec_store(RA, data->stack_a, data->stack_b, data->ops);
		else
		{
			ps_exec_store(PB, data->stack_a, data->stack_b, data->ops);
			if (anywhere_in_family(top_value_group, &data->families[2]))
				ps_exec_store(RB, data->stack_a, data->stack_b, data->ops);
		}
		if (ps_get_error() != OK)
			return (ps_get_error());
		i++;
	}
	return (OK);
}

void	fix_group_sizes(t_lars_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < data->families[i].size)
		{
			data->families[i].threes[j].size
				= group_size(data->families[i].threes[j].low, data)
				+ group_size(data->families[i].threes[j].middle, data)
				+ group_size(data->families[i].threes[j].high, data);
			data->families[i].total_size
				+= data->families[i].threes[j].size
				+ group_size(data->families[i].ones[j], data);
			j++;
		}
		i++;
	}
}

t_status	create_data(int num_groups, t_data *ps_data, t_lars_data *data)
{
	data->stack_a = &ps_data->stack_a;
	data->stack_b = &ps_data->stack_b;
	data->ops = &ps_data->ops;
	data->total_size = ft_lstsize(ps_data->stack_a);
	data->num_groups = num_groups;
	ft_bzero(data->families, sizeof(t_family) * 3);
	if (get_families(num_groups, data->families) != OK)
		return (ps_get_error());
	fix_group_sizes(data);
	return (OK);
}

t_status	solve_threes(t_family *group, t_lars_data *data)
{
	t_group_type	which_three;
	int				value_group;
	int				i;

	group->index = 0;
	while (group->index < group->size)
	{
		i = group->threes[group->index].size;
		while (i > 0)
		{
			value_group = which_group((long)(*data->stack_a)->content, data);
			which_three = in_group(value_group, group);
			if (which_three == THREES_LOW || which_three == THREES_MIDDLE)
				ps_exec_store(PB, data->stack_a, data->stack_b, data->ops);
			else
				ps_exec_store(RA, data->stack_a, data->stack_b, data->ops);
			if (which_three == THREES_LOW)
				ps_exec_store(RB, data->stack_a, data->stack_b, data->ops);
			if (ps_get_error() != OK)
				return (ps_get_error());
			i--;
		}
		group->index++;
	}
	return (OK);
}

t_status	sort_lars(t_data *ps_data)
{
	const int	size = ft_lstsize(ps_data->stack_a);
	int			num_groups;
	t_lars_data	data;

	num_groups = size / 3;
	if (num_groups % 12 != 0)
		num_groups += 12 - (num_groups % 12);
	if (create_data(num_groups, ps_data, &data) != OK
		|| split_to_three(&data) != OK
		|| resolve_family(&data.families[0], 0, &data) != OK
		|| resolve_family(&data.families[1], 1, &data) != OK
		|| resolve_family(&data.families[2], 1, &data) != OK
		|| solve_threes(&data.families[0], &data) != OK
		|| solve_threes(&data.families[1], &data) != OK
		|| solve_threes(&data.families[2], &data) != OK
		|| sort_left(&ps_data->stack_a, &data) != OK
		|| sort_right(&ps_data->stack_b, &data) != OK)
	{
		free_families(data.families, 3);
		return (ps_get_error());
	}
	free_families(data.families, 3);
	return (ps_get_error());
}
