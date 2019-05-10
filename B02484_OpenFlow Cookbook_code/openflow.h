
#ifndef OPENFLOW_H 
#define OPENFLOW_H 
#include "openflow_messages.h"

struct openflow_table { 
   char *name;
   enum ofp_table_config config_flag;
   enum ofp_table_mod_prop_eviction_flag eviction_flag;
   uint8_t vacancy_down;
   uint8_t vacancy_up;
   uint8_t vacancy;
};

/* Switch features. */
struct switch_features {
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
   struct openflow_table *tables;

};

struct openflow_group_table  {
   uint32_t total_group_count;
   struct openflow_group_entry *group_entry;
};

struct openflow_meter_table{
   uint32_t total_meter_count;
   struct openflow_meter_entry *meter_entry;
};

struct openflow_switch {
   struct switch_features features;
   enum ofp_config_flags config_flag;
   struct openflow_group_table *group_table;
   struct openflow_meter_table *meter_table;
};

struct openflow_entry {
   uint64_t cookie;
   uint16_t priority; /* Priority level of flow entry. */
   uint8_t table_id; /* ID of the table */
   long long int creation_time;
   uint16_t idle_timeout; /* Idle timeout from original flow mod. */
   uint16_t hard_timeout; /* Hard timeout from original flow mod. */
   uint64_t packet_count;
   uint64_t byte_count;
   uint32_t buffer_id;    /* Buffered packet to apply to, or
                           * OFP_NO_BUFFER. Not meaningful for
                           *                         * OFPFC_DELETE*. */
   uint16_t flags;       /* Bitmap of OFPFF_* flags. */
   uint16_t importance;  /* Eviction precedence (optional). */

   struct ofp_match match; /* Description of fields. Variable size. */
};

union ofp_action {
    struct ofp_action_output output;
    struct ofp_action_group group;
    struct ofp_action_set_queue queue;
    struct ofp_action_mpls_ttl mpls_ttl;
    struct ofp_action_generic generic;
    struct ofp_action_nw_ttl nw_ttl;
    struct ofp_action_push push;
    struct ofp_action_pop_mpls pop_mpls;
    struct ofp_action_set_field set_field;
    struct ofp_action_experimenter_header experimenter_header;
};

struct ofp_switch_acts{
   struct list list_node;
   struct ofp_action_header header;
   union ofp_action action;
};

/* Bucket for use in groups. */
struct ofp_switch_bucket {
    struct list list_node;
    uint16_t weight;            /* Relative weight, for "select" groups. */
    uint32_t watch_port;      /* Port whose state affects whether this bucket
                                 * is live. Only required for fast failover
                                 * groups. */
    uint32_t watch_group;       /* Group whose state affects whether this
                                 * bucket is live. Only required for fast
                                 * failover groups. */
    struct ofp_switch_acts *ofpacts_list;     /* Series of "struct ofpact"s. */
};

struct openflow_group_entry {
    struct list list_node;
    uint32_t group_id;
    enum ofp_group_type type; /* One of OFPGT_*. */
    uint32_t n_buckets;
    struct ofp_switch_bucket *bucket_list;
}

union ofp_bands {
  uint8_t prec_level;
};
struct ofp_switch_meter_band{
   struct list list_node;
   uint16_t type;       /* One of OFPMBT_*. */
   uint16_t len;        /* Length in bytes of this band. */
   uint32_t rate;       /* Rate for this band. */
   uint32_t burst_size; /* Size of bursts. */
   union ofp_bands band_specific_date;
};

struct openflow_meter_entry {
    struct list list_node;
    uint32_t meter_id;
    uint16_t flags;
    struct ofp_switch_meter_band *band_list;
}

#endif
