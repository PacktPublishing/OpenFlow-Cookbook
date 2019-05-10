#include <errno.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "ofp_global.h"
#include "openflow_conn.h"
#include "openflow_enum.h"
#include "openflow.h"
#include "openflow_macro.h"
#include "openflow_util.h"
#include "openflow_messages.h"

uint8_t 
send_openflow_message (struct ofp_conn *connection, uint16_t length,
                      uint8_t type, uint32_t xid, void *buf)
{
  /* Encode the OpenFlow header first */
  int n=0;
  struct ofp_header *header = NULL;
  header = (struct ofp_header *) buf; 
  header->version = OFP14_VERSION;
  header->type = type;
  header->length = htons(length); 
  header->xid = htonl(xid); 
  /* send the buffer via the socket */ 
  n = write(connection->sock_fd,buf,length+sizeof(struct ofp_header));
  if (n < 0) 
    printf("ERROR writing to socket\n");
  return 0;
}

uint8_t 
send_error_message(struct ofp_conn *connection, uint32_t xid, 
                   enum ofp_error_type error_type, uint8_t error_code)
{
   struct ofp_error_msg *error_msg = NULL;
   uint8_t ret;
   char *buf = malloc (sizeof(struct ofp_error_msg));

   error_msg = (struct ofp_error_msg *) buf; 
   memset(error_msg,0,sizeof(struct ofp_error_msg));
   error_msg->type = htons(error_type);
   error_msg->code = htons(error_code);
   ret = send_openflow_message (connection, 
                               (sizeof(struct ofp_error_msg) - sizeof (struct ofp_header)), 
                               OFPT_ERROR, xid, buf);
   free (buf);
   return ret;
}

uint8_t
process_role_request_message (struct ofp_conn *connection, char *buf)
{
   struct ofp_role_request *role_request;
   struct ofp_role_request *role_reply;
   uint8_t ret;

   role_request = (struct ofp_role_request *) buf;

   /* Validate the generation_id. Here the generation_is_defined i
    * and cached_generation_id are global variables */
   if (generation_is_defined && (int64_t) 
       (role_request->generation_id - cached_generation_id) < 0) {

     /* If it is a stale message then send error message */
     send_error_message(connection, role_request->header.xid, OFPET_ROLE_REQUEST_FAILED, OFPRRFC_STALE);
   }
   else {
      if (role_request->role != OFPCR_ROLE_NOCHANGE) {
         char *ret_buf = malloc (sizeof(struct ofp_role_request));
         cached_generation_id = role_request->generation_id;
         generation_is_defined = TRUE;

         /* set the role of the connection */
         connection->role = role_request->role;

         /* form the reply message and send it to controller*/
         role_reply = (struct ofp_role_request *) ret_buf;
         memset(role_reply,0,sizeof(struct ofp_role_request));

         role_reply->role = htonl(role_request->role);
         role_reply->generation_id = htonl_64(cached_generation_id);
         
         ret = send_openflow_message (connection, 
                                     (sizeof(struct ofp_role_request) - sizeof (struct ofp_header)), 
                                      OFPT_ROLE_REPLY, role_request->header.xid, ret_buf);
         free (ret_buf);
      }
   }
   return 0;
}

uint8_t
process_features_request_message (struct ofp_conn *connection, char *buf) 
{
   struct ofp_switch_features *features_request;
   struct ofp_switch_features *features_reply;
   uint8_t ret;
   char *ret_buf = malloc (sizeof(struct ofp_switch_features));

   features_request = (struct ofp_switch_features *) buf;

   features_reply = (struct ofp_switch_features *) ret_buf;
   memset(features_reply,0,sizeof(struct ofp_switch_features));

   features_reply->datapath_id = htonl_64(ofp_switch.features.datapath_id);
   features_reply->n_buffers = htonl(ofp_switch.features.n_buffers);
   features_reply->n_tables = ofp_switch.features.n_tables;
   features_reply->capabilities = htonl (ofp_switch.features.capabilities);
   features_reply->auxiliary_id = 0; /* Assuming main connection */

   ret = send_openflow_message (connection, 
                               (sizeof(struct ofp_switch_features) - sizeof (struct ofp_header)), 
                               OFPT_FEATURES_REPLY, features_request->header.xid, ret_buf);
   /* The send_openflow_message function adds the OF header */
   free (ret_buf);
   return ret; 
}

uint8_t 
process_get_config_message (struct ofp_conn *connection, char *buf) 
{
   struct ofp_header *get_config_request;
   struct ofp_switch_config *config_reply;
   char *ret_buf;
   uint8_t ret;
   get_config_request = (struct ofp_header *) buf;

   ret_buf = malloc (sizeof(struct ofp_switch_config));
   config_reply = (struct ofp_switch_config *) ret_buf;
   memset(config_reply ,0,sizeof(struct ofp_switch_config));
   config_reply->flags = htons(ofp_switch.config_flag);
   config_reply->miss_send_len = 128; /* default value */
   ret = send_openflow_message (connection, 
                               (sizeof(struct ofp_switch_config) - sizeof (struct ofp_header)), 
                               OFPT_GET_CONFIG_REPLY, get_config_request->xid, ret_buf);
   free (ret_buf);
   return ret; 
  /* The send_openflow_message function adds the OF header */
}

