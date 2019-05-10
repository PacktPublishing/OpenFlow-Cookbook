#ifndef OPENFLOW_MESSAGES_H 
#define OPENFLOW_MESSAGES_H 
#include "openflow_enum.h"
#include "openflow_util.h"
struct ofp_header {
   uint8_t version;   /* OFP_VERSION. */
   uint8_t type;      /* One of the OFPT_ constants. */
   uint16_t length;   /* Length including this ofp_header. */
   uint32_t xid;      /* Transaction id associated with this packet.
   * Replies use the same id as was in the request
   * to facilitate pairing. */
};

/* OFPT_ERROR: Error message (datapath -> controller). */
struct ofp_error_msg {
   struct ofp_header header;
   uint16_t type;
   uint16_t code;
   uint8_t data[0]; /* Variable-length data. Interpreted based
   on the type and code. No padding. */
};

struct ofp_role_request  {
   struct ofp_header header;      /* Type OFPT_ROLE_REQUEST/OFPT_ROLE_REPLY. */
   enum ofp_controller_role role; /* One of OFPCR_ROLE_*. */
   uint8_t pad[4];                /* Align to 64 bits. */
   uint64_t generation_id;        /* Master Election Generation Id */
};

/* Switch configuration. */
struct ofp_switch_config {
   struct ofp_header header;
   uint16_t flags;           /* Bitmap of OFPC_* flags. */
   uint16_t miss_send_len;   /* Max bytes of packet that datapath
                                should send to the controller.*/
};

/* Switch features. */
struct ofp_switch_features {
   struct ofp_header header;
   uint64_t datapath_id; /* Datapath unique ID. The lower 48-bits are for
   a MAC address, while the upper 16-bits are
   implementer-defined. */
   uint32_t n_buffers; /* Max packets buffered at once. */
   uint8_t n_tables; /* Number of tables supported by datapath. */
   uint8_t auxiliary_id; /* Identify auxiliary connections */
   uint8_t pad[2]; /* Align to 64-bits. */
   /* Features. */
   uint32_t capabilities; /* Bitmap of support "ofp_capabilities". */
   uint32_t reserved;
};

/* Various reason based properties */
struct ofp_async_config_prop_reasons {
   uint16_t type; /* One of OFPACPT_PACKET_IN_*,
   OFPACPT_PORT_STATUS_*,
   OFPACPT_FLOW_REMOVED_*,
   OFPACPT_ROLE_STATUS_*,
   OFPACPT_TABLE_STATUS_*,
   OFPACPT_REQUESTFORWARD_*. */
   uint16_t length; /* Length in bytes of this property. */
   uint32_t mask; /* Bitmasks of reason values. */
};

/* Common header for all async config Properties */
struct ofp_async_config_prop_header {
   uint16_t type; /* One of OFPACPT_*. */
   uint16_t length; /* Length in bytes of this property. */
};

/* Asynchronous message configuration. */
struct ofp_async_config {
   struct ofp_header header; /* OFPT_GET_ASYNC_REPLY or
                              * OFPT_SET_ASYNC. */
   /* Async config Property list - 0 or more */
   struct ofp_async_config_prop_header properties[0];
};


/* Fields to match against flows */
struct ofp_match {
   uint16_t type; /* One of OFPMT_* */
   uint16_t length; /* Length of ofp_match (excluding padding) */
   /* Followed by:
    * * - Exactly (length - 4) (possibly 0) bytes containing OXM TLVs, then
    * * - Exactly ((length + 7)/8*8 - length) (between 0 and 7) bytes of
    * * all-zero bytes
    * * In summary, ofp_match is padded as needed, to make its overall size
    * * a multiple of 8, to preserve alignement in structures using it.
    * */
   uint8_t oxm_fields[0]; /* 0 or more OXM match fields */
   uint8_t pad[4]; /* Zero bytes - see above for sizing */
};

struct oxm_tlv_field {
   uint32_t oxm_header; 
   uint8_t  *oxm_value;
};

/* Flow removed (datapath -> controller). */
struct ofp_flow_removed {
   struct ofp_header header;
   uint64_t cookie; /* Opaque controller-issued identifier. */
   uint16_t priority; /* Priority level of flow entry. */
   uint8_t reason; /* One of OFPRR_*. */
   uint8_t table_id; /* ID of the table */
   uint32_t duration_sec; /* Time flow was alive in seconds. */
   uint32_t duration_nsec; /* Time flow was alive in nanoseconds beyond
   duration_sec. */
   uint16_t idle_timeout; /* Idle timeout from original flow mod. */
   uint16_t hard_timeout; /* Hard timeout from original flow mod. */
   uint64_t packet_count;
   uint64_t byte_count;
   struct ofp_match match; /* Description of fields. Variable size. */
};

