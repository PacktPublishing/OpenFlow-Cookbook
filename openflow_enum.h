#ifndef OPENFLOW_ENUM_H 
#define OPENFLOW_ENUM_H 

typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long int       uint64_t;


typedef enum {FALSE=0, TRUE=1} bool;

/* Hello elements types.
 * */
enum ofp_hello_elem_type {
   OFPHET_VERSIONBITMAP = 1, /* Bitmap of version supported. */
};

/* OpenFlow Version */
enum ofp_version {
    OFP10_VERSION = 0x01,
    OFP11_VERSION = 0x02,
    OFP12_VERSION = 0x03,
    OFP13_VERSION = 0x04,
    OFP14_VERSION = 0x05
};

/* Controller roles. */
enum ofp_controller_role {
   OFPCR_ROLE_NOCHANGE = 0, /* Don't change current role. */
   OFPCR_ROLE_EQUAL = 1,    /* Default role, full access. */
   OFPCR_ROLE_MASTER = 2,   /* Full access, at most one master. */ 
   OFPCR_ROLE_SLAVE = 3,    /* Read-only access. */
}; 

enum ofp_config_flags {
   /* Handling of IP fragments. */
   OFPC_FRAG_NORMAL = 0,     /* No special handling for fragments. */
   OFPC_FRAG_DROP = 1 << 0,  /* Drop fragments. */
   OFPC_FRAG_REASM = 1 << 1, /* Reassemble (only if 
                                OFPC_IP_REASM set). */
   OFPC_FRAG_MASK = 3,
};

/* OpenFlow message types */
enum ofp_type {
   /* Immutable messages. */
   OFPT_HELLO = 0, /* Symmetric message */
   OFPT_ERROR = 1, /* Symmetric message */
   OFPT_ECHO_REQUEST = 2, /* Symmetric message */
   OFPT_ECHO_REPLY = 3, /* Symmetric message */
   OFPT_EXPERIMENTER = 4, /* Symmetric message */
   /* Switch configuration messages. */
   OFPT_FEATURES_REQUEST = 5, /* Controller/switch message */
   OFPT_FEATURES_REPLY = 6, /* Controller/switch message */
   OFPT_GET_CONFIG_REQUEST = 7, /* Controller/switch message */
   OFPT_GET_CONFIG_REPLY = 8, /* Controller/switch message */
   OFPT_SET_CONFIG = 9, /* Controller/switch message */
   /* Asynchronous messages. */
   OFPT_PACKET_IN = 10, /* Async message */
   OFPT_FLOW_REMOVED = 11, /* Async message */
   OFPT_PORT_STATUS = 12, /* Async message */
   /* Controller command messages. */
   OFPT_PACKET_OUT = 13, /* Controller/switch message */
   OFPT_FLOW_MOD = 14, /* Controller/switch message */
   OFPT_GROUP_MOD = 15, /* Controller/switch message */
   OFPT_PORT_MOD = 16, /* Controller/switch message */
   OFPT_TABLE_MOD = 17, /* Controller/switch message */
   /* Multipart messages. */
   OFPT_MULTIPART_REQUEST = 18, /* Controller/switch message */
   OFPT_MULTIPART_REPLY = 19, /* Controller/switch message */
   /* Barrier messages. */
   OFPT_BARRIER_REQUEST = 20, /* Controller/switch message */
   OFPT_BARRIER_REPLY = 21, /* Controller/switch message */
   /* Controller role change request messages. */
   OFPT_ROLE_REQUEST = 24, /* Controller/switch message */
   OFPT_ROLE_REPLY = 25, /* Controller/switch message */
   /* Asynchronous message configuration. */
   OFPT_GET_ASYNC_REQUEST = 26, /* Controller/switch message */
   OFPT_GET_ASYNC_REPLY = 27, /* Controller/switch message */
   OFPT_SET_ASYNC = 28, /* Controller/switch message */
   /* Meters and rate limiters configuration messages. */
   OFPT_METER_MOD = 29, /* Controller/switch message */
   /* Controller role change event messages. */
   OFPT_ROLE_STATUS = 30, /* Async message */
   /* Asynchronous messages. */
   OFPT_TABLE_STATUS = 31, /* Async message */
   /* Request forwarding by the switch. */
   OFPT_REQUESTFORWARD = 32, /* Async message */
   /* Bundle operations (multiple messages as a single operation). */
   OFPT_BUNDLE_CONTROL = 33,
   OFPT_BUNDLE_ADD_MESSAGE = 34,
};

/* Values for 'type' in ofp_error_message. These values are immutable: they
 * * will not change in future versions of the protocol (although new values may
 * * be added). */

enum ofp_error_type {
   OFPET_HELLO_FAILED = 0, /* Hello protocol failed. */
   OFPET_BAD_REQUEST = 1, /* Request was not understood. */
   OFPET_BAD_ACTION = 2, /* Error in action description. */
   OFPET_BAD_INSTRUCTION = 3, /* Error in instruction list. */
   OFPET_BAD_MATCH = 4, /* Error in match. */
   OFPET_FLOW_MOD_FAILED = 5, /* Problem modifying flow entry. */
   OFPET_GROUP_MOD_FAILED = 6, /* Problem modifying group entry. */
   OFPET_PORT_MOD_FAILED = 7, /* Port mod request failed. */
   OFPET_TABLE_MOD_FAILED = 8, /* Table mod request failed. */
   OFPET_QUEUE_OP_FAILED = 9, /* Queue operation failed. */
   OFPET_SWITCH_CONFIG_FAILED = 10, /* Switch config request failed. */
   OFPET_ROLE_REQUEST_FAILED = 11, /* Controller Role request failed. */
   OFPET_METER_MOD_FAILED = 12, /* Error in meter. */
   OFPET_TABLE_FEATURES_FAILED = 13, /* Setting table features failed. */
   OFPET_BAD_PROPERTY = 14, /* Some property is invalid. */
   OFPET_ASYNC_CONFIG_FAILED = 15, /* Asynchronous config request failed. */
   OFPET_FLOW_MONITOR_FAILED = 16, /* Setting flow monitor failed. */
   OFPET_BUNDLE_FAILED = 17, /* Bundle operation failed. */
   OFPET_EXPERIMENTER = 0xffff /* Experimenter error messages. */
};
/* ofp_error_msg 'code' values for OFPET_HELLO_FAILED. 'data' contains an
 * * ASCII text string that may give failure details. */
