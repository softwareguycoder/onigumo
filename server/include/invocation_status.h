#ifndef __INVOCATION_STATUS_H__
#define __INVOCATION_STATUS_H__

typedef enum {
  SESSIONCLOSED, /* command is finished being invoked */
  SESSIONOPENED, /* command invocation session has begun */
  SESSIONERROR,  /* command utterly failed to carry out its objective */
  WARNING, /* user supplied incorrect input, or other non-showstopper  */
  WAITING, /* command engine waiting for new input from user/client */
  MULTILINE_START,   /* starting a multiline reply */
  MULITLINE_END,     /* ending a multiline reply */
}  INVOCATIONSTATUS, *LPINVOCATIONSTATUS;

#endif //__INVOCATION_STATUS_H__