#ifndef HEVAKE_SOCKS5_MODULE_H
#define HEVAKE_SOCKS5_MODULE_H

#include <tbox/main/module.h>
#include <tbox/network/tcp_acceptor.h>
#include <tbox/base/cabinet.hpp>

#include "socks5_session.h"
#include "socks5_proto.h"

namespace hevake {
namespace socks5 {

class Module : public tbox::main::Module,
               public Session::Parent {
  public:
    explicit Module(tbox::main::Context &ctx);
    virtual ~Module();

  protected:
    virtual void onFillDefaultConfig(tbox::Json &js) override;
    virtual bool onInit(const tbox::Json &js) override;
    virtual bool onStart() override;
    virtual void onStop() override;
    virtual void onCleanup() override;

  protected:
    void onNewConnection(tbox::network::TcpConnection *new_conn);

    virtual void onSessionClosed(Session::Token token) override;
    virtual tbox::network::DnsRequest& dns_request() override { return dns_request_; }

    virtual const std::set<PROTO_METHOD>& getMethods() const override { return method_set_; }
    virtual std::string getUsername() const override { return username_; }
    virtual std::string getPassword() const override { return password_; }

  private:
    tbox::network::TcpAcceptor *tcp_acceptor_;
    tbox::cabinet::Cabinet<Session> session_cabinet_;
    tbox::network::DnsRequest dns_request_;

    std::set<PROTO_METHOD> method_set_;
    std::string username_, password_;
};

}
}

#endif //HEVAKE_SOCKS5_MODULE_H