enum ofp_hello_failed_code {
   OFPHFC_INCOMPATIBLE = 0, /* No compatible version. */
   OFPHFC_EPERM = 1, /* Permissions error. */
};
/* ofp_error_msg 'code' values for OFPET_BAD_REQUEST. 'data' contains at least
 * * the first 64 bytes of the failed request. */
enum ofp_bad_request_code {
   OFPBRC_BAD_VERSION = 0, /* ofp_header.version not supported. */
   OFPBRC_BAD_TYPE = 1, /* ofp_header.type not supported. */
   OFPBRC_BAD_MULTIPART = 2, /* ofp_multipart_request.type not supported. */
   OFPBRC_BAD_EXPERIMENTER = 3, /* Experimenter id not supported
                                 * (in ofp_experimenter_header or
                                 * ofp_multipart_request or
                                 * ofp_multipart_reply). */
   OFPBRC_BAD_EXP_TYPE = 4, /* Experimenter type not supported. */
   OFPBRC_EPERM = 5, /* Permissions error. */
   OFPBRC_BAD_LEN = 6, /* Wrong request length for type. */
   OFPBRC_BUFFER_EMPTY = 7, /* Specified buffer has already been used. */
   OFPBRC_BUFFER_UNKNOWN = 8, /* Specified buffer does not exist. */
   OFPBRC_BAD_TABLE_ID = 9, /* Specified table-id invalid or does not
   * exist. */
   OFPBRC_IS_SLAVE = 10, /* Denied because controller is slave. */
   OFPBRC_BAD_PORT = 11, /* Invalid port. */
   OFPBRC_BAD_PACKET = 12, /* Invalid packet in packet-out. */
   OFPBRC_MULTIPART_BUFFER_OVERFLOW = 13, /* ofp_multipart_request
   overflowed the assigned buffer. */
   OFPBRC_MULTIPART_REQUEST_TIMEOUT = 14, /* Timeout during multipart request. */
   OFPBRC_MULTIPART_REPLY_TIMEOUT = 15, /* Timeout during multipart reply. */
};
/* ofp_error_msg 'code' values for OFPET_BAD_ACTION. 'data' contains at least
	* * the first 64 bytes of the failed request. */
enum ofp_bad_action_code {
   OFPBAC_BAD_TYPE = 0, /* Unknown action type. */
   OFPBAC_BAD_LEN = 1, /* Length problem in actions. */
   OFPBAC_BAD_EXPERIMENTER = 2, /* Unknown experimenter id specified. */
   OFPBAC_BAD_EXP_TYPE = 3, /* Unknown action for experimenter id. */
   OFPBAC_BAD_OUT_PORT = 4, /* Problem validating output port. */
   OFPBAC_BAD_ARGUMENT = 5, /* Bad action argument. */
   OFPBAC_EPERM = 6, /* Permissions error. */
   OFPBAC_TOO_MANY = 7, /* Can't handle this many actions. */
   OFPBAC_BAD_QUEUE = 8, /* Problem validating output queue. */
   OFPBAC_BAD_OUT_GROUP = 9, /* Invalid group id in forward action. */
   OFPBAC_MATCH_INCONSISTENT = 10, /* Action can't apply for this match,
   or Set-Field missing prerequisite. */
   OFPBAC_UNSUPPORTED_ORDER = 11, /* Action order is unsupported for the
   action list in an Apply-Actions instruction */
   OFPBAC_BAD_TAG = 12, /* Actions uses an unsupported
   tag/encap. */
   OFPBAC_BAD_SET_TYPE = 13, /* Unsupported type in SET_FIELD action. */
   OFPBAC_BAD_SET_LEN = 14, /* Length problem in SET_FIELD action. */
   OFPBAC_BAD_SET_ARGUMENT = 15, /* Bad argument in SET_FIELD action. */
};
/* ofp_error_msg 'code' values for OFPET_BAD_INSTRUCTION. 'data' contains at least
	* * the first 64 bytes of the failed request. */
enum ofp_bad_instruction_code {
   OFPBIC_UNKNOWN_INST = 0, /* Unknown instruction. */
   OFPBIC_UNSUP_INST = 1, /* Switch or table does not support the
   instruction. */
   OFPBIC_BAD_TABLE_ID = 2, /* Invalid Table-ID specified. */
   OFPBIC_UNSUP_METADATA = 3, /* Metadata value unsupported by datapath. */
   OFPBIC_UNSUP_METADATA_MASK = 4, /* Metadata mask value unsupported by
   datapath. */
   OFPBIC_BAD_EXPERIMENTER = 5, /* Unknown experimenter id specified. */
   OFPBIC_BAD_EXP_TYPE = 6, /* Unknown instruction for experimenter id. */
   OFPBIC_BAD_LEN = 7, /* Length problem in instructions. */
   OFPBIC_EPERM = 8, /* Permissions error. */
   OFPBIC_DUP_INST = 9, /* Duplicate instruction. */
};
/* ofp_error_msg 'code' values for OFPET_BAD_MATCH. 'data' contains at least
	* * the first 64 bytes of the failed request. */
