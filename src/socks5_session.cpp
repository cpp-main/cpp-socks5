#include "socks5_session.h"

#include <tbox/base/defines.h>
#include <tbox/base/assert.h>
#include <tbox/util/string.h>

#include "socks5_proto.h"

//! https://www.ietf.org/rfc/rfc1928.txt

namespace hevake {
namespace socks5 {

using namespace std::placeholders;
using namespace tbox;
using namespace tbox::event;
using namespace tbox::network;
using namespace tbox::main;

Session::Session(Context &ctx, Parent &parent,
                 Token token, TcpConnection *src_conn)
  : ctx_(ctx)
  , parent_(parent)
  , token_(token)
  , src_conn_(src_conn)
  , timeout_timer_(ctx.loop()->newTimerEvent())
{
  src_conn_->setDisconnectedCallback(std::bind(&Session::onSrcTcpDisconnected, this));
  src_conn_->setReceiveCallback(std::bind(&Session::onSrcTcpReceived, this, _1), 0);

  timeout_timer_->initialize(std::chrono::seconds(10), Event::Mode::kOneshot);
  timeout_timer_->setCallback(std::bind(&Session::onTimeout, this));
}

Session::~Session() {
  CHECK_DELETE_RESET_OBJ(dst_ctor_);
  CHECK_DELETE_RESET_OBJ(dst_conn_);
  CHECK_DELETE_RESET_OBJ(src_conn_);
  CHECK_DELETE_RESET_OBJ(timeout_timer_);
}

void Session::start() {
  LogTrace("[%u] session start", token_.id());
  timeout_timer_->enable();
}

void Session::stop() {
  timeout_timer_->disable();

  if (state_ == State::kEstablished) {
    dst_conn_->disconnect();
  } else if (state_ == State::kWaitDnsParseResult) {
    parent_.dns_request().cancel(dns_req_id_);
    dns_req_id_ = 0;
  } else if (state_ == State::kWaitConnectResult) {
    dst_ctor_->stop();
  }

  src_conn_->disconnect();
  state_ = State::kTerm;
  LogTrace("[%u] session stoped", token_.id());
}

void Session::sendToSrc(const void *data_ptr, size_t data_size) {
#if 0
  std::string hex_str = tbox::util::string::RawDataToHexStr(data_ptr, data_size);
  LogTrace("[%u] send: %s", token_.id(), hex_str.c_str());
#endif
  src_conn_->send(data_ptr, data_size);
}

void Session::endSession() {
  stop();
  parent_.onSessionClosed(token_);
}

void Session::onSrcTcpDisconnected() {
  LogTrace("[%u] dst tcp disconnected", token_.id());
  endSession();
}

void Session::onSrcTcpReceived(Buffer &buff) {
#if 0
  std::string hex_str = tbox::util::string::RawDataToHexStr(buff.readableBegin(), buff.readableSize());
  LogTrace("[%u] recv: %s", token_.id(), hex_str.c_str());
#endif

  size_t read_size = 0;

  while (buff.readableSize() > 0) {
    tbox::util::Deserializer parser(buff.readableBegin(), buff.readableSize());
    switch (state_) {
      case State::kWaitMethod:
        read_size = handleAsMethodReq(parser);
        break;

      case State::kWaitConnect:
        read_size = handleAsCmdReq(parser);
        break;

      default:
        endSession();
        return;
    }

    if (read_size == 0)
      break;

    buff.hasRead(read_size);
  }
}

size_t Session::handleAsMethodReq(tbox::util::Deserializer &parser) {
  uint8_t ver, nmethods;
  if (!parser.checkSize(2))
    return 0;

  timeout_timer_->disable();

  parser >> ver >> nmethods;

  if (ver != PROTO_VER || nmethods == 0) {
    LogNotice("[%u] ver:%u, nmethods:%u", token_.id(), ver, nmethods);
    endSession();
    return 0;
  }

  auto support_method = parent_.getMethod();
  bool is_match = false;
  for (size_t i = 0; i < nmethods; ++i) {
    uint8_t method_value;
    if (!parser.fetch(method_value)) {
      endSession();
      return 0;
    }
    PROTO_METHOD method = static_cast<PROTO_METHOD>(method_value);
    if (method == support_method) {
      sendMethodResToSrc(method);
      timeout_timer_->enable();
      is_match = true;
      break;
    }
  }

  if (is_match) {
    state_ = State::kWaitConnect;
  } else {
    LogNotice("[%u] no method match", token_.id());
    sendMethodResToSrc(PROTO_METHOD_NO_ACCEPTABLE_METHOD);
    endSession();
  }

  return parser.pos();
}

void Session::sendMethodResToSrc(PROTO_METHOD method) {
  uint8_t buffer[2] = { 0x05, method };
  sendToSrc(buffer, sizeof(buffer));
}

size_t Session::handleAsCmdReq(tbox::util::Deserializer &parser) {
  uint8_t ver, cmd, rsv, atype;

  if (!parser.checkSize(4))
    return 0;

  timeout_timer_->disable();

  parser >> ver >> cmd >> rsv >> atype;
  if (ver != PROTO_VER) {
    LogNotice("[%u] ver not match", token_.id());
    endSession();
    return 0;
  }

  atype_ = static_cast<PROTO_ATYP>(atype);

  if (atype_ == PROTO_ATYP_IPV4) {
    uint32_t ip_value;
    parser >> ip_value;
    dst_ipv4_ = IPAddress(ip_value);
    LogTrace("[%u] dst_ip: %s", token_.id(), dst_ipv4_.toString().c_str());

  } else if (atype_ == PROTO_ATYP_DOMAINNAME) {
    uint8_t str_len;
    parser >> str_len;
    dst_domainname_ = std::string(static_cast<const char*>(parser.fetchNoCopy(str_len)), str_len);
    LogTrace("[%u] dst_domainname: %s", token_.id(), dst_domainname_.toString().c_str());

  } else if (atype_ == PROTO_ATYP_IPV6) {
    parser.fetch(dst_ipv6_, sizeof(dst_ipv6_));

  } else {
    LogNotice("[%u] unsupport atype: %u", token_.id(), atype);
    endSession();
    return 0;
  }

  parser >> dst_port_;
  LogTrace("[%u] dst_port:%u", token_.id(), dst_port_);

  if (cmd == PROTO_CMD_CONNECT) {
    if (atype_ == PROTO_ATYP_DOMAINNAME) {
      startParseDns();
    } else if (atype_ == PROTO_ATYP_IPV4 || atype_ == PROTO_ATYP_IPV6) {
      startConnect();
    }
  } else if (cmd == PROTO_CMD_BIND) {
    LogUndo();
  } else if (cmd == PROTO_CMD_UDP_ASSOCIATE) {
    LogUndo();
  } else {
    LogNotice("[%u] unsupport cmd: %u", token_.id(), cmd);
    endSession();
  }

  return parser.pos();
}

void Session::sendCmdResToSrc(PROTO_REP rep) {
  std::vector<uint8_t> buffer;
  tbox::util::Serializer dumper(buffer);

  dumper \
    << uint8_t(PROTO_VER)
    << uint8_t(rep)
    << uint8_t(0x00)
    << uint8_t(atype_);

  if (atype_ == PROTO_ATYP_IPV4) {
    dumper << uint32_t(dst_ipv4_);

  } else if (atype_ == PROTO_ATYP_DOMAINNAME) {
    auto str = dst_domainname_.toString();
    dumper << uint8_t(str.length());
    dumper.append(str.data(), str.length());

  } else if (atype_ == PROTO_ATYP_IPV6) {
    dumper.append(dst_ipv6_, sizeof(dst_ipv6_));

  } else {
    LogNotice("[%u] unsupport atype:%u", token_.id(), atype_);
  }

  dumper << dst_port_;

  sendToSrc(buffer.data(), buffer.size());
}

void Session::startParseDns() {
  //! 发起DNS请求
  dns_req_id_ = parent_.dns_request().request(dst_domainname_,
      std::bind(&Session::onParseDnsFinished, this, _1));
  state_ = State::kWaitDnsParseResult;
}

void Session::onParseDnsFinished(const DnsRequest::Result &result) {
  dns_req_id_ = 0;

  if (result.status == DnsRequest::Result::Status::kSuccess) {
    auto &a_vec = result.a_vec;
    if (a_vec.empty()) {
      sendCmdResToSrc(PROTO_REP_HOST_UNREACHABLE);
    } else {
      dst_ipv4_ = a_vec.front().ip;
      LogTrace("[%u] got ip: %s", token_.id(), dst_ipv4_.toString().c_str());
      startConnect();
    }
  } else {
    sendCmdResToSrc(PROTO_REP_HOST_UNREACHABLE);
  }
}

void Session::startConnect() {
  dst_ctor_ = new TcpConnector(ctx_.loop());
  dst_ctor_->initialize(SockAddr(dst_ipv4_, dst_port_));
  dst_ctor_->setConnectedCallback(std::bind(&Session::onDstTcpConnected, this, _1));
  dst_ctor_->setConnectFailCallback(std::bind(&Session::onDstTcpConnectFail, this));
  dst_ctor_->setTryTimes(1);
  dst_ctor_->start();

  state_ = State::kWaitConnectResult;
}

void Session::onDstTcpConnected(tbox::network::TcpConnection *dst_conn) {
  LogTrace("[%u] connect done", token_.id());

  auto tobe_delete = dst_ctor_;
  dst_ctor_ = nullptr;
  ctx_.loop()->runNext([tobe_delete] { delete tobe_delete; });

  dst_conn_ = dst_conn;
  dst_conn_->setDisconnectedCallback(std::bind(&Session::onDstTcpDisconnected, this));
  sendCmdResToSrc(PROTO_REP_SUCCEEDED);

  dst_conn_->bind(src_conn_);
  src_conn_->bind(dst_conn_);

  state_ = State::kEstablished;
}

void Session::onDstTcpConnectFail() {
  LogNotice("[%u] connect fail", token_.id());

  auto tobe_delete = dst_ctor_;
  dst_ctor_ = nullptr;
  ctx_.loop()->runNext([tobe_delete] { delete tobe_delete; });

  sendCmdResToSrc(PROTO_REP_CONN_REFUSED);
  endSession();
}

void Session::onDstTcpDisconnected() {
  endSession();
}

void Session::onTimeout() {
  LogTrace("[%u] timeout", token_.id());
  endSession();
}

}
}