uint8_t
send_hello_message (struct ofp_conn *connection)
{
   /* Form a echo Reply message and send it back via the same connection */
   char *buf = NULL;
   struct ofp_hello *hello_msg;
   uint8_t ret;
   uint32_t xid = htonl(0);

   buf = malloc (sizeof(struct ofp_hello));
   hello_msg = (struct ofp_hello *) buf;
   memset(hello_msg,0,sizeof(struct ofp_hello));
   
   hello_msg->elements[0].type = htons(OFPHET_VERSIONBITMAP);
   hello_msg->elements[0].length = sizeof(struct ofp_hello_elem_versionbitmap);
   /* Assuming that this switch supports only OF1.4 spec */
   //hello_msg->elements[0].bitmaps[0]= htonl(0x00000020);
   ret = send_openflow_message (connection,(sizeof(struct ofp_hello) - sizeof (struct ofp_header)) ,
                               OFPT_HELLO, xid, buf);

   free (buf);
   return ret;
}

uint8_t
process_echo_request(struct ofp_conn *connection, char *buf)
{
   /* Form a echo Reply message and send it back via the same connection */
   ofp_echo_request *echo_request = NULL;
   ofp_echo_request *echo_reply;
   char *ret_buf;
   uint8_t ret;
   echo_request = (struct ofp_header *) buf;

   ret_buf = malloc (sizeof(struct ofp_header));
   echo_reply = (struct ofp_header *)buf;

   memset(echo_reply,0,sizeof(struct ofp_header));
   
   ret = send_openflow_message (connection, 
                               0, OFPT_ECHO_REPLY, echo_request->xid, buf);

   free (buf);
   return ret;
}

uint8_t
send_echo_request(struct ofp_conn *connection)
{
   /* Form a echo Reply message and send it back via the same connection */
   char *buf=NULL;
   ofp_echo_request *echo_request;
   uint8_t ret;

   buf = malloc (sizeof(struct ofp_header));
   echo_request = (struct ofp_header *)  buf; 
   memset(echo_request,0,sizeof(struct ofp_header));
   
   ret = send_openflow_message (connection, 
                               0, OFPT_ECHO_REQUEST, echo_request->xid, buf);

   free (buf);
   return ret;
}
uint8_t
process_async_get_config_request(struct ofp_conn *connection, char *buf)
{
   /* Form a echo Reply message and send it back via the same connection */
   struct ofp_header *async_request;
   struct ofp_async_config *async_config_reply; 
   char *ret_buf = NULL;
   uint8_t ret;

   async_request = (struct ofp_header *) buf;

   ret_buf = malloc (sizeof(struct ofp_async_config));
   async_config_reply = (struct ofp_async_config *) ret_buf; 
   memset(async_config_reply,0,sizeof(struct ofp_async_config));
   
   async_config_reply->properties[0].type = OFPACPT_PORT_STATUS_SLAVE;
   async_config_reply->properties[0].length = sizeof (uint32_t);
   
   ret = send_openflow_message (connection, 
                               0, OFPT_GET_CONFIG_REPLY, async_request->xid, ret_buf);

   free (ret_buf);
   return ret;
}

uint8_t
process_async_set_config_request(struct ofp_conn *connection, char *buf)
{
   /* Form a echo Reply message and send it back via the same connection */
   struct ofp_async_config *async_set_config; 
   char *ret_buf = NULL;
   uint8_t ret;

   async_set_config = (struct ofp_async_config *) buf;

   /* set the switch configuration here */ 
   
   return ret;
}

uint8_t 
send_packet_in_message(struct ofp_conn *connection, uint8_t *packet,
                       uint16_t pkt_length) 
{
   char *send_buf=NULL;
   struct ofp_packet_in *packet_in = NULL;
   uint8_t ret;
   uint32_t xid = htonl(0);

   send_buf = malloc (sizeof(struct ofp_packet_in));
   packet_in = (struct ofp_packet_in *)send_buf;
   memset(packet_in ,0,sizeof(struct ofp_packet_in));

   packet_in->buffer_id = htonl(OFP_NO_BUFFER);
   packet_in->total_len = htons(pkt_length);
   packet_in->reason = OFPR_TABLE_MISS;
   packet_in->cookie = 0xFFFF;
   memcpy (packet_in->data, packet, pkt_length);

   ret = send_openflow_message (connection, 
                               (sizeof(struct ofp_packet_in) - sizeof (struct ofp_header)), 
                               OFPT_PACKET_IN, xid, send_buf);
   
   free (send_buf);
   return ret;
}