enum ofp_bad_match_code {
   OFPBMC_BAD_TYPE = 0, /* Unsupported match type specified by the
   match */
   OFPBMC_BAD_LEN = 1, /* Length problem in match. */
   OFPBMC_BAD_TAG = 2, /* Match uses an unsupported tag/encap. */
   OFPBMC_BAD_DL_ADDR_MASK = 3, /* Unsupported datalink addr mask - switch
   does not support arbitrary datalink
   address mask. */
   OFPBMC_BAD_NW_ADDR_MASK = 4, /* Unsupported network addr mask - switch
   does not support arbitrary network
   address mask. */
   OFPBMC_BAD_WILDCARDS = 5, /* Unsupported combination of fields masked
   or omitted in the match. */
   OFPBMC_BAD_FIELD = 6, /* Unsupported field type in the match. */
   OFPBMC_BAD_VALUE = 7, /* Unsupported value in a match field. */
   OFPBMC_BAD_MASK = 8, /* Unsupported mask specified in the match,
   field is not dl-address or nw-address. */
   OFPBMC_BAD_PREREQ = 9, /* A prerequisite was not met. */
   OFPBMC_DUP_FIELD = 10, /* A field type was duplicated. */
   OFPBMC_EPERM = 11, /* Permissions error. */
};
/* ofp_error_msg 'code' values for OFPET_FLOW_MOD_FAILED. 'data' contains
	* * at least the first 64 bytes of the failed request. */
enum ofp_flow_mod_failed_code {
   OFPFMFC_UNKNOWN = 0, /* Unspecified error. */
   OFPFMFC_TABLE_FULL = 1, /* Flow not added because table was full. */
   OFPFMFC_BAD_TABLE_ID = 2, /* Table does not exist */
   OFPFMFC_OVERLAP = 3, /* Attempted to add overlapping flow with
   CHECK_OVERLAP flag set. */
   OFPFMFC_EPERM = 4, /* Permissions error. */
   OFPFMFC_BAD_TIMEOUT = 5, /* Flow not added because of unsupported
   idle/hard timeout. */
   OFPFMFC_BAD_COMMAND = 6, /* Unsupported or unknown command. */
   OFPFMFC_BAD_FLAGS = 7, /* Unsupported or unknown flags. */
   OFPFMFC_CANT_SYNC = 8, /* Problem in table synchronisation. */
   OFPFMFC_BAD_PRIORITY = 9, /* Unsupported priority value. */
};
/* ofp_error_msg 'code' values for OFPET_GROUP_MOD_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request. */
enum ofp_group_mod_failed_code {
   OFPGMFC_GROUP_EXISTS = 0, /* Group not added because a group ADD
   attempted to replace an
   already-present group. */
   OFPGMFC_INVALID_GROUP = 1, /* Group not added because Group
   specified is invalid. */
   OFPGMFC_WEIGHT_UNSUPPORTED = 2, /* Switch does not support unequal load
   sharing with select groups. */
   OFPGMFC_OUT_OF_GROUPS = 3, /* The group table is full. */
   OFPGMFC_OUT_OF_BUCKETS = 4, /* The maximum number of action buckets
   for a group has been exceeded. */
   OFPGMFC_CHAINING_UNSUPPORTED = 5, /* Switch does not support groups that
   forward to groups. */
   OFPGMFC_WATCH_UNSUPPORTED = 6, /* This group cannot watch the watch_port
   or watch_group specified. */
   OFPGMFC_LOOP = 7, /* Group entry would cause a loop. */
   OFPGMFC_UNKNOWN_GROUP = 8, /* Group not modified because a group
   MODIFY attempted to modify a
   non-existent group. */
   OFPGMFC_CHAINED_GROUP = 9, /* Group not deleted because another
   group is forwarding to it. */
   OFPGMFC_BAD_TYPE = 10, /* Unsupported or unknown group type. */
   OFPGMFC_BAD_COMMAND = 11, /* Unsupported or unknown command. */
   OFPGMFC_BAD_BUCKET = 12, /* Error in bucket. */
   OFPGMFC_BAD_WATCH = 13, /* Error in watch port/group. */
   OFPGMFC_EPERM = 14, /* Permissions error. */
};
/* ofp_error_msg 'code' values for OFPET_PORT_MOD_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request. */
enum ofp_port_mod_failed_code {
   OFPPMFC_BAD_PORT = 0, /* Specified port number does not exist. */
   OFPPMFC_BAD_HW_ADDR = 1, /* Specified hardware address does not
   * match the port number. */
   OFPPMFC_BAD_CONFIG = 2, /* Specified config is invalid. */
   OFPPMFC_BAD_ADVERTISE = 3, /* Specified advertise is invalid. */
   OFPPMFC_EPERM = 4, /* Permissions error. */
};
/* ofp_error_msg 'code' values for OFPET_TABLE_MOD_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request. */
enum ofp_table_mod_failed_code {
   OFPTMFC_BAD_TABLE = 0, /* Specified table does not exist. */
   OFPTMFC_BAD_CONFIG = 1, /* Specified config is invalid. */
   OFPTMFC_EPERM = 2, /* Permissions error. */
};
/* ofp_error msg 'code' values for OFPET_QUEUE_OP_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request */
enum ofp_queue_op_failed_code {
   OFPQOFC_BAD_PORT = 0, /* Invalid port (or port does not exist). */
   OFPQOFC_BAD_QUEUE = 1, /* Queue does not exist. */
   OFPQOFC_EPERM = 2, /* Permissions error. */
};
/* ofp_error_msg 'code' values for OFPET_SWITCH_CONFIG_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request. */
enum ofp_switch_config_failed_code {
   OFPSCFC_BAD_FLAGS = 0, /* Specified flags is invalid. */
   OFPSCFC_BAD_LEN = 1, /* Specified len is invalid. */
   OFPSCFC_EPERM = 2, /* Permissions error. */
};
/* ofp_error_msg 'code' values for OFPET_ROLE_REQUEST_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request. */
enum ofp_role_request_failed_code {
   OFPRRFC_STALE = 0, /* Stale Message: old generation_id. */
   OFPRRFC_UNSUP = 1, /* Controller role change unsupported. */
   OFPRRFC_BAD_ROLE = 2, /* Invalid role. */
};
/* ofp_error_msg 'code' values for OFPET_METER_MOD_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request. */
enum ofp_meter_mod_failed_code {
   OFPMMFC_UNKNOWN = 0, /* Unspecified error. */
   OFPMMFC_METER_EXISTS = 1, /* Meter not added because a Meter ADD
   * attempted to replace an existing Meter. */
   OFPMMFC_INVALID_METER = 2, /* Meter not added because Meter specified
   * is invalid. */
   OFPMMFC_UNKNOWN_METER = 3, /* Meter not modified because a Meter
   MODIFY attempted to modify a non-existent
   Meter. */
   OFPMMFC_BAD_COMMAND = 4, /* Unsupported or unknown command. */
   OFPMMFC_BAD_FLAGS = 5, /* Flag configuration unsupported. */
   OFPMMFC_BAD_RATE = 6, /* Rate unsupported. */
   OFPMMFC_BAD_BURST = 7, /* Burst size unsupported. */
   OFPMMFC_BAD_BAND = 8, /* Band unsupported. */
   OFPMMFC_BAD_BAND_VALUE = 9, /* Band value unsupported. */
   OFPMMFC_OUT_OF_METERS = 10, /* No more meters available. */
   OFPMMFC_OUT_OF_BANDS = 11, /* The maximum number of properties
   * for a meter has been exceeded. */
};
/* ofp_error_msg 'code' values for OFPET_TABLE_FEATURES_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request. */
enum ofp_table_features_failed_code {
   OFPTFFC_BAD_TABLE = 0, /* Specified table does not exist. */
   OFPTFFC_BAD_METADATA = 1, /* Invalid metadata mask. */
   OFPTFFC_EPERM = 5, /* Permissions error. */
};
/* ofp_error_msg 'code' values for OFPET_BAD_PROPERTY. 'data' contains at least
 * * the first 64 bytes of the failed request. */
