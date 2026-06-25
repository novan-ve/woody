/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_list_last.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <novan-ve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/05 15:09:39 by novan-ve      #+#    #+#                 */
/*   Updated: 2022/11/05 15:09:40 by novan-ve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list *ft_list_last(t_list *list) {
    if (!list) {
        return (NULL);
    }
    while (list->next) {
        list = list->next;
    }
    return (list);
}
