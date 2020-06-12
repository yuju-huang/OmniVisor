/* NBD client library in userspace
 * WARNING: THIS FILE IS GENERATED FROM
 * generator/generator
 * ANY CHANGES YOU MAKE TO THIS FILE WILL BE LOST.
 *
 * Copyright (C) 2013-2020 Red Hat Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef LIBNBD_H
#define LIBNBD_H

/* This is the public interface to libnbd, a client library for
 * accessing Network Block Device (NBD) servers.
 *
 * Please read the libnbd(3) manual page to
 * find out how to use this library.
 */

#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nbd_handle;

#define LIBNBD_TLS_DISABLE                       0
#define LIBNBD_TLS_ALLOW                         1
#define LIBNBD_TLS_REQUIRE                       2

#define LIBNBD_CMD_FLAG_FUA                      1
#define LIBNBD_CMD_FLAG_NO_HOLE                  2
#define LIBNBD_CMD_FLAG_DF                       4
#define LIBNBD_CMD_FLAG_REQ_ONE                  8
#define LIBNBD_CMD_FLAG_FAST_ZERO                16

#define LIBNBD_HANDSHAKE_FLAG_FIXED_NEWSTYLE     1
#define LIBNBD_HANDSHAKE_FLAG_NO_ZEROES          2

#define LIBNBD_ALLOW_TRANSPORT_TCP               1
#define LIBNBD_ALLOW_TRANSPORT_UNIX              2
#define LIBNBD_ALLOW_TRANSPORT_VSOCK             4

#define LIBNBD_AIO_DIRECTION_READ                1
#define LIBNBD_AIO_DIRECTION_WRITE               2
#define LIBNBD_AIO_DIRECTION_BOTH                3
#define LIBNBD_READ_DATA                         1
#define LIBNBD_READ_HOLE                         2
#define LIBNBD_READ_ERROR                        3

extern struct nbd_handle *nbd_create (void);
#define LIBNBD_HAVE_NBD_CREATE 1

