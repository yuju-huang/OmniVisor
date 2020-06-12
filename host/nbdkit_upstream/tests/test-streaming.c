/* nbdkit
 * Copyright (C) 2014-2020 Red Hat Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of Red Hat nor the names of its contributors may be
 * used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RED HAT AND CONTRIBUTORS ''AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL RED HAT OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <libnbd.h>

static char data[65536];

int
main (int argc, char *argv[])
{
  pid_t md5pid;
  struct nbd_handle *nbd;
  size_t i;
  int pipefd[2];
  char md5[33];
  int status;

  unlink ("streaming.fifo");
  if (mknod ("streaming.fifo", S_IFIFO | 0600, 0) == -1) {
    perror ("streaming.fifo");
    exit (EXIT_FAILURE);
  }

  nbd = nbd_create ();
  if (nbd == NULL) {
    fprintf (stderr, "%s\n", nbd_get_error ());
    exit (EXIT_FAILURE);
  }

  char *args[] = {
    "nbdkit", "-s", "--exit-with-parent",
    "streaming", "pipe=streaming.fifo", "size=640k", NULL
  };
  if (nbd_connect_command (nbd, args) == -1) {
    fprintf (stderr, "%s\n", nbd_get_error ());
    exit (EXIT_FAILURE);
  }

  /* Fork to run a second process which reads from streaming.fifo and
   * checks that the content is correct.  The test doesn't care about
   * fd leaks, so we don't bother with CLOEXEC.
   */
  if (pipe (pipefd) == -1) {
    perror ("pipe");
    exit (EXIT_FAILURE);
  }

  md5pid = fork ();
  if (md5pid == -1) {
    perror ("fork");
    exit (EXIT_FAILURE);
  }

  if (md5pid == 0) {
    /* Child: run md5sum on the pipe. */
    char *exec_argv[] = { "md5sum", NULL };

    close (0);
    open ("streaming.fifo", O_RDONLY);
    close (1);
    dup2 (pipefd[1], 1);
    close (pipefd[0]);

    execvp ("md5sum", exec_argv);
    perror ("md5sum");
    _exit (EXIT_FAILURE);
  }

  close (pipefd[1]);

  /* Write linearly to the virtual disk. */
  memset (data, 1, sizeof data);
  for (i = 0; i < 10; ++i) {
    if (nbd_pwrite (nbd, data, sizeof data, i * sizeof data, 0) == -1) {
      fprintf (stderr, "%s\n", nbd_get_error ());
      exit (EXIT_FAILURE);
    }
  }

  /* This kills the nbdkit subprocess, which implicitly closes the
   * pipe.
   */
  nbd_close (nbd);

  /* Check the hash computed by the child process. */
  if (read (pipefd[0], md5, 32) != 32) {
    perror ("read");
    exit (EXIT_FAILURE);
  }
  md5[32] = '\0';

  printf ("md5 = %s\n", md5);
  if (strcmp (md5, "2c7f81c580f7f9eb52c1bd2f6f493b6f") != 0) {
    fprintf (stderr, "unexpected hash: %s\n", md5);
    exit (EXIT_FAILURE);
  }

  if (waitpid (md5pid, &status, 0) == -1) {
    perror ("waitpid");
    exit (EXIT_FAILURE);
  }
  if (status != 0) {
    fprintf (stderr, "md5sum subprocess failed\n");
    exit (EXIT_FAILURE);
  }

  exit (EXIT_SUCCESS);
}
