#ifndef INTERNAL_MAINLOOP_H
#define INTERNAL_MAINLOOP_H

#include "utils.h"

typedef int(*client_req_cb_t)(struct _osdg_connection *);

struct client_req
{
    struct queue_element qe;
    client_req_cb_t      function;
};

void mainloop_events_init(void);
void mainloop_events_shutdown(void);
void mainloop_send_client_request(struct client_req *req, client_req_cb_t function);
void mainloop_handle_client_requests(void);

int mainloop_init(void);
void mainloop_shutdown(void);
void mainloop_client_event(void);
int mainloop_add_connection(struct _osdg_connection *conn);
void mainloop_remove_connection(struct _osdg_connection *conn);
timestamp_t mainloop_ping(struct _osdg_connection** connList, unsigned int connCount);
int mainloop_calc_timeout(timestamp_t nextPing);

extern const struct osdg_main_loop_callbacks *main_cb;

static inline void main_loop_start_cb(void)
{
    if (main_cb)
        main_cb->mainloop_start();
}

static inline void main_loop_stop_cb(void)
{
    if (main_cb)
        main_cb->mainloop_stop();
}

#endif
