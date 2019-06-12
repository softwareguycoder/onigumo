#ifndef __INVOCATION_STATUS_H__
#define __INVOCATION_STATUS_H__

typedef enum {
  CLOSED, /* command is finished being invoked */
  OPENED, /* command invocation session has begun */
  ERROR,  /* command utterly failed to carry out its objective */
  WARNING, /* user supplied incorrect input, or other non-showstopper  */
  WAITING, /* command engine waiting for new input from user */
}  INVOCATIONSTATUS, *LPINVOCATIONSTATUS;

#endif //__INVOCATION_STATUS_H__