enum ofp_bad_property_code {
   OFPBPC_BAD_TYPE = 0, /* Unknown property type. */
   OFPBPC_BAD_LEN = 1, /* Length problem in property. */
   OFPBPC_BAD_VALUE = 2, /* Unsupported property value. */
   OFPBPC_TOO_MANY = 3, /* Can't handle this many properties. */
   OFPBPC_DUP_TYPE = 4, /* A property type was duplicated. */
   OFPBPC_BAD_EXPERIMENTER = 5, /* Unknown experimenter id specified. */
   OFPBPC_BAD_EXP_TYPE = 6, /* Unknown exp_type for experimenter id. */
   OFPBPC_BAD_EXP_VALUE = 7, /* Unknown value for experimenter id. */
   OFPBPC_EPERM = 8, /* Permissions error. */
};
/* ofp_error_msg 'code' values for OFPET_ASYNC_CONFIG_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request. */
enum ofp_async_config_failed_code {
   OFPACFC_INVALID = 0, /* One mask is invalid. */
   OFPACFC_UNSUPPORTED = 1, /* Requested configuration not supported. */
   OFPACFC_EPERM = 2, /* Permissions error. */
};
/* ofp_error_msg 'code' values for OFPET_FLOW_MONITOR_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request. */
enum ofp_flow_monitor_failed_code {
   OFPMOFC_UNKNOWN = 0, /* Unspecified error. */
   OFPMOFC_MONITOR_EXISTS = 1, /* Monitor not added because a Monitor ADD
   * attempted to replace an existing Monitor. */
   OFPMOFC_INVALID_MONITOR = 2, /* Monitor not added because Monitor specified
   * is invalid. */
   OFPMOFC_UNKNOWN_MONITOR = 3, /* Monitor not modified because a Monitor
   MODIFY attempted to modify a non-existent
   Monitor. */
   OFPMOFC_BAD_COMMAND = 4, /* Unsupported or unknown command. */
   OFPMOFC_BAD_FLAGS = 5, /* Flag configuration unsupported. */
   OFPMOFC_BAD_TABLE_ID = 6, /* Specified table does not exist. */
   OFPMOFC_BAD_OUT = 7, /* Error in output port/group. */
};
/* ofp_error_msg 'code' values for OFPET_BUNDLE_FAILED. 'data' contains
 * * at least the first 64 bytes of the failed request. */
enum ofp_bundle_failed_code {
   OFPBFC_UNKNOWN = 0, /* Unspecified error. */
   OFPBFC_EPERM = 1, /* Permissions error. */
   OFPBFC_BAD_ID = 2, /* Bundle ID doesn't exist. */
   OFPBFC_BUNDLE_EXIST = 3, /* Bundle ID already exist. */
   OFPBFC_BUNDLE_CLOSED = 4, /* Bundle ID is closed. */
   OFPBFC_OUT_OF_BUNDLES = 5, /* Too many bundles IDs. */
   OFPBFC_BAD_TYPE = 6, /* Unsupported or unknown message control type. */
   OFPBFC_BAD_FLAGS = 7, /* Unsupported, unknown, or inconsistent flags. */
   OFPBFC_MSG_BAD_LEN = 8, /* Length problem in included message. */
   OFPBFC_MSG_BAD_XID = 9, /* Inconsistent or duplicate XID. */
   OFPBFC_MSG_UNSUP = 10, /* Unsupported message in this bundle. */
   OFPBFC_MSG_CONFLICT = 11, /* Unsupported message combination in this bundle. */
   OFPBFC_MSG_TOO_MANY = 12, /* Cant handle this many messages in bundle. */
   OFPBFC_MSG_FAILED = 13, /* One message in bundle failed. */
   OFPBFC_TIMEOUT = 14, /* Bundle is taking too long. */
   OFPBFC_BUNDLE_IN_PROGRESS = 15, /* Bundle is locking the resource. */
};
/* Async Config property types.
 * * Low order bit cleared indicates a property for the slave role.
 * * Low order bit set indicates a property for the master/equal role.
 * */
