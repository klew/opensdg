#ifndef INTERNAL_CONTROL_PROTOCOL_H
#define INTERNAL_CONTROL_PROTOCOL_H

#include <sodium.h>

/* Grid message types */
#define MSG_FORWARD_REMOTE      0
#define MSG_PROTOCOL_VERSION    1
#define MSG_FORWARD_HOLD        1
#define MSG_FORWARD_REPLY       2
#define MSG_FORWARD_ERROR       3
#define MSG_PING                4
#define MSG_PONG                5
#define MSG_CALL_REMOTE         10
#define MSG_REMOTE_REPLY        11
#define MSG_INCOMING_CALL       12
#define MSG_INCOMING_CALL_REPLY 13
#define MSG_PAIR_REMOTE         32
#define MSG_PAIR_REMOTE_REPLY   33

#define FORWARD_REMOTE_MAGIC     0xF09D8C95
#define FORWARD_REMOTE_SIGNATURE "Mdg-NaCl/binary"
#define PROTOCOL_VERSION_MAGIC   0xF09D8CA8
#define PROTOCOL_VERSION_MAJOR   1
#define PROTOCOL_VERSION_MINOR   0

/* Pairing message types */
#define MSG_PAIRING_CHALLENGE   3
#define MSG_PAIRING_RESPONSE    4
#define MSG_PAIRING_RESULT      5

/* Error codes for MSG_FORWARD_ERROR */
#define FORWARD_NO_ERROR     0
#define FORWARD_SERVER_ERROR 1
#define FORWARD_SYNTAX_ERROR 2
#define FORWARD_BAD_TOKEN    3
#define FORWARD_PEER_TIMEOUT 4
#define FORWARD_BAD_VERSION  5

#pragma pack(1)

struct PairingChallenge
{
    unsigned char msgCode;
    unsigned char X[crypto_scalarmult_BYTES];
    unsigned char nonce[32];
    unsigned char Y[32];
};

struct PairingResponse
{
    unsigned char msgCode;
    unsigned char X[crypto_scalarmult_BYTES];
    unsigned char Y[32];
};

struct PairingResult
{
    unsigned char msgCode;
    unsigned char result[32];
};

#pragma pack()

#endif