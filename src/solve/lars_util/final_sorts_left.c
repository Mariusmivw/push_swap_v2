/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   final_sorts_left.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mvan-wij <mvan-wij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/11 17:27:22 by mvan-wij      #+#    #+#                 */
/*   Updated: 2022/07/11 17:40:16 by mvan-wij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "operations.h"
#include "lars_util.h"

t_status	sort_group_left_3(t_lars_data *data)
{
	t_3_order const	order = get_group_order_3(data->ps_data->stack_a);

	if (order == O_231 || order == O_312 || order == O_321)
		ps_exec_store(PB, data->ps_data);
	if (order == O_123 || order == O_132 || order == O_312)
		ps_exec_store(RA, data->ps_data);
	if (order == O_132 || order == O_213 || order == O_231 || order == O_321)
		ps_exec_store(SA, data->ps_data);
	ps_exec_store(RA, data->ps_data);
	if (order == O_123 || order == O_132 || order == O_213 || order == O_321)
		ps_exec_store(RA, data->ps_data);
	if (order == O_231 || order == O_312 || order == O_321)
		ps_exec_store(PA, data->ps_data);
	if (order == O_213 || order == O_231 || order == O_312 || order == O_321)
		ps_exec_store(RA, data->ps_data);
	if (order == O_231)
		ps_exec_store(RA, data->ps_data);
	return (ps_get_error());
}

t_status	sort_group_left(int group_size, t_lars_data *data)
{
	if (group_size == 0)
		return (OK);
	if (group_size == 3)
		return (sort_group_left_3(data));
	if (group_size == 1)
		return (ps_exec_store(RA, data->ps_data));
	if (data->ps_data->stack_a->content > data->ps_data->stack_a->next->content)
		ps_exec_store(SA, data->ps_data);
	ps_exec_store(RA, data->ps_data);
	ps_exec_store(RA, data->ps_data);
	return (ps_get_error());
}

t_status	sort_left(t_list **stack_a, t_lars_data *data)
{
	int	len;
	int	size;

	len = ft_lstsize(*stack_a);
	while (len > 0)
	{
		size = group_size(which_group((long)(*stack_a)->content, data), data);
		if (sort_group_left(size, data) != OK)
			return (ps_get_error());
		len -= size;
	}
	return (OK);
}
