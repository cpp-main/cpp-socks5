#include <tbox/main/main.h>

#include "socks5/module.h"

namespace tbox {
namespace main {

void RegisterApps(Module &app, Context &ctx) {
  app.add(new hevake::socks5::Module(ctx));
}

std::string GetAppDescribe() {
  return "socks5 server base on cpp-tbox.";
}

std::string GetAppBuildTime() {
  return __DATE__ " " __TIME__;
}

void GetAppVersion(int &major, int &minor, int &rev, int &build) {
  major = 0;
  minor = 0;
  rev = 2;
  build = 9;
}


}
}
