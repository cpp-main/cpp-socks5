#include <tbox/main/main.h>
#include "module.h"

//! 作为插件的入口，使得可以被 run 执行
extern "C" void RegisterApps(tbox::main::Module &apps, tbox::main::Context &ctx) {
  apps.add(new hevake::socks5::Module(ctx));
}