/* Packet received on port (datapath -> controller). */
struct ofp_packet_in {
   struct ofp_header header;
   uint32_t buffer_id; /* ID assigned by datapath. */
   uint16_t total_len; /* Full length of frame. */
   uint8_t reason;     /* Reason packet is being sent 
                        * (one of OFPR_*) */
   uint8_t table_id;   /* ID of the table that was looked up */
   uint64_t cookie;    /* Cookie of the flow entry that was 
                        * looked up. */
   struct ofp_match match; /* Packet metadata. Variable size. */
   /* The variable size and padded match is always followed by:
    * * - Exactly 2 all-zero padding bytes, then
    * * - An Ethernet frame whose length is inferred from header.length.
    * * The padding bytes preceding the Ethernet frame ensure that IP
    * * header (if any) following the Ethernet header is 32-bit aligned.
    * */
   uint8_t pad[2]; /* Align to 64 bit + 16 bit */
   uint8_t data[0]; /* Ethernet frame */
};

 /* A physical port has changed in the datapath */
struct ofp_port_status {
   struct ofp_header header;
   uint8_t reason; /* One of OFPPR_*. */
   uint8_t pad[7]; /* Align to 64-bits. */
   struct ofp_port desc;
};

/* Role status event message. */
struct ofp_role_status {
   struct ofp_header header; /* Type OFPT_ROLE_REQUEST / 
                              * OFPT_ROLE_REPLY. */
   uint32_t role;            /* One of OFPCR_ROLE_*. */
   uint8_t reason;           /* One of OFPCRR_*. */
   uint8_t pad[3];           /* Align to 64 bits. */
   uint64_t generation_id;   /* Master Election Generation Id */
   /* Role Property list */
#if 0
   struct ofp_role_prop_header properties[0];
#endif
};

/* Common header for all Table Mod Properties */
struct ofp_table_mod_prop_header {
   uint16_t type; /* One of OFPTMPT_*. */
   uint16_t length; /* Length in bytes of this property. */
};

/* Body of reply to OFPMP_TABLE_DESC request. */
struct ofp_table_desc {
   uint16_t length; /* Length is padded to 64 bits. */
   uint8_t table_id; /* Identifier of table. Lower numbered tables
   are consulted first. */
   uint8_t pad[1]; /* Align to 32-bits. */
   uint32_t config; /* Bitmap of OFPTC_* values. */
   /* Table Mod Property list - 0 or more. */
   struct ofp_table_mod_prop_header properties[0];
};

/* A table config has changed in the datapath */
struct ofp_table_status {
   struct ofp_header header;
   uint8_t reason;              /* One of OFPTR_*. */
   uint8_t pad[7];              /* Pad to 64 bits */
   struct ofp_table_desc table; /* New table config. */
};

/* Group/Meter request forwarding. */
struct ofp_requestforward_header {
   struct ofp_header header;  /* Type OFPT_REQUESTFORWARD. */
   struct ofp_header request; /* Request being forwarded. */
};

/* Send packet (controller -> datapath). */
struct ofp_packet_out {
   struct ofp_header header;
   uint32_t buffer_id;  /* ID assigned by datapath 
                         * (OFP_NO_BUFFER if none). */
   uint32_t in_port;    /* Packet's input port or OFPP_CONTROLLER. */
   uint16_t actions_len;/* Size of action array in bytes. */
   uint8_t pad[6];
#if 0
   struct ofp_action_header actions[0]; /*Action list - 0 or more. */
   /* The variable size action list is optionally followed by 
    *  * packet data. This data is only present and meaningful if
    *   * buffer_id == -1. */
#endif
   uint8_t data[0]; /* Packet data. The length is inferred
                     * from the length field in the header. */
};

/* Version bitmap Hello Element */
struct ofp_hello_elem_versionbitmap {
   uint16_t type;           /* OFPHET_VERSIONBITMAP. */
   uint16_t length;         /* Length in bytes of this element. */
           /* Followed by:
    *          * - Exactly (length - 4) bytes containing the bitmaps,
    *                   * then Exactly (length + 7)/8*8 - (length) (between 0 
    *                            * and 7) bytes of all-zero bytes */
   uint32_t bitmaps[0]; /* List of bitmaps - supported versions */
};

