/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_list_clear_data.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <novan-ve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/06 16:41:24 by novan-ve      #+#    #+#                 */
/*   Updated: 2022/11/08 16:35:45 by novan-ve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void    ft_list_clear_data(t_list *list) {
    while (list) {
        if (list->data) {
            free(list->data);
        }
        list = list->next;
    }
}
