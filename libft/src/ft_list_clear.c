/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_list_clear.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <novan-ve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/05 15:09:04 by novan-ve      #+#    #+#                 */
/*   Updated: 2022/11/05 15:09:05 by novan-ve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void    ft_list_clear(t_list **list) {
    t_list *node;

    while (*list) {
        node = *list;
        *list = (*list)->next;
        free(node);
    }
}
