#include <tbox/main/main.h>

#include "socks5/create.h"

namespace tbox {
namespace main {

void RegisterApps(Module &app, Context &ctx) {
  app.add(hevake::socks5::Create(ctx));
}

std::string GetAppDescribe() {
  return "socks5 server base on cpp-tbox.";
}

std::string GetAppBuildTime() {
  return __DATE__ " " __TIME__;
}

void GetAppVersion(int &major, int &minor, int &rev, int &build) {
  major = 0;
  minor = 1;
  rev = 3;
  build = 10;
}


}
}