uint8_t 
fill_match_field_buffer (struct ofp_match *dest_match, struct ofp_match *src_match)
{
   struct oxm_tlv_field *oxm_tlv;
   struct oxm_tlv_field *src_oxm_tlv;
   uint32_t oxm_header;
   uint32_t oxm_value;
   dest_match->type = htons (src_match->type); 
   if (src_match->type == OFPMT_OXM) {
      if (src_match->length) {
         dest_match->length = htons(sizeof(struct ofp_match) - 4);
         oxm_tlv = malloc (sizeof(struct oxm_tlv_field));
         dest_match->oxm_fields[0] = oxm_tlv;
         src_oxm_tlv = (struct oxm_tlv_field *)src_match->oxm_fields[0];
         memcpy(&oxm_header, src_oxm_tlv->oxm_header, sizeof(uint32_t));
         oxm_tlv->oxm_header = htonl (oxm_header);
         if ((OXM_FIELD(src_oxm_tlv->oxm_header) == OFPXMT_OFB_IN_PORT) ||
             (OXM_FIELD(src_oxm_tlv->oxm_header) == OXM_OF_IPV4_SRC) ||
             (OXM_FIELD(src_oxm_tlv->oxm_header) == OXM_OF_IPV4_DST) ||
             (OXM_FIELD(src_oxm_tlv->oxm_header) == OXM_OF_ARP_SPA)  ||
             (OXM_FIELD(src_oxm_tlv->oxm_header) == OXM_OF_ARP_TPA)) {
             memcpy(&oxm_value, src_oxm_tlv->oxm_value, sizeof(uint32_t));
             oxm_tlv->oxm_value = malloc (sizeof(uint32_t));
             oxm_tlv->oxm_value = htonl (oxm_value);
         }
         else if ((OXM_FIELD(src_oxm_tlv->oxm_header) == OXM_OF_ETH_TYPE) ||
                  (OXM_FIELD(src_oxm_tlv->oxm_header) == OXM_OF_VLAN_VID)) {
             memcpy(&oxm_value, src_oxm_tlv->oxm_value, sizeof(uint16_t));
             oxm_tlv->oxm_value = malloc (sizeof(uint16_t));
             oxm_tlv->oxm_value = oxm_value;
         }
      }
   }

}

uint8_t 
send_flow_removed_message(struct ofp_conn *connection, struct openflow_entry *flow_entry,
                         enum ofp_flow_removed_reason reason) 
{
   char *buf = NULL;
   struct ofp_flow_removed *flow_removed_msg = NULL;
   uint8_t ret;
   uint32_t xid = htonl(0);
   long long int duration_in_msecs = 0;
   uint32_t duration_in_secs = 0;
   uint32_t duration_in_nsecs = 0;
   uint8_t match_ptr = NULL;

   buf = malloc (sizeof(struct ofp_flow_removed));
   flow_removed_msg = (struct ofp_flow_removed *) buf;
   memset(flow_removed_msg,0,sizeof(struct ofp_flow_removed));

   flow_removed_msg->cookie = htonl_64 (flow_entry->cookie); 
   flow_removed_msg->priority = htons (flow_entry->priority);
   flow_removed_msg->reason = reason;
   flow_removed_msg->table_id = flow_entry->table_id;
   duration_in_msecs = time_msec() - flow_entry->creation_time;
   duration_in_secs = duration_in_msecs / 1000;
   duration_in_nsecs = (duration_in_msecs % 1000) * (1000 * 1000);

   flow_removed_msg->duration_sec =  htonl(duration_in_secs);
   flow_removed_msg->duration_nsec = htonl (duration_in_nsecs);
   flow_removed_msg->idle_timeout =  htons(flow_entry->idle_timeout);
   flow_removed_msg->hard_timeout =  htons(flow_entry->hard_timeout);
   flow_removed_msg->packet_count =  htonl_64 (flow_entry->packet_count);
   flow_removed_msg->byte_count =  htonl_64 (flow_entry->byte_count);
   fill_match_field_buffer(&flow_removed_msg->match,&flow_entry->match);

   /* TODO check the size */
   ret = send_openflow_message (connection, 
                               (sizeof(struct ofp_flow_removed) - sizeof (struct ofp_header)), 
                               OFPT_FLOW_REMOVED, xid, buf);
   
   free (buf);
   return ret;
}

uint8_t 
send_port_status_message(struct ofp_conn *connection, struct ofp_port *port,
                         enum ofp_port_reason reason) 
{
   char *buf = NULL;
   struct ofp_port_status *port_status_msg = NULL;
   uint8_t ret;
   uint32_t xid = htonl(0);

   buf = malloc (sizeof(struct ofp_port_status));
   port_status_msg = (struct ofp_port_status *)buf;
   memset(port_status_msg ,0,sizeof(struct ofp_port_status));

