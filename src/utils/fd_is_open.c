/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_is_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amann <amann@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 16:20:14 by amann             #+#    #+#             */
/*   Updated: 2022/12/16 16:25:42 by amann            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

bool	fd_is_open(int fd)
{
	struct stat	buf;

	if (fstat(fd, &buf) == -1)
		return (false);
	return (true);
}
