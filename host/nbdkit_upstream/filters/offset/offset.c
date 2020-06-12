/* nbdkit
 * Copyright (C) 2018-2020 Red Hat Inc.
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
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include <nbdkit-filter.h>

#include "cleanup.h"

static int64_t offset = 0, range = -1;

/* Called for each key=value passed on the command line. */
static int
offset_config (nbdkit_next_config *next, void *nxdata,
               const char *key, const char *value)
{
  if (strcmp (key, "offset") == 0) {
    offset = nbdkit_parse_size (value);
    if (offset == -1)
      return -1;
    return 0;
  }
  else if (strcmp (key, "range") == 0) {
    range = nbdkit_parse_size (value);
    if (range == -1)
      return -1;
    return 0;
  }
  else
    return next (nxdata, key, value);
}

#define offset_config_help \
  "offset=<OFFSET>            The start offset to serve (default 0).\n" \
  "range=<LENGTH>             The total size to serve (default rest of file)."

/* Get the file size. */
static int64_t
offset_get_size (struct nbdkit_next_ops *next_ops, void *nxdata,
                 void *handle)
{
  int64_t real_size = next_ops->get_size (nxdata);

  if (real_size == -1)
    return -1;

  if (range >= 0) {
    if (offset > real_size - range) {
      nbdkit_error ("offset+range (%" PRIi64 "+%" PRIi64 ") "
                    "is larger than the real size (%" PRIi64 ") "
                    "of the underlying file or device",
                    offset, range, real_size);
      return -1;
    }
    return range;
  }
  else if (offset > real_size) {
    nbdkit_error ("offset (%" PRIi64 ") "
                  "is larger than the real size (%" PRIi64 ") "
                  "of the underlying file or device",
                  offset, real_size);
    return -1;
  }
  else
    return real_size - offset;
}

/* Read data. */
static int
offset_pread (struct nbdkit_next_ops *next_ops, void *nxdata,
              void *handle, void *buf, uint32_t count, uint64_t offs,
              uint32_t flags, int *err)
{
  return next_ops->pread (nxdata, buf, count, offs + offset, flags, err);
}

/* Write data. */
static int
offset_pwrite (struct nbdkit_next_ops *next_ops, void *nxdata,
               void *handle,
               const void *buf, uint32_t count, uint64_t offs, uint32_t flags,
               int *err)
{
  return next_ops->pwrite (nxdata, buf, count, offs + offset, flags, err);
}

/* Trim data. */
static int
offset_trim (struct nbdkit_next_ops *next_ops, void *nxdata,
             void *handle, uint32_t count, uint64_t offs, uint32_t flags,
             int *err)
{
  return next_ops->trim (nxdata, count, offs + offset, flags, err);
}

/* Zero data. */
static int
offset_zero (struct nbdkit_next_ops *next_ops, void *nxdata,
             void *handle, uint32_t count, uint64_t offs, uint32_t flags,
             int *err)
{
  return next_ops->zero (nxdata, count, offs + offset, flags, err);
}

/* Extents. */
static int
offset_extents (struct nbdkit_next_ops *next_ops, void *nxdata,
                void *handle, uint32_t count, uint64_t offs, uint32_t flags,
                struct nbdkit_extents *extents, int *err)
{
  size_t i;
  CLEANUP_EXTENTS_FREE struct nbdkit_extents *extents2 = NULL;
  struct nbdkit_extent e;
  int64_t end = range >= 0 ? offset + range : next_ops->get_size (nxdata);

  extents2 = nbdkit_extents_new (offs + offset, end);
  if (extents2 == NULL) {
    *err = errno;
    return -1;
  }
  if (next_ops->extents (nxdata, count, offs + offset,
                         flags, extents2, err) == -1)
    return -1;

  for (i = 0; i < nbdkit_extents_count (extents2); ++i) {
    e = nbdkit_get_extent (extents2, i);
    e.offset -= offset;
    if (nbdkit_add_extent (extents, e.offset, e.length, e.type) == -1) {
      *err = errno;
      return -1;
    }
  }
  return 0;
}

/* Cache data. */
static int
offset_cache (struct nbdkit_next_ops *next_ops, void *nxdata,
              void *handle, uint32_t count, uint64_t offs, uint32_t flags,
              int *err)
{
  return next_ops->cache (nxdata, count, offs + offset, flags, err);
}

static struct nbdkit_filter filter = {
  .name              = "offset",
  .longname          = "nbdkit offset filter",
  .config            = offset_config,
  .config_help       = offset_config_help,
  .get_size          = offset_get_size,
  .pread             = offset_pread,
  .pwrite            = offset_pwrite,
  .trim              = offset_trim,
  .zero              = offset_zero,
  .extents           = offset_extents,
  .cache             = offset_cache,
};

NBDKIT_REGISTER_FILTER(filter)
