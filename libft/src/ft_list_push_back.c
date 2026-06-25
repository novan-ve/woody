/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_list_push_back.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <novan-ve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/05 15:10:40 by novan-ve      #+#    #+#                 */
/*   Updated: 2022/11/05 15:10:44 by novan-ve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_list_push_back(t_list **list, void *data) {
    t_list *new;

    new = malloc(sizeof(t_list));
    if (!new) {
        return (-1);
    }
    new->next = NULL;
    new->data = data;
    if (*list) {
        ft_list_last(*list)->next = new;
    }
    else {
        *list = new;
    }
    return (0);
}
