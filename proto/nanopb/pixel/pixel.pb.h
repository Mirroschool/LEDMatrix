/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.3 */

#ifndef PB_PIXEL_PB_H_INCLUDED
#define PB_PIXEL_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _PixelMessage {
    int32_t x;
    int32_t y;
    int32_t red;
    int32_t green;
    int32_t blue;
} PixelMessage;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define PixelMessage_init_default                {0, 0, 0, 0, 0}
#define PixelMessage_init_zero                   {0, 0, 0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define PixelMessage_x_tag                       1
#define PixelMessage_y_tag                       2
#define PixelMessage_red_tag                     3
#define PixelMessage_green_tag                   4
#define PixelMessage_blue_tag                    5

/* Struct field encoding specification for nanopb */
#define PixelMessage_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    x,                 1) \
X(a, STATIC,   SINGULAR, INT32,    y,                 2) \
X(a, STATIC,   SINGULAR, INT32,    red,               3) \
X(a, STATIC,   SINGULAR, INT32,    green,             4) \
X(a, STATIC,   SINGULAR, INT32,    blue,              5)
#define PixelMessage_CALLBACK NULL
#define PixelMessage_DEFAULT NULL

extern const pb_msgdesc_t PixelMessage_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define PixelMessage_fields &PixelMessage_msg

/* Maximum encoded size of messages (where known) */
#define PixelMessage_size                        55

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
