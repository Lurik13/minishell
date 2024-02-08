/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 10:25:37 by kcouchma          #+#    #+#             */
/*   Updated: 2024/02/08 15:16:26 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*output;

	output = malloc(nmemb * size);
	if (output == NULL)
		return (NULL);
	ft_bzero(output, (nmemb * size));
	return (output);
}

/*
ft_memset(output, 65, nmemb*size); //to test, replace bzero
int	main(void)
{
	printf("%s\n", (char *)ft_calloc(5, 2));
	return (0);
}
*/
