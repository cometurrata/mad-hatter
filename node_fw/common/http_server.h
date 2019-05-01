#ifndef __HTTP_SERVER__H__
#define __HTTP_SERVER__H__

#include <stdint.h>
#include <user_config.h>
#include <SmingCore/SmingCore.h>

void startWebServer(void);
void httpServerAddPath(String path, const HttpPathDelegate &callback);

#endif /* __HTTP_SERVER__H__ */