   port_status_msg->reason = reason;
   port_status_msg->desc.port_no = htonl(port->port_no); 
   port_status_msg->desc.length = htons(port->length); 
   memcpy (port_status_msg->desc.hw_addr, port->hw_addr, ETHHDR_ADDR_LEN);
   memcpy (port_status_msg->desc.name, port->name, MAX_PORT_NAME_LEN);
   port_status_msg->desc.config = htonl(port->config);
   port_status_msg->desc.state = htonl(port->state);
   port_status_msg->desc.properties[0].type = htons(port->properties[0].type);
   port_status_msg->desc.properties[0].length = htons(port->properties[0].length);
   port_status_msg->desc.properties[0].curr = htonl(port->properties[0].curr);
   port_status_msg->desc.properties[0].advertised = htonl(port->properties[0].advertised);
   port_status_msg->desc.properties[0].supported = htonl(port->properties[0].supported);
   port_status_msg->desc.properties[0].peer = htonl(port->properties[0].peer);
   port_status_msg->desc.properties[0].curr_speed = htonl(port->properties[0].curr_speed);
   port_status_msg->desc.properties[0].max_speed = htonl(port->properties[0].max_speed);
   /* TODO check the size */
   ret = send_openflow_message (connection, 
                               (sizeof(struct ofp_port_status) - sizeof (struct ofp_header)), 
                               OFPT_PORT_STATUS, xid, buf);
   
   free (buf);
   return ret;
}

uint8_t 
send_role_status_message(struct ofp_conn *connection, enum ofp_controller_role role,
                         enum ofp_port_reason reason) 
{
   char *buf = NULL;
   struct ofp_role_status *role_status_msg = NULL;
   uint8_t ret;
   uint32_t xid = htonl(0);

   buf = malloc (sizeof(struct ofp_role_status));
   role_status_msg = (struct ofp_role_status *)buf;
   memset(role_status_msg,0,sizeof(struct ofp_role_status));

   role_status_msg->role = htonl(role);
   role_status_msg->reason = reason;
   role_status_msg->generation_id = htonl_64(connection->generation_id);

   /* TODO check the size */
   ret = send_openflow_message (connection, 
                               (sizeof(struct ofp_role_status) - sizeof (struct ofp_header)), 
                               OFPT_ROLE_STATUS, xid, buf);
   
   free (buf);
   return ret;
}

uint8_t 
send_table_status_message(struct ofp_conn *connection, struct ofp_table_desc *table_config,
                         enum ofp_port_reason reason) 
{
   char *buf = NULL;
   struct ofp_table_status *table_status_msg = NULL;
   uint8_t ret;
   uint32_t xid = htonl(0);
   uint32_t eviction_flag = 0;

   buf = malloc (sizeof(struct ofp_table_status));
   table_status_msg = (struct ofp_table_status *)buf;
   memset(table_status_msg,0,sizeof(struct ofp_table_status));

   table_status_msg->reason = reason;
   table_status_msg->table.length = table_config->length;
   table_status_msg->table.table_id = htons (table_config->table_id);
   table_status_msg->table.config = htonl (table_config->config);
   table_status_msg->table.properties[0].type = table_config->properties[0].type;
   table_status_msg->table.properties[0].length = table_config->properties[0].length;
   if (table_config->properties[0].type == OFPTMPT_EVICTION) {
      memcpy(&eviction_flag, (&table_config->properties[0]+4), sizeof(uint32_t));
      /* TODO Need to fill the eviction flag here */
   }

   /* TODO check the size */
   ret = send_openflow_message (connection, 
                               (sizeof(struct ofp_table_status) - sizeof (struct ofp_header)), 
                               OFPT_TABLE_STATUS, xid, buf);
   
   free (buf);
   return ret;
}

uint8_t 
send_request_forward_message(struct ofp_conn *connection, struct ofp_header *request)
{
   char *buf = NULL;
   struct ofp_requestforward_header *req_forward_msg = NULL;
   uint8_t ret;
   uint32_t xid = htonl(0);

   buf = malloc (sizeof(struct ofp_requestforward_header));
   req_forward_msg = (struct ofp_requestforward_header *) buf;
   memset(req_forward_msg,0,sizeof(struct ofp_requestforward_header));
   req_forward_msg->request.type = request->type;
   req_forward_msg->request.version = request->version;

   req_forward_msg->request.length = htons(request->length); 
   req_forward_msg->request.xid = htonl(request->xid); 

   /* TODO check the size */
   ret = send_openflow_message (connection, 
                               (sizeof(struct ofp_requestforward_header) - sizeof (struct ofp_header)), 
                               OFPT_REQUESTFORWARD, xid, buf);
   
   free (buf);
   return ret;
}

uint8_t
process_packet_out_message (struct ofp_conn *connection, struct ofp_packet_out *packet)
{
   struct ofp_role_request *role_reply;
   uint8_t ret;

   if (packet->buffer_id == OFP_NO_BUFFER) {
     /* call the function which sends the packet via the switch port */
   }
   else {
     /* retrieve the packet which are buffered */
   } 
   return 0;   
}

