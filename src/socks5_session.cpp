#include "socks5_session.h"

#include <tbox/base/defines.h>
#include <tbox/util/string.h>

#include "socks5_proto.h"

//! https://www.ietf.org/rfc/rfc1928.txt

namespace hevake {
namespace socks5 {

using namespace std::placeholders;

Session::Session(tbox::main::Context &ctx, Parent &parent,
                 Token token, tbox::network::TcpConnection *src_conn)
  : ctx_(ctx)
  , parent_(parent)
  , token_(token)
  , src_conn_(src_conn)
{
  src_conn_->setDisconnectedCallback(std::bind(&Session::onSrcTcpDisconnected, this));
  src_conn_->setReceiveCallback(std::bind(&Session::onSrcTcpReceived, this, _1), 0);
}

Session::~Session() {
  CHECK_DELETE_RESET_OBJ(dst_conn_);
  CHECK_DELETE_RESET_OBJ(src_conn_);
}

void Session::onSrcTcpDisconnected() {
  LogTag();
  parent_.onSessionClosed(token_);
}

void Session::onSrcTcpReceived(tbox::network::Buffer &buff) {
  std::string hex_str = tbox::util::string::RawDataToHexStr(buff.readableBegin(), buff.readableSize());
  LogDbg("recv: %s", hex_str.c_str());

  buff.hasReadAll();
}

}
}
