#include "socks5_module.h"

#include <tbox/base/log.h>
#include <tbox/base/defines.h>
#include <tbox/base/json.hpp>
#include <tbox/util/json.h>
#include <tbox/network/tcp_connection.h>

namespace hevake {

Socks5Module::Socks5Module(tbox::main::Context &ctx)
  : tbox::main::Module("socks5", ctx)
  , tcp_acceptor_(new tbox::network::TcpAcceptor(ctx.loop())) {
}

Socks5Module::~Socks5Module() {
  CHECK_DELETE_RESET_OBJ(tcp_acceptor_);
}

void Socks5Module::onFillDefaultConfig(tbox::Json &js) {
  js["bind"] = "0.0.0.0:1080";
  js["backlog"] = 20;
}

bool Socks5Module::onInit(const tbox::Json &js) {
  std::string bind;
  int backlog = 1;

  tbox::util::json::GetField(js, "bind", bind);
  tbox::util::json::GetField(js, "backlog", backlog);

  if (!tcp_acceptor_->initialize(tbox::network::SockAddr::FromString(bind), backlog)) {
    LogErr("tcp_acceptor initialize fail");
    return false;
  }

  using namespace std::placeholders;
  tcp_acceptor_->setNewConnectionCallback(std::bind(&Socks5Module::onNewConnection, this, _1));

  return true;
}

bool Socks5Module::onStart() {
  tcp_acceptor_->start();
  return true;
}

void Socks5Module::onStop() {
  tcp_acceptor_->stop();
}

void Socks5Module::onCleanup() {
  tcp_acceptor_->cleanup();
}

void Socks5Module::onNewConnection(tbox::network::TcpConnection *new_conn) {
  LogInfo("got new conn from: %s", new_conn->peerAddr().toString().c_str());
  ctx().loop()->runNext([new_conn] {delete new_conn;});
}

}
