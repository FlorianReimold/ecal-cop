/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_ECAL_PB_TOPIC_PB_H_INCLUDED
#define PB_ECAL_PB_TOPIC_PB_H_INCLUDED
#include <pb.h>
#include "layer.pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _eCAL_pb_DataTypeInformation {
    pb_callback_t name; /* name of the datatype */
    pb_callback_t encoding; /* encoding of the datatype (e.g. protobuf, flatbuffers, capnproto) */
    pb_callback_t descriptor; /* descriptor information of the datatype (necessary for reflection) */
} eCAL_pb_DataTypeInformation;

typedef struct _eCAL_pb_Topic {
    int32_t rclock; /* registration clock (heart beat) */
    pb_callback_t hname; /* host name */
    int32_t pid; /* process id */
    pb_callback_t pname; /* process name */
    pb_callback_t uname; /* unit name */
    pb_callback_t tid; /* topic id */
    pb_callback_t tname; /* topic name */
    pb_callback_t direction; /* direction (publisher, subscriber) */
    pb_callback_t ttype; /* topic type + topic encoding (deprecated) */
    pb_callback_t tdesc; /* topic description (protocol descriptor) (deprecated) */
    pb_callback_t tlayer; /* active topic transport layers and it's specific parameter */
    int32_t tsize; /* topic size */
    int32_t connections_loc; /* number of local connected entities */
    int32_t connections_ext; /* number of external connected entities */
    int32_t message_drops; /* dropped messages */
    int64_t did; /* data send id (publisher setid) */
    int64_t dclock; /* data clock (send / receive action) */
    int32_t dfreq; /* data frequency (send / receive samples per second) [mHz] */
    pb_callback_t attr; /* generic topic description */
    pb_callback_t hgname; /* host group name */
    bool has_tdatatype;
    eCAL_pb_DataTypeInformation tdatatype; /* topic datatype information (encoding & type & description) */
} eCAL_pb_Topic;

