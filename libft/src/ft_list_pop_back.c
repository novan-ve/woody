/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_list_pop_back.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: novan-ve <novan-ve@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/05 15:10:02 by novan-ve      #+#    #+#                 */
/*   Updated: 2022/11/05 15:10:09 by novan-ve      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_list_pop_back(t_list **list) {
    t_list *node;
    
    if (*list) {
        node = *list;
        if (node->next) {
            while (node->next->next) {
                node = node->next;
            }
            free(node->next);
            node->next = NULL;
        }
        else {
            free(*list);
            *list = NULL;
        }
    }
}
