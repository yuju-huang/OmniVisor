/* nbdkit
 * Copyright (C) 2013-2020 Red Hat Inc.
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

/* liblzma is a complex interface, so abstract it here. */

#ifndef NBDKIT_XZFILE_H
#define NBDKIT_XZFILE_H

#include <nbdkit-filter.h>

typedef struct xzfile xzfile;

/* Open (and verify) the named xz file. */
extern xzfile *xzfile_open (struct nbdkit_next_ops *next_ops, void *nxdata);

/* Close the file and free up all resources. */
extern void xzfile_close (xzfile *);

/* Get (uncompressed) size of the largest block in the file. */
extern uint64_t xzfile_max_uncompressed_block_size (xzfile *);

/* Get the total uncompressed size of the file. */
extern uint64_t xzfile_get_size (xzfile *);

/* Read the xz file block that contains the byte at 'offset' in the
 * uncompressed file.
 *
 * The uncompressed block of data, which probably begins before the
 * requested byte and ends after it, is returned.  The caller must
 * free it.  NULL is returned if there was an error.
 *
 * The start offset & size of the block relative to the uncompressed
 * file are returned in *start and *size.
 */
extern char *xzfile_read_block (xzfile *xz,
                                struct nbdkit_next_ops *next_ops,
                                void *nxdata, uint32_t flags, int *err,
                                uint64_t offset,
                                uint64_t *start, uint64_t *size);

#endif /* NBDKIT_XZFILE_H */
