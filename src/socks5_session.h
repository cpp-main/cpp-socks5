#ifndef HEVAKE_SOCKS5_SESSION_H
#define HEVAKE_SOCKS5_SESSION_H

#include <tbox/base/cabinet_token.h>
#include <tbox/main/module.h>
#include <tbox/network/tcp_connection.h>

namespace hevake {
namespace socks5 {

class Session {
  public:
    using Token = tbox::cabinet::Token;

  public:
    class Parent {
      public:
        virtual void onSessionClosed(Token) = 0;
    };

  public:
    explicit Session(tbox::main::Context &ctx, Parent &parent,
                     Token token, tbox::network::TcpConnection *src_conn);
    virtual ~Session();

  protected:
    void onSrcTcpDisconnected();
    void onSrcTcpReceived(tbox::network::Buffer &buff);

  private:
    tbox::main::Context &ctx_;
    Parent &parent_;
    Token token_;
    tbox::network::TcpConnection *src_conn_;

    tbox::network::TcpConnection *dst_conn_ = nullptr;
};

}
}

#endif //HEVAKE_SOCKS5_SESSION_H