/* Common header for all Hello Elements */
struct ofp_hello_elem_header {
   uint16_t type; /* One of OFPHET_*. */
   uint16_t length; /* Length in bytes of this element. */
};

/* OFPT_HELLO. This message includes zero or more 
 *    hello elements having variable size.  */
struct ofp_hello {
   struct ofp_header header;
   /* Hello element list */
   struct ofp_hello_elem_header elements[0]; /* List of elements */
};

typedef struct ofp_header ofp_echo_request;
typedef struct ofp_header ofp_echo_reply;

  /* Configure/Modify behavior of a flow table */
struct ofp_table_mod {
   struct ofp_header header;
   uint8_t table_id; /* ID of the table, OFPTT_ALL indicates 
                      * all tables */
   uint8_t pad[3];   /* Pad to 32 bits */
   uint32_t config;  /* Bitmap of OFPTC_* flags */
   /* Table Mod Property list */
   struct ofp_table_mod_prop_header properties[0];
};

/* Eviction table mod Property. Mostly used in 
 *  * OFPMP_TABLE_DESC replies. */
struct ofp_table_mod_prop_eviction {
   uint16_t type;   /* OFPTMPT_EVICTION. */
   uint16_t length; /* Length in bytes of this property. */
   uint32_t flags;  /* Bitmap of OFPTMPEF_* flags */
};

/* Vacancy table mod property */
struct ofp_table_mod_prop_vacancy {
   uint16_t type; /* OFPTMPT_VACANCY. */
   uint16_t length; /* Length in bytes of this property. */
   uint8_t vacancy_down; /* Vacancy threshold when space decreases (%). */
   uint8_t vacancy_up; /* Vacancy threshold when space increases (%). */
   uint8_t vacancy; /* Current vacancy (%) - only in ofp_table_desc. */
   uint8_t pad[1]; /* Align to 64 bits. */
};

/* Instruction header that is common to all instructions. The length includes
 * * the header and any padding used to make the instruction 64-bit aligned.
 * * NB: The length of an instruction *must* always be a multiple of eight. */
struct ofp_instruction_header {
   uint16_t type; /* One of OFPIT_*. */
   uint16_t len; /* Length of this struct in bytes. */
};

/* Instruction structure for OFPIT_GOTO_TABLE */
struct ofp_instruction_goto_table {
   uint16_t type; /* OFPIT_GOTO_TABLE */
   uint16_t len; /* Length is 8. */
   uint8_t table_id; /* Set next table in the lookup pipeline */
   uint8_t pad[3]; /* Pad to 64 bits. */
};

/* Instruction structure for OFPIT_WRITE_METADATA */
struct ofp_instruction_write_metadata {
   uint16_t type; /* OFPIT_WRITE_METADATA */
   uint16_t len; /* Length is 24. */
   uint8_t pad[4]; /* Align to 64-bits */
   uint64_t metadata; /* Metadata value to write */
   uint64_t metadata_mask; /* Metadata write bitmask */
};

/* Action header that is common to all actions. The length includes the
 * * header and any padding used to make the action 64-bit aligned.
 * * NB: The length of an action *must* always be a multiple of eight. */
struct ofp_action_header {
   uint16_t type; /* One of OFPAT_*. */
   uint16_t len; /* Length of action, including this
   header. This is the length of action,
   including any padding to make it
   64-bit aligned. */
};


/* Instruction structure for OFPIT_WRITE/APPLY/CLEAR_ACTIONS */
struct ofp_instruction_actions {
   uint16_t type; /* One of OFPIT_*_ACTIONS */
   uint16_t len; /* Length is padded to 64 bits. */
   uint8_t pad[4]; /* Align to 64-bits */
   struct ofp_action_header actions[0]; /* 0 or more actions associated with
   OFPIT_WRITE_ACTIONS and OFPIT_APPLY_ACTIONS */
};

/* Instruction structure for OFPIT_METER */
struct ofp_instruction_meter {
   uint16_t type; /* OFPIT_METER */
   uint16_t len; /* Length is 8. */
   uint32_t meter_id; /* Meter instance. */
};

/* Instruction structure for experimental instructions */
struct ofp_instruction_experimenter_header {
   uint16_t type; /* OFPIT_EXPERIMENTER. */
   uint16_t len; /* Length is padded to 64 bits. */
   uint32_t experimenter; /* Experimenter ID. */
   /* Experimenter-defined arbitrary additional data. */
};