enum ofp_async_config_prop_type {
   OFPACPT_PACKET_IN_SLAVE = 0, /* Packet-in mask for slave. */
   OFPACPT_PACKET_IN_MASTER = 1, /* Packet-in mask for master. */
   OFPACPT_PORT_STATUS_SLAVE = 2, /* Port-status mask for slave. */
   OFPACPT_PORT_STATUS_MASTER = 3, /* Port-status mask for master. */
   OFPACPT_FLOW_REMOVED_SLAVE = 4, /* Flow removed mask for slave. */
   OFPACPT_FLOW_REMOVED_MASTER = 5, /* Flow removed mask for master. */
   OFPACPT_ROLE_STATUS_SLAVE = 6, /* Role status mask for slave. */
   OFPACPT_ROLE_STATUS_MASTER = 7, /* Role status mask for master. */
   OFPACPT_TABLE_STATUS_SLAVE = 8, /* Table status mask for slave. */
   OFPACPT_TABLE_STATUS_MASTER = 9, /* Table status mask for master. */
   OFPACPT_REQUESTFORWARD_SLAVE = 10, /* RequestForward mask for slave. */
   OFPACPT_REQUESTFORWARD_MASTER = 11, /* RequestForward mask for master. */
   OFPTFPT_EXPERIMENTER_SLAVE = 0xFFFE, /* Experimenter for slave. */
   OFPTFPT_EXPERIMENTER_MASTER = 0xFFFF, /* Experimenter for master. */
   OFPACPT_MAX = 14 /* Max no. of entries */
};

/* Special buffer-id to indicate 'no buffer' */
#define OFP_NO_BUFFER 0xffffffff
#define ETHHDR_ADDR_LEN  6
#define MAX_PORT_NAME_LEN  16

/* Why is this packet being sent to the controller? */
enum ofp_packet_in_reason {
   OFPR_TABLE_MISS = 0,   /* No matching flow (table-miss 
                           * flow entry). */
   OFPR_APPLY_ACTION = 1, /* Output to controller in 
                           * apply-actions. */
   OFPR_INVALID_TTL = 2,  /* Packet has invalid TTL */
   OFPR_ACTION_SET = 3,   /* Output to controller in action set. */
   OFPR_GROUP = 4,        /* Output to controller in group bucket. */
   OFPR_PACKET_OUT = 5,   /* Output to controller in packet-out. */
};

/* Why was this flow removed? */
enum ofp_flow_removed_reason {
   OFPRR_IDLE_TIMEOUT = 0,/* Flow idle time exceeded idle_timeout. */
   OFPRR_HARD_TIMEOUT = 1, /* Time exceeded hard_timeout. */
   OFPRR_DELETE = 2,       /* Evicted by a DELETE flow mod. */
   OFPRR_GROUP_DELETE = 3, /* Group was removed. */
   OFPRR_METER_DELETE = 4, /* Meter was removed. */
   OFPRR_EVICTION = 5,     /* Switch eviction to free resources. */
};

/* What changed about the physical port */
enum ofp_port_reason {
   OFPPR_ADD = 0,    /* The port was added. */
   OFPPR_DELETE = 1, /* The port was removed. */
   OFPPR_MODIFY = 2, /* Some attribute of the port has changed. */
};

/* Flags to configure the table. */
enum ofp_table_config {
   OFPTC_DEPRECATED_MASK = 3, /* Deprecated bits */
   OFPTC_EVICTION = 1 << 2, /* Authorise table to evict flows. */
   OFPTC_VACANCY_EVENTS = 1 << 3, /* Enable vacancy events. */
};

/* What changed about the controller role */
enum ofp_controller_role_reason {
   OFPCRR_MASTER_REQUEST = 0, /* Another controller asked 
                               * to be master. */
   OFPCRR_CONFIG = 1,         /* Configuration changed on the 
                               * switch. */
   OFPCRR_EXPERIMENTER = 2,   /* Experimenter data changed. */
}; 

/* What changed about the table */
enum ofp_table_reason {
   OFPTR_VACANCY_DOWN = 3, /* Vacancy down threshold event. */
   OFPTR_VACANCY_UP = 4,   /* Vacancy up threshold event. */
};

/* Eviction flags. */
enum ofp_table_mod_prop_eviction_flag {
   OFPTMPEF_OTHER = 1 << 0,      /* Using other factors. */
   OFPTMPEF_IMPORTANCE = 1 << 1, /* Using flow entry importance. */
   OFPTMPEF_LIFETIME = 1 << 2,   /* Using flow entry lifetime. */
};


/* Table Mod property types.
 * */
enum ofp_table_mod_prop_type {
   OFPTMPT_EVICTION = 0x2, /* Eviction property. */
   OFPTMPT_VACANCY = 0x3, /* Vacancy property. */
   OFPTMPT_EXPERIMENTER = 0xFFFF, /* Experimenter property. */
};

/* Components of a OXM TLV header. */
#define OXM_HEADER__(CLASS, FIELD, HASMASK, LENGTH) \
    (((CLASS) << 16) | ((FIELD) << 9) | ((HASMASK) << 8) | (LENGTH))
#define OXM_HEADER(CLASS, FIELD, LENGTH) \
    OXM_HEADER__(CLASS, FIELD, 0, LENGTH)
#define OXM_HEADER_W(CLASS, FIELD, LENGTH) \
    OXM_HEADER__(CLASS, FIELD, 1, (LENGTH) * 2)
