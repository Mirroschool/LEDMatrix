/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.3 */

#ifndef PB_CLOCK_PB_H_INCLUDED
#define PB_CLOCK_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _ClockMessage {
    int32_t mode_id;
    int32_t timestamp;
    pb_byte_t text[255];
    int32_t bg_r;
    int32_t bg_g;
    int32_t bg_b;
    int32_t fg_r;
    int32_t fg_g;
    int32_t fg_b;
} ClockMessage;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define ClockMessage_init_default                {0, 0, {0}, 0, 0, 0, 0, 0, 0}
#define ClockMessage_init_zero                   {0, 0, {0}, 0, 0, 0, 0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define ClockMessage_mode_id_tag                 1
#define ClockMessage_timestamp_tag               2
#define ClockMessage_text_tag                    3
#define ClockMessage_bg_r_tag                    4
#define ClockMessage_bg_g_tag                    5
#define ClockMessage_bg_b_tag                    6
#define ClockMessage_fg_r_tag                    7
#define ClockMessage_fg_g_tag                    8
#define ClockMessage_fg_b_tag                    9

/* Struct field encoding specification for nanopb */
#define ClockMessage_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    mode_id,           1) \
X(a, STATIC,   SINGULAR, INT32,    timestamp,         2) \
X(a, STATIC,   SINGULAR, FIXED_LENGTH_BYTES, text,              3) \
X(a, STATIC,   SINGULAR, INT32,    bg_r,              4) \
X(a, STATIC,   SINGULAR, INT32,    bg_g,              5) \
X(a, STATIC,   SINGULAR, INT32,    bg_b,              6) \
X(a, STATIC,   SINGULAR, INT32,    fg_r,              7) \
X(a, STATIC,   SINGULAR, INT32,    fg_g,              8) \
X(a, STATIC,   SINGULAR, INT32,    fg_b,              9)
#define ClockMessage_CALLBACK NULL
#define ClockMessage_DEFAULT NULL

extern const pb_msgdesc_t ClockMessage_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define ClockMessage_fields &ClockMessage_msg

/* Maximum encoded size of messages (where known) */
#define ClockMessage_size                        346

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
