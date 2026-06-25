/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_list_create.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <novan-ve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/05 15:09:25 by novan-ve      #+#    #+#                 */
/*   Updated: 2022/11/05 15:09:28 by novan-ve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list *ft_list_create(void *data) {
    t_list *list;

    list = malloc(sizeof(t_list));
    if (!list) {
        return (NULL);
    }
    list->data = data;
    list->next = NULL;
    return (list);
}
