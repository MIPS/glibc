/* accept4 with error checking.
   Copyright (C) 2017-2025 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <support/xsocket.h>

#include <stdio.h>
#include <stdlib.h>
#include <support/check.h>

int
xaccept4 (int fd, struct sockaddr *sa, socklen_t *salen, int flags)
{
  int clientfd = accept4 (fd, sa, salen, flags);
  if (clientfd < 0)
    FAIL_EXIT1 ("accept4 (%d, 0x%x): %m", fd, flags);
  return clientfd;
}
