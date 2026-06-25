/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_list_push_front.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <novan-ve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/05 15:10:53 by novan-ve      #+#    #+#                 */
/*   Updated: 2022/11/05 15:10:57 by novan-ve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_list_push_front(t_list **list, void *data) {
    t_list *new;

    new = malloc(sizeof(t_list));
    if (!new) {
        return (-1);
    }
    new->next = *list;
    new->data = data;
    *list = new;
    return (0);
}
