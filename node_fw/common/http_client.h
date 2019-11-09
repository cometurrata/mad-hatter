#ifndef __HTTP_CLIENT__H__
#define __HTTP_CLIENT__H__

#include <SmingCore.h>

void sendHttpRequest(String path, String data, RequestCompletedDelegate onHttpRequestResponse);
void sendNodeUpdate(String data);
void sendHeartBeat(void);

#endif /* __HTTP_CLIENT__H__ */