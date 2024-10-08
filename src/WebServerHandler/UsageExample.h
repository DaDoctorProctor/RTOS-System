#ifndef USAGEEXAMPLE_H
#define USAGEEXAMPLE_H

#include "WebServerHandler.h"

class UsageExample {
public:
  UsageExample(WebServerHandler& handler);
  String printCommand();

private:
  WebServerHandler& webServerHandler;
};

#endif // USAGEEXAMPLE_H