#define OXM_CLASS(HEADER) ((HEADER) >> 16)
#define OXM_FIELD(HEADER) (((HEADER) >> 9) & 0x7f)
#define OXM_TYPE(HEADER) (((HEADER) >> 9) & 0x7fffff)
#define OXM_HASMASK(HEADER) (((HEADER) >> 8) & 1)
#define OXM_LENGTH(HEADER) ((HEADER) & 0xff)
#define OXM_MAKE_WILD_HEADER(HEADER) \
    OXM_HEADER_W(OXM_CLASS(HEADER), OXM_FIELD(HEADER), OXM_LENGTH(HEADER))

/* The match type indicates the match structure (set of fields
 * that compose the match) in use. The match type is placed in the 
 * type field at the beginning of all match structures. The 
 * "OpenFlow Extensible Match" type corresponds to OXM TLV format 
 * described below and must be supported by all OpenFlow switches. 
 * Extensions that define other match types may be published on
 * the ONF wiki. Support for extensions is optional.
 *        */
enum ofp_match_type {
   OFPMT_STANDARD = 0, /* Deprecated. */
   OFPMT_OXM = 1,      /* OpenFlow Extensible Match */
};

/* OXM Class IDs.
 * The high order bit differentiate reserved classes from member 
 * classes. 
 * Classes 0x0000 to 0x7FFF are member classes, allocated by ONF.
 * Classes 0x8000 to 0xFFFE are reserved classes, reserved for
 * standardisation.
 * */
enum ofp_oxm_class {
   OFPXMC_NXM_0 = 0x0000, /* Backward compatibility with NXM */
   OFPXMC_NXM_1 = 0x0001, /* Backward compatibility with NXM */
   OFPXMC_OPENFLOW_BASIC = 0x8000, /* Basic class for OpenFlow */
   OFPXMC_EXPERIMENTER = 0xFFFF,   /* Experimenter class */
}; 
/* Packet's Ethernet type.
 * *
 * * Prereqs: None.
 * *
 * * Format: 16-bit integer in network byte order.
 * *
 * * Masking: Not maskable. */
#define OXM_OF_ETH_TYPE OXM_HEADER (0x8000, OFPXMT_OFB_ETH_TYPE, 2)
/* 802.1Q VID.
 * *
 * * For a packet with an 802.1Q header, this is the VLAN-ID (VID) from the
 * * outermost tag, with the CFI bit forced to 1. For a packet with no 802.1Q
 * * header, this has value OFPVID_NONE.
 * *
 * * Prereqs: None.
 * *
 * * Format: 16-bit integer in network byte order with bit 13 indicating
 * * presence of VLAN header and 3 most-significant bits forced to 0.
 * * Only the lower 13 bits have meaning.
 * *
 * * Masking: Arbitrary masks.
 * *
 * * This field can be used in various ways:
 * *
 * * - If it is not constrained at all, the nx_match matches packets without
 * * an 802.1Q header or with an 802.1Q header that has any VID value.
 * *
 * * - Testing for an exact match with 0x0 matches only packets without
 * * an 802.1Q header.
 * *
 * * - Testing for an exact match with a VID value with CFI=1 matches packets
 * * that have an 802.1Q header with a specified VID.
 * *
* - Testing for an exact match with a nonzero VID value with CFI=0 does
*   * not make sense. The switch may reject this combination.
*   *
*   * - Testing with nxm_value=0, nxm_mask=0x0fff matches packets with no 802.1Q
*   * header or with an 802.1Q header with a VID of 0.
*   *
*   * - Testing with nxm_value=0x1000, nxm_mask=0x1000 matches packets with
*   * an 802.1Q header that has any VID value.
*   */
#define OXM_OF_VLAN_VID OXM_HEADER (0x8000, OFPXMT_OFB_VLAN_VID, 2)
#define OXM_OF_VLAN_VID_W OXM_HEADER_W(0x8000, OFPXMT_OFB_VLAN_VID, 2)

#define OXM_OF_IPV4_SRC OXM_HEADER (0x8000, OFPXMT_OFB_IPV4_SRC, 4)
#define OXM_OF_IPV4_SRC_W OXM_HEADER_W(0x8000, OFPXMT_OFB_IPV4_SRC, 4)
#define OXM_OF_IPV4_DST OXM_HEADER (0x8000, OFPXMT_OFB_IPV4_DST, 4)
#define OXM_OF_IPV4_DST_W OXM_HEADER_W(0x8000, OFPXMT_OFB_IPV4_DST, 4)

/* ARP opcode.
 * *
 * * For an Ethernet+IP ARP packet, the opcode in the ARP header. Always 0
 * * otherwise.
 * *
 * * Prereqs: OXM_OF_ETH_TYPE must match 0x0806 exactly.
 * *
 * * Format: 16-bit integer in network byte order.
 * *
 * * Masking: Not maskable. */
#define OXM_OF_ARP_OP OXM_HEADER (0x8000, OFPXMT_OFB_ARP_OP, 2)
/* For an Ethernet+IP ARP packet, the source or target protocol address
 * * in the ARP header. Always 0 otherwise.
 * *
 * * Prereqs: OXM_OF_ETH_TYPE must match 0x0806 exactly.
 * *
 * * Format: 32-bit integer in network byte order.
 * *
 * * Masking: Arbitrary masks.
 * */
#define OXM_OF_ARP_SPA OXM_HEADER (0x8000, OFPXMT_OFB_ARP_SPA, 4)
#define OXM_OF_ARP_SPA_W OXM_HEADER_W(0x8000, OFPXMT_OFB_ARP_SPA, 4)
#define OXM_OF_ARP_TPA OXM_HEADER (0x8000, OFPXMT_OFB_ARP_TPA, 4)
#define OXM_OF_ARP_TPA_W OXM_HEADER_W(0x8000, OFPXMT_OFB_ARP_TPA, 4)