uint8_t
process_table_modify_message (struct ofp_conn *connection, char *buf)
{
   struct ofp_table_mod *table_modify_msg = (struct ofp_table_mod *) (buf);
   int i;
   uint8_t table_id = ntohs(table_modify_msg->table_id);
   uint32_t config_flag = ntohl (table_modify_msg->config);

   if (table_id == OFPTT_ALL) {
      for (i=0;i<ofp_switch.features.n_tables;i++) {  
         if (config_flag == OFPTC_EVICTION) {
            struct ofp_table_mod_prop_eviction *eviction_prop = (struct ofp_table_mod_prop_eviction *)&(table_modify_msg->properties[0]);
            ofp_switch.features.tables[i].eviction_flag = ntohl (eviction_prop->flags);
         }
         else if (config_flag == OFPTC_VACANCY_EVENTS){
            struct ofp_table_mod_prop_vacancy *vacancy_prop = (struct ofp_table_mod_prop_vacancy *)&(table_modify_msg->properties[0]);
            ofp_switch.features.tables[i].vacancy_down = ntohs (vacancy_prop->vacancy_down);
            ofp_switch.features.tables[i].vacancy_up = ntohs (vacancy_prop->vacancy_up);
            ofp_switch.features.tables[i].vacancy = ntohs (vacancy_prop->vacancy);
         }
      }
   }
   else {
      if (config_flag == OFPTC_EVICTION) {
         struct ofp_table_mod_prop_eviction *eviction_prop = (struct ofp_table_mod_prop_eviction *)&(table_modify_msg->properties[0]);
         ofp_switch.features.tables[table_id].eviction_flag = ntohl (eviction_prop->flags);
       }
       else if (config_flag == OFPTC_VACANCY_EVENTS){
          struct ofp_table_mod_prop_vacancy *vacancy_prop = (struct ofp_table_mod_prop_vacancy *)&(table_modify_msg->properties[0]);
          ofp_switch.features.tables[i].vacancy_down = ntohs (vacancy_prop->vacancy_down);
          ofp_switch.features.tables[i].vacancy_up = ntohs (vacancy_prop->vacancy_up);
          ofp_switch.features.tables[i].vacancy = ntohs (vacancy_prop->vacancy);
       }
   }
   return 0;
}

uint8_t
process_flow_modify_message (struct ofp_conn *connection, char *buf)
{
   struct ofp_flow_mod *flow_modify_msg = (struct ofp_flow_mod *) (buf);
   uint16_t flag;
   struct openflow_entry *flow_entry;
   flow_entry = malloc (sizeof (struct openflow_entry));
   flow_entry->cookie = ntohl_64 (flow_modify_msg->cookie); /* TODO define this ntohl*/
   flow_entry->table_id = ntohs(flow_modify_msg->table_id);
   flow_entry->idle_timeout = flow_modify_msg->idle_timeout;
   flow_entry->hard_timeout = flow_modify_msg->hard_timeout;
   flow_entry->priority = flow_modify_msg->priority;
   flag  = flow_modify_msg->flags;
   flow_entry->importance = flow_modify_msg->importance;
   memcpy(&flow_entry->match,&flow_modify_msg->match, sizeof(struct ofp_match)); 
   if (ntohs (flow_modify_msg->command) == OFPFC_ADD) {
      /* Based on the flow entry implementation install it in the h/w */
   }
   else if (ntohs (flow_modify_msg->command) == OFPFC_DELETE) {
      /* Based on the flow entry implementation delete it from the h/w */   
   }
   else {
      /* Based on the flow entry implementation modify the existing entry in h/w */   
   }
   return 0;
}

