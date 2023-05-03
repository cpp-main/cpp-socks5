#include "socks5_module.h"

#include <tbox/base/log.h>
#include <tbox/base/defines.h>
#include <tbox/base/json.hpp>
#include <tbox/util/json.h>
#include <tbox/network/tcp_connection.h>

namespace hevake {
namespace socks5 {

Module::Module(tbox::main::Context &ctx)
  : tbox::main::Module("socks5", ctx)
  , tcp_acceptor_(new tbox::network::TcpAcceptor(ctx.loop()))
  , dns_request_(ctx.loop()) {
}

Module::~Module() {
  CHECK_DELETE_RESET_OBJ(tcp_acceptor_);
}

void Module::onFillDefaultConfig(tbox::Json &js) {
  js["bind"] = "0.0.0.0:1080";
  js["backlog"] = 20;
}

bool Module::onInit(const tbox::Json &js) {
  std::string bind;
  int backlog = 1;

  tbox::util::json::GetField(js, "bind", bind);
  tbox::util::json::GetField(js, "backlog", backlog);

  if (!tcp_acceptor_->initialize(tbox::network::SockAddr::FromString(bind), backlog)) {
    LogErr("tcp_acceptor initialize fail");
    return false;
  }

  using namespace std::placeholders;
  tcp_acceptor_->setNewConnectionCallback(std::bind(&Module::onNewConnection, this, _1));

  tbox::network::DnsRequest::IPAddressVec dns_vec = {
    tbox::network::IPAddress::FromString("114.114.114.114"),
    tbox::network::IPAddress::FromString("8.8.8.8")
  };
  dns_request_.setDnsIPAddresses(dns_vec);

  return true;
}

bool Module::onStart() {
  tcp_acceptor_->start();
  return true;
}

void Module::onStop() {
  tcp_acceptor_->stop();
}

void Module::onCleanup() {
  tcp_acceptor_->cleanup();
}

void Module::onNewConnection(tbox::network::TcpConnection *new_conn) {
  LogInfo("new session from: %s", new_conn->peerAddr().toString().c_str());
  auto new_token = session_cabinet_.alloc();
  auto new_session = new Session(ctx(), *this, new_token, new_conn);
  session_cabinet_.update(new_token, new_session);
}

void Module::onSessionClosed(Session::Token token) {
  LogInfo("session closed");
  auto session = session_cabinet_.free(token);
  ctx().loop()->runNext([session] { CHECK_DELETE_OBJ(session); });
}

PROTO_METHOD Module::getMethod() const {
  return PROTO_METHOD_NO_AUTH;
}

tbox::network::DnsRequest& Module::dns_request() { return dns_request_; }

}
}