/* OXM Flow match field types for OpenFlow basic class. */
enum oxm_ofb_match_fields {
   OFPXMT_OFB_IN_PORT = 0,     /* Switch input port. */
   OFPXMT_OFB_IN_PHY_PORT = 1, /* Switch physical input port. */
   OFPXMT_OFB_METADATA = 2,    /* Metadata passed between tables. */
   OFPXMT_OFB_ETH_DST = 3,     /* Ethernet destination address. */
   OFPXMT_OFB_ETH_SRC = 4,     /* Ethernet source address. */
   OFPXMT_OFB_ETH_TYPE = 5,    /* Ethernet frame type. */
   OFPXMT_OFB_VLAN_VID = 6,    /* VLAN id. */
   OFPXMT_OFB_VLAN_PCP = 7,    /* VLAN priority. */
   OFPXMT_OFB_IP_DSCP = 8,     /* IP DSCP (6 bits in ToS field). */
   OFPXMT_OFB_IP_ECN = 9,      /* IP ECN (2 bits in ToS field). */
   OFPXMT_OFB_IP_PROTO = 10,   /* IP protocol. */
   OFPXMT_OFB_IPV4_SRC = 11,    /* IPv4 source address. */
   OFPXMT_OFB_IPV4_DST = 12,    /* IPv4 destination address. */
   OFPXMT_OFB_TCP_SRC = 13,     /* TCP source port. */
   OFPXMT_OFB_TCP_DST = 14,     /* TCP destination port. */
   OFPXMT_OFB_UDP_SRC = 15,     /* UDP source port. */
   OFPXMT_OFB_UDP_DST = 16,     /* UDP destination port. */
   OFPXMT_OFB_SCTP_SRC = 17,    /* SCTP source port. */
   OFPXMT_OFB_SCTP_DST = 18,    /* SCTP destination port. */
   OFPXMT_OFB_ICMPV4_TYPE = 19, /* ICMP type. */
   OFPXMT_OFB_ICMPV4_CODE = 20, /* ICMP code. */
   OFPXMT_OFB_ARP_OP = 21,      /* ARP opcode. */
   OFPXMT_OFB_ARP_SPA = 22,     /* ARP source IPv4 address. */
   OFPXMT_OFB_ARP_TPA = 23,     /* ARP target IPv4 address. */
   OFPXMT_OFB_ARP_SHA = 24,     /* ARP source hardware address. */
   OFPXMT_OFB_ARP_THA = 25,     /* ARP target hardware address. */
   OFPXMT_OFB_IPV6_SRC = 26,    /* IPv6 source address. */
   OFPXMT_OFB_IPV6_DST = 27,    /* IPv6 destination address. */
   OFPXMT_OFB_IPV6_FLABEL = 28, /* IPv6 Flow Label */
   OFPXMT_OFB_ICMPV6_TYPE = 29, /* ICMPv6 type. */
   OFPXMT_OFB_ICMPV6_CODE = 30, /* ICMPv6 code. */
   OFPXMT_OFB_IPV6_ND_TARGET = 31, /* Target address for ND. */
   OFPXMT_OFB_IPV6_ND_SLL = 32, /* Source link-layer for ND. */
   OFPXMT_OFB_IPV6_ND_TLL = 33, /* Target link-layer for ND. */
   OFPXMT_OFB_MPLS_LABEL = 34,  /* MPLS label. */
   OFPXMT_OFB_MPLS_TC = 35,     /* MPLS TC. */
   OFPXMT_OFP_MPLS_BOS = 36,    /* MPLS BoS bit. */
   OFPXMT_OFB_PBB_ISID = 37,    /* PBB I-SID. */
   OFPXMT_OFB_TUNNEL_ID = 38,   /* Logical Port Metadata. */
   OFPXMT_OFB_IPV6_EXTHDR = 39, /* IPv6 Extension Header pseudo-field */
   OFPXMT_OFB_PBB_UCA = 41,     /* PBB UCA header field. */
};

/* The VLAN id is 12-bits, so we can use the entire 16 bits to indicate
 * * special conditions.
 * */
enum ofp_vlan_id {
   OFPVID_PRESENT = 0x1000, /* Bit that indicate that a VLAN id is set */
   OFPVID_NONE = 0x0000, /* No VLAN id was set. */
};

/* Bit definitions for IPv6 Extension Header pseudo-field. */
enum ofp_ipv6exthdr_flags {
   OFPIEH_NONEXT = 1 << 0, /* "No next header" encountered. */
   OFPIEH_ESP = 1 << 1, /* Encrypted Sec Payload header present. */
   OFPIEH_AUTH = 1 << 2, /* Authentication header present. */
   OFPIEH_DEST = 1 << 3, /* 1 or 2 dest headers present. */
   OFPIEH_FRAG = 1 << 4, /* Fragment header present. */
   OFPIEH_ROUTER = 1 << 5, /* Router header present. */
   OFPIEH_HOP = 1 << 6, /* Hop-by-hop header present. */
   OFPIEH_UNREP = 1 << 7, /* Unexpected repeats encountered. */
   OFPIEH_UNSEQ = 1 << 8, /* Unexpected sequencing encountered. */
};

enum ofp_flow_mod_command {
   OFPFC_ADD = 0,           /* New flow. */
   OFPFC_MODIFY = 1,        /* Modify all matching flows. */
   OFPFC_MODIFY_STRICT = 2, /* Modify entry strictly matching 
                             * wildcards and priority. */
   OFPFC_DELETE = 3,        /* Delete all matching flows. */
   OFPFC_DELETE_STRICT = 4, /* Delete entry strictly matching 
                             * wildcards and priority. */
};

