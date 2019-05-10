#ifndef OPENFLOW_UTIL_H 
#define OPENFLOW_UTIL_H 
#include "openflow_enum.h"
static inline uint64_t 
htonl_64(uint64_t n)
{
    return htonl(1) == 1 ? n : ((uint64_t) htonl(n) << 32) | htonl(n >> 32);
}

/* Ethernet port description property. */
struct ofp_port_desc_prop_ethernet {
   uint16_t type; /* OFPPDPT_ETHERNET. */
   uint16_t length; /* Length in bytes of this property. */
   uint8_t pad[4]; /* Align to 64 bits. */
   /* Bitmaps of OFPPF_* that describe features. All bits zeroed if
    * * unsupported or unavailable. */
   uint32_t curr; /* Current features. */
   uint32_t advertised; /* Features being advertised by the port. */
   uint32_t supported; /* Features supported by the port. */
   uint32_t peer; /* Features advertised by peer. */
   uint32_t curr_speed; /* Current port bitrate in kbps. */
   uint32_t max_speed; /* Max port bitrate in kbps */
};

/* Description of a port */
struct ofp_port {
   uint32_t port_no;
   uint16_t length;
   uint8_t pad[2];
   uint8_t hw_addr[ETHHDR_ADDR_LEN];
   uint8_t pad2[2]; /* Align to 64 bits. */
   char name[MAX_PORT_NAME_LEN]; /* Null-terminated */
   uint32_t config; /* Bitmap of OFPPC_* flags. */
   uint32_t state; /* Bitmap of OFPPS_* flags. */
   /* Port description property list - 0 or more properties */
   struct ofp_port_desc_prop_ethernet properties[0];
};

struct list {
    struct list *prev;     /* Previous list element. */
    struct list *next;     /* Next list element. */
};


#endif