/* Action structure for OFPAT_OUTPUT, which sends packets out 'port'.
 * * When the 'port' is the OFPP_CONTROLLER, 'max_len' indicates the max
 * * number of bytes to send. A 'max_len' of zero means no bytes of the
 * * packet should be sent. A 'max_len' of OFPCML_NO_BUFFER means that
 * * the packet is not buffered and the complete packet is to be sent to
 * * the controller. */
struct ofp_action_output {
   uint16_t type; /* OFPAT_OUTPUT. */
   uint16_t len; /* Length is 16. */
   uint32_t port; /* Output port. */
   uint16_t max_len; /* Max length to send to controller. */
   uint8_t pad[6]; /* Pad to 64 bits. */
};

/* Action structure for OFPAT_GROUP. */
struct ofp_action_group {
   uint16_t type; /* OFPAT_GROUP. */
   uint16_t len; /* Length is 8. */
   uint32_t group_id; /* Group identifier. */
};

/* Action structure for OFPAT_SET_QUEUE. */
struct ofp_action_set_queue {
   uint16_t type; /* OFPAT_SET_QUEUE. */
   uint16_t len; /* Len is 8. */
   uint32_t queue_id; /* Queue id for the packets. */
};

/* Action structure for OFPAT_SET_MPLS_TTL. */
struct ofp_action_mpls_ttl {
   uint16_t type; /* OFPAT_SET_MPLS_TTL. */
   uint16_t len; /* Length is 8. */
   uint8_t mpls_ttl; /* MPLS TTL */
   uint8_t pad[3];
};

/* Action structure for OFPAT_COPY_TTL_OUT, OFPAT_COPY_TTL_IN,
 * * OFPAT_DEC_MPLS_TTL, OFPAT_DEC_NW_TTL, OFPAT_POP_VLAN and OFPAT_POP_PBB. */
struct ofp_action_generic {
   uint16_t type; /* One of OFPAT_*. */
   uint16_t len; /* Length is 8. */
   uint8_t pad[4]; /* Pad to 64 bits. */
};

/* Action structure for OFPAT_SET_NW_TTL. */
struct ofp_action_nw_ttl {
   uint16_t type; /* OFPAT_SET_NW_TTL. */
   uint16_t len; /* Length is 8. */
   uint8_t nw_ttl; /* IP TTL */
   uint8_t pad[3];
};

/* Action structure for OFPAT_PUSH_VLAN/MPLS/PBB. */
struct ofp_action_push {
   uint16_t type; /* OFPAT_PUSH_VLAN/MPLS/PBB. */
   uint16_t len; /* Length is 8. */
   uint16_t ethertype; /* Ethertype */
   uint8_t pad[2];
};

/* Action structure for OFPAT_POP_MPLS. */
struct ofp_action_pop_mpls {
   uint16_t type; /* OFPAT_POP_MPLS. */
   uint16_t len; /* Length is 8. */
   uint16_t ethertype; /* Ethertype */
   uint8_t pad[2];
};

/* Action structure for OFPAT_SET_FIELD. */
struct ofp_action_set_field {
   uint16_t type; /* OFPAT_SET_FIELD. */
   uint16_t len; /* Length is padded to 64 bits. */
   /* Followed by:
    * * - Exactly oxm_len bytes containing a single OXM TLV, then
    * * - Exactly ((oxm_len + 4) + 7)/8*8 - (oxm_len + 4) (between 0 and 7)
    * * bytes of all-zero bytes
    * */
   uint8_t field[4]; /* OXM TLV - Make compiler happy */
};

/* Action header for OFPAT_EXPERIMENTER.
 * * The rest of the body is experimenter-defined. */
struct ofp_action_experimenter_header {
   uint16_t type; /* OFPAT_EXPERIMENTER. */
   uint16_t len; /* Length is a multiple of 8. */
   uint32_t experimenter; /* Experimenter ID. */
};


