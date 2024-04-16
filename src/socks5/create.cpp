#include "create.h"

#include "module.h"

namespace hevake {
namespace socks5 {

tbox::main::Module* Create(tbox::main::Context &ctx) {
  return new Module(ctx);
}

}
}

