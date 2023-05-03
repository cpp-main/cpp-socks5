#ifndef HEVAKE_SOCKS5_SESSION_H
#define HEVAKE_SOCKS5_SESSION_H

#include <tbox/base/cabinet_token.h>
#include <tbox/main/module.h>
#include <tbox/event/timer_event.h>
#include <tbox/network/tcp_connector.h>
#include <tbox/network/tcp_connection.h>
#include <tbox/network/dns_request.h>
#include <tbox/util/serializer.h>

#include "socks5_proto.h"

namespace hevake {
namespace socks5 {

class Session {
  public:
    using Token = tbox::cabinet::Token;

  public:
    class Parent {
      public:
        virtual tbox::network::DnsRequest& dns_request() = 0;
        virtual void onSessionClosed(Token) = 0;
        virtual PROTO_METHOD getMethod() const = 0;
    };

  public:
    explicit Session(tbox::main::Context &ctx, Parent &parent,
                     Token token, tbox::network::TcpConnection *src_conn);
    virtual ~Session();

    void start();
    void stop();

  protected:
    enum class State {
      kWaitMethod,  //!< 等待Method
      kWaitConnect, //!< 回复了Method
      kWaitDnsParseResult,
      kWaitConnectResult,
      kEstablished, //!< 已建立
      kTerm,        //!< 终止
    };

    void sendToSrc(const void *data_ptr, size_t data_size);
    void disconnectWithSrc();
    void endSession();

    void onSrcTcpDisconnected();
    void onSrcTcpReceived(tbox::network::Buffer &buff);

    size_t handleAsMethodReq(tbox::util::Deserializer &parser);
    void sendMethodResToSrc(PROTO_METHOD method);

    size_t handleAsCmdReq(tbox::util::Deserializer &parser);

    size_t handleAsConnectReq(tbox::util::Deserializer &parser);
    size_t handleAsBindReq(tbox::util::Deserializer &parser);
    size_t handleAsUdpAssociateReq(tbox::util::Deserializer &parser);

    void sendCmdResToSrc(PROTO_REP rep);

    void startParseDns();
    void startConnect();

    void onParseDnsFinished(const tbox::network::DnsRequest::Result &result);

    void onDstTcpConnected(tbox::network::TcpConnection *dst_conn);
    void onDstTcpConnectFail();
    void onDstTcpDisconnected();

    void onTimeout();

  private:
    tbox::main::Context &ctx_;
    Parent &parent_;
    Token token_;
    tbox::network::TcpConnection *src_conn_;
    tbox::event::TimerEvent *timeout_timer_;

    State state_ = State::kWaitMethod;

    PROTO_ATYP atype_;
    tbox::network::IPAddress dst_ipv4_;
    tbox::network::DomainName dst_domainname_;
    uint8_t dst_ipv6_[16];  //! UNSUPPORT
    uint16_t dst_port_ = 0;

    tbox::network::TcpConnector *dst_ctor_ = nullptr;
    tbox::network::TcpConnection *dst_conn_ = nullptr;

    tbox::network::DnsRequest::ReqId dns_req_id_ = 0;
};

}
}

#endif //HEVAKE_SOCKS5_SESSION_H