enum ofp_flow_mod_flags {
   OFPFF_SEND_FLOW_REM = 1 << 0, /* Send flow removed message 
                                  * when flow expires or is deleted */
   OFPFF_CHECK_OVERLAP = 1 << 1, /* Check for overlapping entries
                                  * * first. */
   OFPFF_RESET_COUNTS = 1 << 2,  /* Reset flow packet and byte
                                  * counts. */
   OFPFF_NO_PKT_COUNTS = 1 << 3, /* Don't keep track of packet count. 
                                  */
   OFPFF_NO_BYT_COUNTS = 1 << 4, /* Don't keep track of byte count. 
                                  */
};

enum ofp_instruction_type {
   OFPIT_GOTO_TABLE = 1, /* Setup the next table in the lookup
   pipeline */
   OFPIT_WRITE_METADATA = 2, /* Setup the metadata field for use later in
   pipeline */
   OFPIT_WRITE_ACTIONS = 3, /* Write the action(s) onto the datapath action
   set */
   OFPIT_APPLY_ACTIONS = 4, /* Applies the action(s) immediately */
   OFPIT_CLEAR_ACTIONS = 5, /* Clears all actions from the datapath
   action set */
   OFPIT_METER = 6, /* Apply meter (rate limiter) */
   OFPIT_EXPERIMENTER = 0xFFFF /* Experimenter instruction */
};


enum ofp_action_type {
   OFPAT_OUTPUT = 0, /* Output to switch port. */
   OFPAT_COPY_TTL_OUT = 11, /* Copy TTL "outwards" -- from next-to-outermost
   to outermost */
   OFPAT_COPY_TTL_IN = 12, /* Copy TTL "inwards" -- from outermost to
   next-to-outermost */
   OFPAT_SET_MPLS_TTL = 15, /* MPLS TTL */
   OFPAT_DEC_MPLS_TTL = 16, /* Decrement MPLS TTL */
   OFPAT_PUSH_VLAN = 17, /* Push a new VLAN tag */
   OFPAT_POP_VLAN = 18, /* Pop the outer VLAN tag */
   OFPAT_PUSH_MPLS = 19, /* Push a new MPLS tag */
   OFPAT_POP_MPLS = 20, /* Pop the outer MPLS tag */
   OFPAT_SET_QUEUE = 21, /* Set queue id when outputting to a port */
   OFPAT_GROUP = 22, /* Apply group. */
   OFPAT_SET_NW_TTL = 23, /* IP TTL. */
   OFPAT_DEC_NW_TTL = 24, /* Decrement IP TTL. */
   OFPAT_SET_FIELD = 25, /* Set a header field using OXM TLV format. */
   OFPAT_PUSH_PBB = 26, /* Push a new PBB service tag (I-TAG) */
   OFPAT_POP_PBB = 27, /* Pop the outer PBB service tag (I-TAG) */
   OFPAT_EXPERIMENTER = 0xffff
};

enum ofp_controller_max_len {
   OFPCML_MAX = 0xffe5, /* maximum max_len value which can be used
   to request a specific byte length. */
   OFPCML_NO_BUFFER = 0xffff /* indicates that no buffering should be
   applied and the whole packet is to be
   sent to the controller. */
};

enum ofp_table {
   /* Last usable table number. */
   OFPTT_MAX = 0xfe,
   /* Fake tables. */
   OFPTT_ALL = 0xff /* Wildcard table used for table config,
   flow stats and flow deletes. */
};

/* Group commands */
enum ofp_group_mod_command {
  OFPGC_ADD = 0,    /* New group. */
  OFPGC_MODIFY = 1, /* Modify all matching groups. */
  OFPGC_DELETE = 2, /* Delete all matching groups. */
}; 

/* Group types. Values in the range [128, 255] are reserved 
 *    for experimental use. */
enum ofp_group_type {
  OFPGT_ALL = 0,      /* All (multicast/broadcast) group. */
  OFPGT_SELECT = 1,   /* Select group. */
  OFPGT_INDIRECT = 2, /* Indirect group. */
  OFPGT_FF = 3,       /* Fast failover group. */
};

enum ofp_group {
  /* Last usable group number. */
  OFPG_MAX = 0xffffff00,
  /* Fake groups. */
  OFPG_ALL = 0xfffffffc, /* Represents all groups for group 
                          * delete commands. */
  OFPG_ANY = 0xffffffff  /* Wildcard group used only for flow stats
                          * requests. Selects all flows regardless 
                          * of group (including flows with no 
                          * group).*/
};

/* Meter commands */
enum ofp_meter_mod_command {
  OFPMC_ADD,    /* New meter. */
  OFPMC_MODIFY, /* Modify specified meter. */
  OFPMC_DELETE, /* Delete specified meter. */
}; 

/* Meter numbering. Flow meters can use any number up to OFPM_MAX. */
enum ofp_meter {
            /* Last usable meter. */
   OFPM_MAX = 0xffff0000,
   /* Virtual meters. */
   OFPM_SLOWPATH = 0xfffffffd,   /* Meter for slow datapath. */
   OFPM_CONTROLLER = 0xfffffffe, /* Meter for controller 
                                  * connection. */
   OFPM_ALL = 0xffffffff,        /* Represents all meters for stat
                                  * requests commands. */
};

/* Meter configuration flags */
enum ofp_meter_flags {
  OFPMF_KBPS = 1 << 0, /* Rate value in kb/s (kilo-bit per 
                        * second). */
  OFPMF_PKTPS = 1 << 1, /* Rate value in packet/sec. */
  OFPMF_BURST = 1 << 2, /* Do burst size. */
  OFPMF_STATS = 1 << 3, /* Collect statistics. */
};

/* Meter band types */
enum ofp_meter_band_type {
  OFPMBT_DROP = 1,             /* Drop packet. */
  OFPMBT_DSCP_REMARK = 2,      /* Remark DSCP in the IP header. */
  OFPMBT_EXPERIMENTER = 0xFFFF /* Experimenter meter band. */
};


#endif