uint8_t
process_group_modify_add_message (struct ofp_conn *connection, uint32_t xid, struct ofp_group_mod *group_modify_msg)
{
   uint16_t flag = 0;
   uint16_t i,j;
   uint16_t no_of_bucket = 0;
   uint16_t no_of_actions = 0;
   struct openflow_group_entry *group_entry = NULL;
   struct openflow_group_entry *tmp_group_entry = NULL;
   struct openflow_group_entry *position_to_insert = NULL;
   struct ofp_switch_bucket *ofs_bucket = NULL;
   struct ofp_switch_bucket *prev_bucket = NULL;
   struct ofp_bucket *msg_bucket = NULL;
   struct ofp_action_header *msg_action = NULL;
   struct ofp_switch_acts *prev_action = NULL;
   struct ofp_switch_acts *ofpacts = NULL;
   uint32_t group_id = nthol(group_modify_msg->group_id);

   if (IS_GROUP_ALREADY_EXISTS(group_id)) {
     send_error_message(connection, xid, OFPET_GROUP_MOD_FAILED, OFPGMFC_GROUP_EXISTS);
     return  0;
   }

   if (ofp_switch.group_table->total_group_count > OFPG_MAX) {
     send_error_message(connection, xid, OFPET_GROUP_MOD_FAILED, OFPGMFC_OUT_OF_GROUPS);
     return  0;
   }

   group_entry = malloc (sizeof (struct openflow_group_entry));

   group_entry->type = ntohs (group_modify_msg->type); 
   group_entry->group_id = ntohl (group_modify_msg->group_id); 
   //group_count = ofp_switch.group_table->total_group_count;
   tmp_group_entry = ofp_switch.group_table->group_entry;
   while (tmp_group_entry) {
     position_to_insert = tmp_group_entry; 
     tmp_group_entry = tmp_group_entry->list_node.next;
   }
   position_to_insert->list_node.next = group_entry;
   group_entry->list_node.prev = position_to_insert;

   for (i=0;i<no_of_bucket;i++) {
     ofs_bucket = malloc (sizeof (struct ofp_switch_bucket));
     msg_bucket = &group_modify_msg->buckets[i];
     prev_bucket = msg_bucket;
     ofs_bucket->weight = msg_bucket->weight;
     ofs_bucket->watch_port = ntohl(msg_bucket->watch_port);
     ofs_bucket->watch_group = ntohl(msg_bucket->watch_group);
     msg_action = msg_bucket->actions;
     for (j=0;j<no_of_actions;j++) {
        ofpacts = malloc (sizeof (struct ofp_switch_acts));
        prev_action = ofpacts;
        ofpacts->header.len = msg_action->len;
        ofpacts->header.type = msg_action->type;
        if (msg_action->type == OFPAT_OUTPUT) {
          struct ofp_action_output *msg_output = (struct ofp_action_output *) 
                  (msg_action + sizeof(struct ofp_action_header));
          ofpacts->action.output.port = nthol(msg_output->port);
          ofpacts->action.output.max_len= nthol(msg_output->max_len);
        }
        else if (msg_action->type == OFPAT_GROUP) {
          struct ofp_action_group *msg_group = (struct ofp_action_group *) 
                  (msg_action + sizeof(struct ofp_action_header));
          ofpacts->action.group.group_id = nthol(msg_group->group_id);
        }
        else if (msg_action->type == OFPAT_SET_QUEUE) {
          struct ofp_action_set_queue *msg_set_queue = (struct ofp_action_set_queue *) 
                  (msg_action + sizeof(struct ofp_action_header));
          ofpacts->action.queue.queue_id = nthol(msg_set_queue->queue_id);
        }
        else if (msg_action->type == OFPAT_SET_MPLS_TTL) {
          struct ofp_action_mpls_ttl *msg_mpls_ttl = (struct ofp_action_mpls_ttl *) 
                  (msg_action + sizeof(struct ofp_action_header));
          ofpacts->action.mpls_ttl.mpls_ttl = nthos(msg_mpls_ttl->mpls_ttl);
        }
        else if (msg_action->type == OFPAT_SET_NW_TTL) {
          struct ofp_action_nw_ttl *msg_nw_ttl = (struct ofp_action_nw_ttl *) 
                  (msg_action + sizeof(struct ofp_action_header));
          ofpacts->action.nw_ttl.nw_ttl = nthos(msg_nw_ttl->nw_ttl);
        }
        else if ((msg_action->type == OFPAT_PUSH_VLAN) ||
                 (msg_action->type == OFPAT_PUSH_MPLS) ||
                 (msg_action->type == OFPAT_PUSH_PBB)) {
          struct ofp_action_push *msg_push = (struct ofp_action_push *) 
                  (msg_action + sizeof(struct ofp_action_header));
          ofpacts->action.push.ethertype = msg_push->ethertype;
        }
        else if (msg_action->type == OFPAT_POP_MPLS) {
          struct ofp_action_pop_mpls *msg_pop_mpls = (struct ofp_action_pop_mpls *) 
                  (msg_action + sizeof(struct ofp_action_header));
          ofpacts->action.pop_mpls.ethertype = msg_pop_mpls->ethertype;
        }
        else if (msg_action->type == OFPAT_SET_FIELD) {
          struct ofp_action_set_field *msg_set_field = (struct ofp_action_set_field *) 
                  (msg_action + sizeof(struct ofp_action_header));
          memcpy(&ofpacts->action.set_field.field, &msg_set_field->field, sizeof(uint32_t));
        }
        else if ((msg_action->type == OFPAT_COPY_TTL_OUT) ||
                 (msg_action->type == OFPAT_COPY_TTL_IN) ||
                 (msg_action->type == OFPAT_DEC_MPLS_TTL) ||
                 (msg_action->type == OFPAT_DEC_NW_TTL) ||
                 (msg_action->type == OFPAT_POP_VLAN) ||
                 (msg_action->type == OFPAT_POP_PBB)) {
          /* ignore this as these actions doesn't have any datas. */
        }
        if (j==0)
           ofs_bucket->ofpacts_list = ofpacts;
        else {
           ofpacts->list_node.prev = prev_action;
           prev_action->list_node.next = ofpacts;
        }
      }
     if (i==0)
        group_entry->bucket_list = ofs_bucket;
     else {
        ofs_bucket->list_node.prev = prev_bucket;
        prev_bucket->list_node.next = ofs_bucket;
     }
   }
   return 0;
}

