/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_lstinsert.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mvan-wij <mvan-wij@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/29 13:20:54 by mvan-wij      #+#    #+#                 */
/*   Updated: 2022/07/05 14:29:49 by mvan-wij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

/**
 * Inserts the list @p new into @p lst at index @p i
 * @param lst
 * @param new
 * @param i
 * @return 0 on failure, 1 on success
 */
int	ft_lstinsert(t_list **lst, t_list *new, unsigned int i)
{
	t_list	*l;

	if (lst == NULL || new == NULL)
		return (0);
	if (i == 0)
	{
		ft_lstadd_back(&new, *lst);
		*lst = new;
		return (1);
	}
	l = *lst;
	while (i - 1 > 0 && l != NULL)
	{
		i--;
		l = l->next;
	}
	if (l == NULL)
		return (0);
	ft_lstadd_back(&new, l->next);
	l->next = new;
	return (1);
}