typedef struct _eCAL_pb_Topic_AttrEntry {
    pb_callback_t key;
    pb_callback_t value;
} eCAL_pb_Topic_AttrEntry;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define eCAL_pb_DataTypeInformation_init_default {{{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}}
#define eCAL_pb_Topic_init_default               {0, {{NULL}, NULL}, 0, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, 0, 0, 0, 0, 0, 0, 0, {{NULL}, NULL}, {{NULL}, NULL}, false, eCAL_pb_DataTypeInformation_init_default}
#define eCAL_pb_Topic_AttrEntry_init_default     {{{NULL}, NULL}, {{NULL}, NULL}}
#define eCAL_pb_DataTypeInformation_init_zero    {{{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}}
#define eCAL_pb_Topic_init_zero                  {0, {{NULL}, NULL}, 0, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}, 0, 0, 0, 0, 0, 0, 0, {{NULL}, NULL}, {{NULL}, NULL}, false, eCAL_pb_DataTypeInformation_init_zero}
#define eCAL_pb_Topic_AttrEntry_init_zero        {{{NULL}, NULL}, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define eCAL_pb_DataTypeInformation_name_tag     1
#define eCAL_pb_DataTypeInformation_encoding_tag 2
#define eCAL_pb_DataTypeInformation_descriptor_tag 3
#define eCAL_pb_Topic_rclock_tag                 1
#define eCAL_pb_Topic_hname_tag                  2
#define eCAL_pb_Topic_pid_tag                    3
#define eCAL_pb_Topic_pname_tag                  4
#define eCAL_pb_Topic_uname_tag                  5
#define eCAL_pb_Topic_tid_tag                    6
#define eCAL_pb_Topic_tname_tag                  7
#define eCAL_pb_Topic_direction_tag              8
#define eCAL_pb_Topic_ttype_tag                  9
#define eCAL_pb_Topic_tdesc_tag                  10
#define eCAL_pb_Topic_tlayer_tag                 12
#define eCAL_pb_Topic_tsize_tag                  13
#define eCAL_pb_Topic_connections_loc_tag        16
#define eCAL_pb_Topic_connections_ext_tag        17
#define eCAL_pb_Topic_message_drops_tag          18
#define eCAL_pb_Topic_did_tag                    19
#define eCAL_pb_Topic_dclock_tag                 20
#define eCAL_pb_Topic_dfreq_tag                  21
#define eCAL_pb_Topic_attr_tag                   27
#define eCAL_pb_Topic_hgname_tag                 28
#define eCAL_pb_Topic_tdatatype_tag              30
#define eCAL_pb_Topic_AttrEntry_key_tag          1
#define eCAL_pb_Topic_AttrEntry_value_tag        2

/* Struct field encoding specification for nanopb */
#define eCAL_pb_DataTypeInformation_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   name,              1) \
X(a, CALLBACK, SINGULAR, STRING,   encoding,          2) \
X(a, CALLBACK, SINGULAR, BYTES,    descriptor,        3)
#define eCAL_pb_DataTypeInformation_CALLBACK pb_default_field_callback
#define eCAL_pb_DataTypeInformation_DEFAULT NULL

#define eCAL_pb_Topic_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    rclock,            1) \
X(a, CALLBACK, SINGULAR, STRING,   hname,             2) \
X(a, STATIC,   SINGULAR, INT32,    pid,               3) \
X(a, CALLBACK, SINGULAR, STRING,   pname,             4) \
X(a, CALLBACK, SINGULAR, STRING,   uname,             5) \
X(a, CALLBACK, SINGULAR, STRING,   tid,               6) \
X(a, CALLBACK, SINGULAR, STRING,   tname,             7) \
X(a, CALLBACK, SINGULAR, STRING,   direction,         8) \
X(a, CALLBACK, SINGULAR, STRING,   ttype,             9) \
X(a, CALLBACK, SINGULAR, BYTES,    tdesc,            10) \
X(a, CALLBACK, REPEATED, MESSAGE,  tlayer,           12) \
X(a, STATIC,   SINGULAR, INT32,    tsize,            13) \
X(a, STATIC,   SINGULAR, INT32,    connections_loc,  16) \
X(a, STATIC,   SINGULAR, INT32,    connections_ext,  17) \
X(a, STATIC,   SINGULAR, INT32,    message_drops,    18) \
X(a, STATIC,   SINGULAR, INT64,    did,              19) \
X(a, STATIC,   SINGULAR, INT64,    dclock,           20) \
X(a, STATIC,   SINGULAR, INT32,    dfreq,            21) \
X(a, CALLBACK, REPEATED, MESSAGE,  attr,             27) \
X(a, CALLBACK, SINGULAR, STRING,   hgname,           28) \
X(a, STATIC,   OPTIONAL, MESSAGE,  tdatatype,        30)
#define eCAL_pb_Topic_CALLBACK pb_default_field_callback
#define eCAL_pb_Topic_DEFAULT NULL
#define eCAL_pb_Topic_tlayer_MSGTYPE eCAL_pb_TLayer
#define eCAL_pb_Topic_attr_MSGTYPE eCAL_pb_Topic_AttrEntry
#define eCAL_pb_Topic_tdatatype_MSGTYPE eCAL_pb_DataTypeInformation

#define eCAL_pb_Topic_AttrEntry_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   key,               1) \
X(a, CALLBACK, SINGULAR, STRING,   value,             2)
#define eCAL_pb_Topic_AttrEntry_CALLBACK pb_default_field_callback
#define eCAL_pb_Topic_AttrEntry_DEFAULT NULL

extern const pb_msgdesc_t eCAL_pb_DataTypeInformation_msg;
extern const pb_msgdesc_t eCAL_pb_Topic_msg;
extern const pb_msgdesc_t eCAL_pb_Topic_AttrEntry_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define eCAL_pb_DataTypeInformation_fields &eCAL_pb_DataTypeInformation_msg
#define eCAL_pb_Topic_fields &eCAL_pb_Topic_msg
#define eCAL_pb_Topic_AttrEntry_fields &eCAL_pb_Topic_AttrEntry_msg

/* Maximum encoded size of messages (where known) */
/* eCAL_pb_DataTypeInformation_size depends on runtime parameters */
/* eCAL_pb_Topic_size depends on runtime parameters */
/* eCAL_pb_Topic_AttrEntry_size depends on runtime parameters */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