uint8_t
ofp_delete_group (struct openflow_group_entry *group_entry)
{
   uint16_t i,j;
   uint16_t no_of_bucket = 0;
   uint16_t no_of_actions = 0;
   struct ofp_switch_bucket *bucket = group_entry->bucket_list;
   struct ofp_switch_bucket *next_bucket = NULL;
   for (i=0;i<no_of_bucket;i++) {
      struct ofp_switch_acts *ofp_acts = bucket->ofpacts_list;
      struct ofp_switch_acts *next_act = NULL;
      for (j=0;j<no_of_actions;j++) {
         next_act = ofp_acts->list_node.next;
         free (next_act);
         ofp_acts = next_act;
      }
      next_bucket = bucket->list_node.next;
      free (bucket);
      bucket = next_bucket;
   }
   return 0;
}
uint8_t
process_group_add_modify_del_message (struct ofp_conn *connection, uint32_t xid, struct ofp_group_mod *group_modify_msg)
{
   struct openflow_group_entry *group_entry = NULL;
   struct openflow_group_entry *prev_group_entry = NULL;
   struct openflow_group_entry *next_group_entry = NULL;
   bool delete_all = FALSE;
   uint32_t group_id;

   group_id = ntohl (group_modify_msg->group_id); 

   if (!IS_GROUP_ALREADY_EXISTS(group_id)) {
     return  0;
   }
   else if (group_id == OFPG_ALL) {
     delete_all = TRUE;
   }
   group_entry = ofp_switch.group_table->group_entry;

   while (group_entry) {
     next_group_entry = group_entry->list_node.next;
     if ((group_entry->group_id == group_id) || (delete_all)) {
        if (next_group_entry)
           next_group_entry->list_node.prev = group_entry->list_node.prev;
        if (prev_group_entry) 
          prev_group_entry->list_node.next = group_entry->list_node.next;
        ofp_delete_group (group_entry);
        free (group_entry);
     }
     prev_group_entry = group_entry;
     group_entry = group_entry->list_node.next;
   }
   return 0;
}

uint8_t
process_group_add_modify_mod_message (struct ofp_conn *connection, uint32_t xid, struct ofp_group_mod *group_modify_msg)
{
   uint16_t i,j;
   uint16_t no_of_bucket = 0;
   uint16_t no_of_actions = 0;
   struct openflow_group_entry *group_entry = NULL;
   bool delete_all = FALSE;
   uint32_t group_id;

   group_id = ntohl (group_modify_msg->group_id); 

   if (!IS_GROUP_ALREADY_EXISTS(group_id)) {
     send_error_message(connection, xid, OFPET_GROUP_MOD_FAILED, OFPGMFC_UNKNOWN_GROUP);
     return  0;
   }

   group_entry = ofp_switch.group_table->group_entry;
   while (group_entry) {
     if (group_entry->group_id == group_id) {
        ofp_delete_group (group_entry);
        process_group_modify_add_message (connection, xid, group_modify_msg);
     }
     else {
        group_entry = group_entry->list_node.next;
     }
   }
   return 0;
}

uint8_t
process_group_modify_message (struct ofp_conn *connection, char *buf)
{
   struct ofp_group_mod *group_modify_msg = (struct ofp_group_mod *) (buf);
   uint8_t command = ntohs(group_modify_msg->command);
   uint32_t xid = htonl(0);

   if (command == OFPGC_ADD)
     return process_group_modify_add_message (connection, xid, group_modify_msg);
   else if (command == OFPGC_MODIFY)
     return process_group_add_modify_mod_message (connection, xid, group_modify_msg);
   else if (command == OFPGC_DELETE)
     return process_group_add_modify_del_message (connection, xid, group_modify_msg);
   else
     send_error_message(connection, xid, OFPET_GROUP_MOD_FAILED, OFPGMFC_INVALID_GROUP);

   return 0;
}
   
