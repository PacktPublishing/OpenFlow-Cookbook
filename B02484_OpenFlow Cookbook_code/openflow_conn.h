
#ifndef OPENFLOW_CONN_H
#define OPENFLOW_CONN_H
#include "openflow_enum.h"
struct ofp_conn {
   int sock_fd;
   enum ofp_controller_role role;
   int miss_send_len;
   uint16_t connection_id;
   /* place holder for asyn config event and mask */
   uint32_t async_config [OFPACPT_MAX];
   uint32_t async_config_mask [OFPACPT_MAX];
   uint64_t generation_id; /* monotonically increasing sequence number
                            * for master election */
};
#endif

