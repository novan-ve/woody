/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_list_pop_front.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <novan-ve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/05 15:10:13 by novan-ve      #+#    #+#                 */
/*   Updated: 2022/11/05 15:10:23 by novan-ve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_list_pop_front(t_list **list) {
    t_list *node;

    if (*list) {
        node = *list;
        *list = (*list)->next;
        free(node);
    }
}