uint8_t
process_meter_modify_add_message (struct ofp_conn *connection, uint32_t xid, struct ofp_meter_mod *meter_modify_msg)
{
   uint16_t flag = 0;
   uint16_t i,j;
   uint16_t no_of_bands = 0;
   struct openflow_meter_entry *meter_entry = NULL;
   struct openflow_meter_entry *tmp_meter_entry = NULL;
   struct openflow_meter_entry *position_to_insert = NULL;
   struct ofp_switch_meter_band *bands = NULL;
   struct ofp_switch_meter_band *prev_band = NULL;
   struct ofp_meter_band_header *msg_band = NULL;
   uint32_t meter_id = ntohl(meter_modify_msg->meter_id);

   if (IS_METER_ALREADY_EXISTS(meter_id)) {
     send_error_message(connection, xid, OFPET_METER_MOD_FAILED, OFPMMFC_METER_EXISTS);
     return  0;
   }

   if (ofp_switch.meter_table->total_meter_count > OFPM_MAX) {
     send_error_message(connection, xid, OFPET_METER_MOD_FAILED, OFPMMFC_OUT_OF_METERS);
     return  0;
   }

   meter_entry = malloc (sizeof (struct openflow_meter_entry));
   meter_entry->meter_id = meter_id;
   tmp_meter_entry = ofp_switch.meter_table->meter_entry;
   while (tmp_meter_entry) {
     position_to_insert = tmp_meter_entry; 
     tmp_meter_entry = tmp_meter_entry->list_node.next;
   }
   position_to_insert->list_node.next = meter_entry;
   meter_entry->list_node.prev = position_to_insert;

   for (i=0;i<no_of_bands;i++) {
     bands = malloc (sizeof (struct ofp_switch_meter_band));
     msg_band = &meter_modify_msg->bands[i];
     bands->type = msg_band->type;
     bands->len = msg_band->len;
     bands->rate = ntohl(msg_band->rate);
     bands->burst_size = ntohl(msg_band->burst_size);
     if (bands->type == OFPMBT_DSCP_REMARK) {
        struct ofp_meter_band_dscp_remark *msg_band_dsc = (struct ofp_meter_band_dscp_remark *) 
               (msg_band + sizeof(struct ofp_meter_band_header));
        bands->band_specific_date.prec_level = nthos(msg_band_dsc->prec_level);
     }
     if (i==0)
        meter_entry->band_list = bands;
     else {
        bands->list_node.prev = prev_band;
        prev_band->list_node.next = bands;
     }
     prev_band = bands;
   }
   return 0;
}

uint8_t
ofp_delete_meter (struct openflow_meter_entry *meter_entry)
{
   uint16_t i,j;
   uint16_t no_of_bands = 0;
   uint16_t no_of_actions = 0;
   struct ofp_switch_meter_band *band = meter_entry->band_list;
   struct ofp_switch_meter_band *next_band = NULL;
   for (i=0;i<no_of_bands;i++) {
      next_band = band->list_node.next;
      free (band);
      band = next_band;
   }
   return 0;
}

uint8_t
process_meter_add_modify_del_message (struct ofp_conn *connection, uint32_t xid, struct ofp_meter_mod *meter_modify_msg)
{
   struct openflow_meter_entry *meter_entry = NULL;
   struct openflow_meter_entry *prev_meter_entry = NULL;
   struct openflow_meter_entry *next_meter_entry = NULL;
   bool delete_all = FALSE;
   uint32_t meter_id;

   meter_id = ntohl (meter_modify_msg->meter_id); 

   if (!IS_GROUP_ALREADY_EXISTS(meter_id)) {
     return  0;
   }
   else if (meter_id == OFPG_ALL) {
     delete_all = TRUE;
   }
   meter_entry = ofp_switch.meter_table->meter_entry;
   while (meter_entry) {
     next_meter_entry = meter_entry->list_node.next;
     if ((meter_entry->meter_id == meter_id) || (delete_all)) {
        if (next_meter_entry)
           next_meter_entry->list_node.prev = meter_entry->list_node.prev;
        if (prev_meter_entry) 
          prev_meter_entry->list_node.next = meter_entry->list_node.next;
        ofp_delete_meter (meter_entry);
        free (meter_entry);
     }
     prev_meter_entry = meter_entry;
     meter_entry = meter_entry->list_node.next;
   }
   return 0;
}

uint8_t
process_meter_mod_modify_mod_message (struct ofp_conn *connection, uint32_t xid, struct ofp_meter_mod *meter_modify_msg)
{
   uint16_t i,j;
   uint16_t no_of_bucket = 0;
   uint16_t no_of_actions = 0;
   struct openflow_meter_entry *meter_entry = NULL;
   bool delete_all = FALSE;
   uint32_t meter_id;

   meter_id = ntohl (meter_modify_msg->meter_id); 

   if (!IS_METER_ALREADY_EXISTS(meter_id)) {
     send_error_message(connection, xid, OFPET_METER_MOD_FAILED, OFPMMFC_UNKNOWN_METER);
     return  0;
   }

   meter_entry = ofp_switch.meter_table->meter_entry;
   while (meter_entry) {
     if (meter_entry->meter_id == meter_id) {
        ofp_delete_meter (meter_entry);
        process_group_modify_add_message (connection, xid, meter_modify_msg);
     }
     else {
        meter_entry = meter_entry->list_node.next;
     }
   }
   return 0;
}

uint8_t
process_meter_modify_message (struct ofp_conn *connection, char *buf)
{
   struct ofp_meter_mod *meter_modify_msg = (struct ofp_meter_mod *) (buf);
   uint8_t command = ntohs(meter_modify_msg->command);
   uint32_t xid = htonl(0);

   if (command == OFPMC_ADD)
     return process_meter_modify_add_message (connection, xid, meter_modify_msg);
   else if (command == OFPMC_MODIFY)
     return process_meter_add_modify_mod_message (connection, xid, meter_modify_msg);
   else if (command == OFPMC_DELETE)
     return process_meter_add_modify_del_message (connection, xid, meter_modify_msg);
   else
     send_error_message(connection, xid, OFPET_METER_MOD_FAILED, OFPMMFC_INVALID_METER);

   return 0;
}


