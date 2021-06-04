/* Copyright (C) 2002-2021 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Jakub Jelinek <jakub@redhat.com>, 2002.

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

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdlib.h>
#include <semaphore.h>

#include <support/check.h>
#include <support/support.h>
#include <support/temp_file.h>
#include <support/xstdio.h>
#include <support/xunistd.h>
#include <support/xthread.h>

static const char *command;
static const char *pidfile;
static char *pidfilename;

static const char sem_name[] = "/tst-cancel7-sem";
static sem_t *sem;

static void *
tf (void *arg)
{
#if 0
  const char *args = " --direct --pidfile ";
  char *cmd = alloca (strlen (command) + strlen (args)
		      + strlen (pidfilename) + 1);

  strcpy (stpcpy (stpcpy (cmd, command), args), pidfilename);
#endif
  char *cmd = xasprintf ("%s --direct --pidfile %s", command, pidfilename);
  system (cmd);
  /* This call should never return.  */
  return NULL;
}

static void
sl (void)
{
#if 0
  FILE *f = fopen (pidfile, "w");
  if (f == NULL)
    exit (1);
#endif
  FILE *f = xfopen (pidfile, "w");

  fprintf (f, "%lld\n", (long long) getpid ());
  fflush (f);

  if (sem_post (sem) != 0)
    FAIL_EXIT1 ("sem_post: %m");

  struct flock fl =
    {
      .l_type = F_WRLCK,
      .l_start = 0,
      .l_whence = SEEK_SET,
      .l_len = 1
    };
  if (fcntl (fileno (f), F_SETLK, &fl) != 0)
    FAIL_EXIT1 ("fcntl (F_SETFL): %m");

  sigset_t ss;
  sigfillset (&ss);
  sigsuspend (&ss);
  exit (0);
}


static void
do_prepare (int argc, char *argv[])
{
  sem = sem_open (sem_name, O_RDWR | O_CREAT | O_EXCL | O_TRUNC, 0666, 0);
  if (sem == SEM_FAILED)
    {
      if (errno != EEXIST)
	FAIL_EXIT1 ("sem_open failed: %m");
      sem = sem_open (sem_name, O_RDWR);
      if (sem == SEM_FAILED)
	FAIL_EXIT1 ("sem_open failed: %m");
    }

  if (command == NULL)
    command = argv[0];

  if (pidfile)
    sl ();

  int fd = create_temp_file ("tst-cancel7-pid-", &pidfilename);
  if (fd == -1)
    FAIL_EXIT1 ("create_temp_file failed: %m");

  xwrite (fd, " ", 1);
  xclose (fd);
}


static int
do_test (void)
{
  pthread_t th = xpthread_create (NULL, tf, NULL);

  do
    nanosleep (&(struct timespec) { .tv_sec = 0, .tv_nsec = 100000000 }, NULL);
  while (access (pidfilename, R_OK) != 0);

  if (sem_wait (sem) != 0)
    FAIL_EXIT1 ("sem_wait: %m");

  xpthread_cancel (th);
  void *r = xpthread_join (th);

  sleep (1);

  FILE *f = xfopen (pidfilename, "r+");

  long long ll;
  if (fscanf (f, "%lld\n", &ll) != 1)
    FAIL_EXIT1 ("fscanf: %m");

  struct flock fl =
    {
      .l_type = F_WRLCK,
      .l_start = 0,
      .l_whence = SEEK_SET,
      .l_len = 1
    };
  if (fcntl (fileno (f), F_GETLK, &fl) != 0)
    FAIL_EXIT1 ("fcntl: %m");

  if (fl.l_type != F_UNLCK)
    {
      printf ("child %lld still running\n", (long long) fl.l_pid);
      if (fl.l_pid == ll)
	kill (fl.l_pid, SIGKILL);

      return 1;
    }

  xfclose (f);

  sem_unlink (sem_name);

  return r != PTHREAD_CANCELED;
}

static void
do_cleanup (void)
{
  FILE *f = fopen (pidfilename, "r+");
  long long ll;

  if (f != NULL && fscanf (f, "%lld\n", &ll) == 1)
    {
      struct flock fl =
	{
	  .l_type = F_WRLCK,
	  .l_start = 0,
	  .l_whence = SEEK_SET,
	  .l_len = 1
	};
      if (fcntl (fileno (f), F_GETLK, &fl) == 0 && fl.l_type != F_UNLCK
	  && fl.l_pid == ll)
	kill (fl.l_pid, SIGKILL);

      fclose (f);
    }

  sem_unlink (sem_name);
}

#define OPT_COMMAND	10000
#define OPT_PIDFILE	10001
#define CMDLINE_OPTIONS \
  { "command", required_argument, NULL, OPT_COMMAND },	\
  { "pidfile", required_argument, NULL, OPT_PIDFILE },
static void
cmdline_process (int c)
{
  switch (c)
    {
    case OPT_COMMAND:
      command = optarg;
      break;
    case OPT_PIDFILE:
      pidfile = optarg;
      break;
    }
}
#define CMDLINE_PROCESS cmdline_process
#define CLEANUP_HANDLER do_cleanup
#define PREPARE do_prepare
#include <support/test-driver.c>