/* Flow setup and teardown (controller -> datapath). */
struct ofp_flow_mod {
   struct ofp_header header;
   uint64_t cookie;        /* Opaque controller-issued identifier. */
   uint64_t cookie_mask;   /* Mask used to restrict the cookie bits
                            * that must match when the command is
                            *                          * OFPFC_MODIFY* or OFPFC_DELETE*. A value
                            *                                                   * of 0 indicates no restriction. */
   /* Flow actions. */
   uint8_t table_id;      /* ID of the table to put the flow in.
                           * For OFPFC_DELETE_* commands, OFPTT_ALL
                           *                         * can also be used to delete matching
                           *                                                 * flows from all tables. */
   uint8_t command;       /* One of OFPFC_*. */
   uint16_t idle_timeout; /* Idle time before discarding 
                           * (seconds). */
   uint16_t hard_timeout; /* Max time before discarding (seconds). */
   uint16_t priority;     /* Priority level of flow entry. */
   uint32_t buffer_id;    /* Buffered packet to apply to, or
                           * OFP_NO_BUFFER. Not meaningful for
                           *                         * OFPFC_DELETE*. */
   uint32_t out_port;     /* For OFPFC_DELETE* commands, require
                           * matching entries to include this as an
                           *                         * output port. A value of OFPP_ANY
                           *                                                * indicates no restriction. */
   uint32_t out_group;   /* For OFPFC_DELETE* commands, require
                          * matching entries to include this as an
                          *                        * output group. A value of OFPG_ANY
                          *                                               * indicates no restriction. */
   uint16_t flags;       /* Bitmap of OFPFF_* flags. */
   uint16_t importance;  /* Eviction precedence (optional). */
   struct ofp_match match; /* Fields to match. Variable size. */
                           /* The variable size and padded match is
    *                          * always followed by instructions. */
   struct ofp_instruction_header instructions[0];
   /* Instruction set - 0 or more. The length
    *  * of the instruction set is inferred from
    *   * the length field in the header. */
};		

/* Bucket for use in groups. */
struct ofp_bucket {
  uint16_t len;         /* Length the bucket in bytes, including
                         * this header and any padding to make it
                         *                        * 64-bit aligned. */
  uint16_t weight;      /* Relative weight of bucket. Only defined
                          for select groups. */
  uint32_t watch_port;  /* Port whose state affects whether this
                         * bucket is live. Only required for fast
                         * failover groups. */
  uint32_t watch_group; /* Group whose state affects whether this
                         * bucket is live. Only required for fast
                         * failover groups. */
  uint8_t pad[4];
  struct ofp_action_header actions[0]; /* 0 or more actions 
                                        * associated with the bucket - The action
                                        * list length is inferred from the length
                                        * of the bucket. */
};

/* Group setup and teardown (controller -> datapath). */
struct ofp_group_mod {
  struct ofp_header header;
  uint16_t command;             /* One of OFPGC_*. */
  uint8_t type;                 /* One of OFPGT_*. */
  uint8_t pad;                  /* Pad to 64 bits. */
  uint32_t group_id;            /* Group identifier. */
  struct ofp_bucket buckets[0]; /* The length of the bucket array 
                                 * is inferred from the length field 
                                 * in the header. */
};

/* Common header for all meter bands */
struct ofp_meter_band_header {
  uint16_t type;       /* One of OFPMBT_*. */
  uint16_t len;        /* Length in bytes of this band. */
  uint32_t rate;       /* Rate for this band. */
  uint32_t burst_size; /* Size of bursts. */
};

/* OFPMBT_DROP band - drop packets */
struct ofp_meter_band_drop {
  uint16_t type;       /* OFPMBT_DROP. */
  uint16_t len;        /* Length is 16. */
  uint32_t rate;       /* Rate for dropping packets. */
  uint32_t burst_size; /* Size of bursts. */
  uint8_t pad[4];
};

/* OFPMBT_DSCP_REMARK band - Remark DSCP in the IP header */
struct ofp_meter_band_dscp_remark {
  uint16_t type;       /* OFPMBT_DSCP_REMARK. */
  uint16_t len;        /* Length is 16. */
  uint32_t rate;       /* Rate for remarking packets. */
  uint32_t burst_size; /* Size of bursts. */
  uint8_t prec_level;  /* Number of drop precedence level to add. */
  uint8_t pad[3];
};

/* Meter configuration. OFPT_METER_MOD. */
struct ofp_meter_mod {
  struct ofp_header header;
  uint16_t command;  /* One of OFPMC_*. */
  uint16_t flags;    /* Bitmap of OFPMF_* flags. */
  uint32_t meter_id; /* Meter instance. */
  struct ofp_meter_band_header bands[0]; /* The band list length is
                                          * inferred from the length 
                                          * field in the header. */
};


#endif
