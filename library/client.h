#ifndef _INTERNAL_CLIENT_H
#define _INTERNAL_CLIENT_H

#include <errno.h>
#include "events_wrapper.h"

#include "opensdg.h"
#include "tunnel_protocol.h"
#include "control_protocol.pb-c.h"
#include "mainloop.h"

struct osdg_buffer
{
    struct queue_element qe;
};

enum connection_mode
{
    mode_none,
    mode_grid,
    mode_peer,
    mode_pairing
};

struct _osdg_connection
{
  struct client_req          req;
  struct list_element        forwardReq;
  int                        uid;
  SOCKET                     sock;
  osdg_result_t              errorKind;
  unsigned int               errorCode;
  enum connection_mode       mode;
  enum osdg_connection_state state;
  osdg_state_cb_t            changeState;
  osdg_receive_cb_t          receiveData;
  void                      *userData;
  unsigned char              clientPubkey[crypto_box_PUBLICKEYBYTES];     /* Client's public key */
  unsigned char              clientSecret[crypto_box_SECRETKEYBYTES];     /* Client's private key */
  unsigned char              serverPubkey[crypto_box_PUBLICKEYBYTES];     /* Server's public key */
  unsigned char              clientTempPubkey[crypto_box_PUBLICKEYBYTES]; /* Client's short term key pair */
  unsigned char              clientTempSecret[crypto_box_SECRETKEYBYTES];
  unsigned char              serverCookie[curvecp_COOKIEBYTES];
  unsigned char              beforenmData[crypto_box_BEFORENMBYTES];
  unsigned long long         nonce;
  unsigned char             *tunnelId;
  size_t                     tunnelIdSize;
  osdg_connection_t          grid;
  struct list                forwardList;
  char                       protocol[SDG_MAX_PROTOCOL_BYTES];
  unsigned char              pairingResult[32];
  event_t                    completion;
  char                       blocking;
  char                       closing;
  char                       haveBuffers;
  size_t                     bufferSize;
  struct queue               bufferQueue;
  unsigned char             *receiveBuffer;
  unsigned int               bytesReceived;
  unsigned int               bytesLeft;
  unsigned int               discardFirstBytes;
  unsigned int               pingSequence;      /* Ping packet counter, monotonically increases */
  unsigned int               pingInterval;      /* In milliseconds */
  unsigned int               pingDelay;         /* Last PING roundtrip time */
  unsigned long long         lastPing;          /* When the last PING has been sent */
};

int connection_allocate_buffers(struct _osdg_connection *conn);
void *client_get_buffer(struct _osdg_connection *conn);

static inline void client_put_buffer(struct _osdg_connection *client, void *ptr)
{
    queue_put(&client->bufferQueue, ptr);
}

static inline int connection_init(struct _osdg_connection *conn)
{
    int ret = connection_allocate_buffers(conn);

    if (ret)
        return ret;

    conn->discardFirstBytes = 0;
    conn->state             = osdg_connecting;
    conn->pingSequence      = 0;
    conn->pingDelay         = -1;
    /* This causes mainloop_ping() to ignore the connection until
       the very first PING has been sent manually */
    conn->lastPing          = -1LL;

    return 0;
}

void connection_read_data(struct _osdg_connection *conn);
int connection_handle_data(struct _osdg_connection *conn, const unsigned char *data, unsigned int length);
void connection_shutdown(struct _osdg_connection *conn);
void connection_terminate(struct _osdg_connection *conn, enum osdg_connection_state state);
int connection_set_result(struct _osdg_connection *conn, osdg_result_t result);

static inline int connection_in_use(struct _osdg_connection *conn)
{
    return conn->state == osdg_connecting || conn->state == osdg_connected;
}

void connection_set_status(struct _osdg_connection *conn, enum osdg_connection_state state);
osdg_result_t connection_wait(struct _osdg_connection *conn);

int peer_handle_remote_call_reply(struct _osdg_connection *peer, PeerReply *reply);

static inline struct _osdg_connection *get_connection(struct list_element *forwardReq)
{
    return (struct _osdg_connection *)((char *)forwardReq - offsetof(struct _osdg_connection, forwardReq));
}

osdg_result_t connection_ping(struct _osdg_connection *grid);

#endif