extern void nbd_close (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CLOSE 1

extern const char *nbd_get_error (void);
#define LIBNBD_HAVE_NBD_GET_ERROR 1

extern int nbd_get_errno (void);
#define LIBNBD_HAVE_NBD_GET_ERRNO 1

/* These are used for callback parameters.  They are passed
 * by value not by reference.  See CALLBACKS in libnbd(3).
 */
typedef struct {
  int (*callback) (void *user_data, const void *subbuf, size_t count,
                   uint64_t offset, unsigned status, int *error);
  void *user_data;
  void (*free) (void *user_data);
} nbd_chunk_callback;
#define LIBNBD_HAVE_NBD_CHUNK_CALLBACK 1

typedef struct {
  int (*callback) (void *user_data, int *error);
  void *user_data;
  void (*free) (void *user_data);
} nbd_completion_callback;
#define LIBNBD_HAVE_NBD_COMPLETION_CALLBACK 1

typedef struct {
  int (*callback) (void *user_data, const char *context, const char *msg);
  void *user_data;
  void (*free) (void *user_data);
} nbd_debug_callback;
#define LIBNBD_HAVE_NBD_DEBUG_CALLBACK 1

typedef struct {
  int (*callback) (void *user_data, const char *metacontext,
                   uint64_t offset, uint32_t *entries, size_t nr_entries,
                   int *error);
  void *user_data;
  void (*free) (void *user_data);
} nbd_extent_callback;
#define LIBNBD_HAVE_NBD_EXTENT_CALLBACK 1

/* Note NBD_NULL_* are only generated for callbacks which are
 * optional.  (See OClosure in the generator).
 */
#define NBD_NULL_COMPLETION ((nbd_completion_callback) { .callback = NULL })

extern int nbd_set_debug (struct nbd_handle *h, bool debug);
#define LIBNBD_HAVE_NBD_SET_DEBUG 1

extern int nbd_get_debug (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_DEBUG 1

extern int nbd_set_debug_callback (struct nbd_handle *h,
                                   nbd_debug_callback debug_callback);
#define LIBNBD_HAVE_NBD_SET_DEBUG_CALLBACK 1

extern int nbd_clear_debug_callback (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CLEAR_DEBUG_CALLBACK 1

extern int nbd_set_handle_name (struct nbd_handle *h,
                                const char *handle_name);
#define LIBNBD_HAVE_NBD_SET_HANDLE_NAME 1

extern char * nbd_get_handle_name (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_HANDLE_NAME 1

extern int nbd_set_export_name (struct nbd_handle *h,
                                const char *export_name);
#define LIBNBD_HAVE_NBD_SET_EXPORT_NAME 1

extern char * nbd_get_export_name (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_EXPORT_NAME 1

extern int nbd_set_tls (struct nbd_handle *h, int tls);
#define LIBNBD_HAVE_NBD_SET_TLS 1

extern int nbd_get_tls (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_TLS 1

extern int nbd_get_tls_negotiated (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_TLS_NEGOTIATED 1

extern int nbd_set_tls_certificates (struct nbd_handle *h, const char *dir);
#define LIBNBD_HAVE_NBD_SET_TLS_CERTIFICATES 1

extern int nbd_set_tls_verify_peer (struct nbd_handle *h, bool verify);
#define LIBNBD_HAVE_NBD_SET_TLS_VERIFY_PEER 1

extern int nbd_get_tls_verify_peer (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_TLS_VERIFY_PEER 1

extern int nbd_set_tls_username (struct nbd_handle *h, const char *username);
#define LIBNBD_HAVE_NBD_SET_TLS_USERNAME 1

extern char * nbd_get_tls_username (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_TLS_USERNAME 1

extern int nbd_set_tls_psk_file (struct nbd_handle *h, const char *filename);
#define LIBNBD_HAVE_NBD_SET_TLS_PSK_FILE 1

extern int nbd_set_request_structured_replies (struct nbd_handle *h,
                                               bool request);
#define LIBNBD_HAVE_NBD_SET_REQUEST_STRUCTURED_REPLIES 1

extern int nbd_get_request_structured_replies (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_REQUEST_STRUCTURED_REPLIES 1

extern int nbd_get_structured_replies_negotiated (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_STRUCTURED_REPLIES_NEGOTIATED 1

extern int nbd_set_handshake_flags (struct nbd_handle *h, uint32_t flags);
#define LIBNBD_HAVE_NBD_SET_HANDSHAKE_FLAGS 1

extern unsigned nbd_get_handshake_flags (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_HANDSHAKE_FLAGS 1

extern int nbd_add_meta_context (struct nbd_handle *h, const char *name);
#define LIBNBD_HAVE_NBD_ADD_META_CONTEXT 1

extern int nbd_set_uri_allow_transports (struct nbd_handle *h,
                                         uint32_t mask);
#define LIBNBD_HAVE_NBD_SET_URI_ALLOW_TRANSPORTS 1

extern int nbd_set_uri_allow_tls (struct nbd_handle *h, int tls);
#define LIBNBD_HAVE_NBD_SET_URI_ALLOW_TLS 1

extern int nbd_set_uri_allow_local_file (struct nbd_handle *h, bool allow);
#define LIBNBD_HAVE_NBD_SET_URI_ALLOW_LOCAL_FILE 1

extern int nbd_connect_uri (struct nbd_handle *h, const char *uri);
#define LIBNBD_HAVE_NBD_CONNECT_URI 1

extern int nbd_connect_unix (struct nbd_handle *h, const char *unixsocket);
#define LIBNBD_HAVE_NBD_CONNECT_UNIX 1

extern int nbd_connect_vsock (struct nbd_handle *h, uint32_t cid,
                              uint32_t port);
#define LIBNBD_HAVE_NBD_CONNECT_VSOCK 1

extern int nbd_connect_tcp (struct nbd_handle *h, const char *hostname,
                            const char *port);
#define LIBNBD_HAVE_NBD_CONNECT_TCP 1

extern int nbd_connect_socket (struct nbd_handle *h, int sock);
#define LIBNBD_HAVE_NBD_CONNECT_SOCKET 1

extern int nbd_connect_command (struct nbd_handle *h, char **argv);
#define LIBNBD_HAVE_NBD_CONNECT_COMMAND 1

extern int nbd_connect_systemd_socket_activation (struct nbd_handle *h,
                                                  char **argv);
#define LIBNBD_HAVE_NBD_CONNECT_SYSTEMD_SOCKET_ACTIVATION 1

extern int nbd_is_read_only (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_IS_READ_ONLY 1

extern int nbd_can_flush (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CAN_FLUSH 1

extern int nbd_can_fua (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CAN_FUA 1

extern int nbd_is_rotational (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_IS_ROTATIONAL 1

extern int nbd_can_trim (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CAN_TRIM 1

extern int nbd_can_zero (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CAN_ZERO 1

extern int nbd_can_fast_zero (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CAN_FAST_ZERO 1

extern int nbd_can_df (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CAN_DF 1

extern int nbd_can_multi_conn (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CAN_MULTI_CONN 1

extern int nbd_can_cache (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CAN_CACHE 1

extern int nbd_can_meta_context (struct nbd_handle *h,
                                 const char *metacontext);
#define LIBNBD_HAVE_NBD_CAN_META_CONTEXT 1

extern const char * nbd_get_protocol (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_PROTOCOL 1

extern int64_t nbd_get_size (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_SIZE 1

extern int nbd_pread (struct nbd_handle *h, void *buf, size_t count,
                      uint64_t offset, uint32_t flags);
#define LIBNBD_HAVE_NBD_PREAD 1

extern int nbd_pread_structured (struct nbd_handle *h, void *buf,
                                 size_t count, uint64_t offset,
                                 nbd_chunk_callback chunk_callback,
                                 uint32_t flags);
#define LIBNBD_HAVE_NBD_PREAD_STRUCTURED 1

extern int nbd_pwrite (struct nbd_handle *h, const void *buf, size_t count,
                       uint64_t offset, uint32_t flags);
#define LIBNBD_HAVE_NBD_PWRITE 1

extern int nbd_shutdown (struct nbd_handle *h, uint32_t flags);
#define LIBNBD_HAVE_NBD_SHUTDOWN 1

extern int nbd_flush (struct nbd_handle *h, uint32_t flags);
#define LIBNBD_HAVE_NBD_FLUSH 1

extern int nbd_trim (struct nbd_handle *h, uint64_t count, uint64_t offset,
                     uint32_t flags);
#define LIBNBD_HAVE_NBD_TRIM 1

extern int nbd_cache (struct nbd_handle *h, uint64_t count, uint64_t offset,
                      uint32_t flags);
#define LIBNBD_HAVE_NBD_CACHE 1

extern int nbd_zero (struct nbd_handle *h, uint64_t count, uint64_t offset,
                     uint32_t flags);
#define LIBNBD_HAVE_NBD_ZERO 1

extern int nbd_block_status (struct nbd_handle *h, uint64_t count,
                             uint64_t offset,
                             nbd_extent_callback extent_callback,
                             uint32_t flags);
#define LIBNBD_HAVE_NBD_BLOCK_STATUS 1

extern int nbd_poll (struct nbd_handle *h, int timeout);
#define LIBNBD_HAVE_NBD_POLL 1

extern int nbd_aio_connect (struct nbd_handle *h,
                            const struct sockaddr *addr, socklen_t addrlen);
#define LIBNBD_HAVE_NBD_AIO_CONNECT 1

extern int nbd_aio_connect_uri (struct nbd_handle *h, const char *uri);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_URI 1

extern int nbd_aio_connect_unix (struct nbd_handle *h,
                                 const char *unixsocket);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_UNIX 1

extern int nbd_aio_connect_vsock (struct nbd_handle *h, uint32_t cid,
                                  uint32_t port);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_VSOCK 1

extern int nbd_aio_connect_tcp (struct nbd_handle *h, const char *hostname,
                                const char *port);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_TCP 1

extern int nbd_aio_connect_socket (struct nbd_handle *h, int sock);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_SOCKET 1

extern int nbd_aio_connect_command (struct nbd_handle *h, char **argv);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_COMMAND 1

extern int nbd_aio_connect_systemd_socket_activation (struct nbd_handle *h,
                                                      char **argv);
#define LIBNBD_HAVE_NBD_AIO_CONNECT_SYSTEMD_SOCKET_ACTIVATION 1

extern int64_t nbd_aio_pread (struct nbd_handle *h, void *buf, size_t count,
                              uint64_t offset,
                              nbd_completion_callback completion_callback,
                              uint32_t flags);
#define LIBNBD_HAVE_NBD_AIO_PREAD 1

extern int64_t nbd_aio_pread_structured (struct nbd_handle *h, void *buf,
                                         size_t count, uint64_t offset,
                                         nbd_chunk_callback chunk_callback,
                                         nbd_completion_callback completion_callback,
                                         uint32_t flags);
#define LIBNBD_HAVE_NBD_AIO_PREAD_STRUCTURED 1

extern int64_t nbd_aio_pwrite (struct nbd_handle *h, const void *buf,
                               size_t count, uint64_t offset,
                               nbd_completion_callback completion_callback,
                               uint32_t flags);
#define LIBNBD_HAVE_NBD_AIO_PWRITE 1

extern int nbd_aio_disconnect (struct nbd_handle *h, uint32_t flags);
#define LIBNBD_HAVE_NBD_AIO_DISCONNECT 1

extern int64_t nbd_aio_flush (struct nbd_handle *h,
                              nbd_completion_callback completion_callback,
                              uint32_t flags);
#define LIBNBD_HAVE_NBD_AIO_FLUSH 1

extern int64_t nbd_aio_trim (struct nbd_handle *h, uint64_t count,
                             uint64_t offset,
                             nbd_completion_callback completion_callback,
                             uint32_t flags);
#define LIBNBD_HAVE_NBD_AIO_TRIM 1

extern int64_t nbd_aio_cache (struct nbd_handle *h, uint64_t count,
                              uint64_t offset,
                              nbd_completion_callback completion_callback,
                              uint32_t flags);
#define LIBNBD_HAVE_NBD_AIO_CACHE 1

extern int64_t nbd_aio_zero (struct nbd_handle *h, uint64_t count,
                             uint64_t offset,
                             nbd_completion_callback completion_callback,
                             uint32_t flags);
#define LIBNBD_HAVE_NBD_AIO_ZERO 1

extern int64_t nbd_aio_block_status (struct nbd_handle *h, uint64_t count,
                                     uint64_t offset,
                                     nbd_extent_callback extent_callback,
                                     nbd_completion_callback completion_callback,
                                     uint32_t flags);
#define LIBNBD_HAVE_NBD_AIO_BLOCK_STATUS 1

extern int nbd_aio_get_fd (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_GET_FD 1

extern unsigned nbd_aio_get_direction (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_GET_DIRECTION 1

extern int nbd_aio_notify_read (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_NOTIFY_READ 1

extern int nbd_aio_notify_write (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_NOTIFY_WRITE 1

extern int nbd_aio_is_created (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_IS_CREATED 1

extern int nbd_aio_is_connecting (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_IS_CONNECTING 1

extern int nbd_aio_is_ready (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_IS_READY 1

extern int nbd_aio_is_processing (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_IS_PROCESSING 1

extern int nbd_aio_is_dead (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_IS_DEAD 1

extern int nbd_aio_is_closed (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_IS_CLOSED 1

extern int nbd_aio_command_completed (struct nbd_handle *h, uint64_t cookie);
#define LIBNBD_HAVE_NBD_AIO_COMMAND_COMPLETED 1

extern int64_t nbd_aio_peek_command_completed (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_PEEK_COMMAND_COMPLETED 1

extern int nbd_aio_in_flight (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_AIO_IN_FLIGHT 1

extern const char * nbd_connection_state (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_CONNECTION_STATE 1

extern const char * nbd_get_package_name (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_PACKAGE_NAME 1

extern const char * nbd_get_version (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_GET_VERSION 1

extern int nbd_kill_subprocess (struct nbd_handle *h, int signum);
#define LIBNBD_HAVE_NBD_KILL_SUBPROCESS 1

extern int nbd_supports_tls (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_SUPPORTS_TLS 1

extern int nbd_supports_uri (struct nbd_handle *h);
#define LIBNBD_HAVE_NBD_SUPPORTS_URI 1

/* "base" namespace */
#define LIBNBD_NAMESPACE_BASE "base:"

/* "base" namespace contexts */
#define LIBNBD_CONTEXT_BASE_ALLOCATION "base:allocation"

/* "base:allocation" context related constants */
#define LIBNBD_STATE_HOLE                     1
#define LIBNBD_STATE_ZERO                     2

#ifdef __cplusplus
}
#endif

#endif /* LIBNBD_H */
