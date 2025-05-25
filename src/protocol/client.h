#pragma once
#include <atomic>
#include <thread>

#include "base/macro.h"
#include "base/task_queue.h"
#include "frontend/desktop/gui.h"
#include "protocol/model.h"
#include "websocketpp/client.hpp"
#include "websocketpp/config/asio_no_tls_client.hpp"

namespace yrtr {

class Client {
 public:
  static constexpr int kTimerIdUpdateState = 1;

  Client(frontend::Gui& gui, uint16_t port);
  ~Client();
  // Despite invoked by render loop, I'm still gonna name it "Update".
  void Update();
  // Use an explicit stop to put it before the window destroied. Otherwise if
  // someday a background buggy thread blocks after the main window closed,
  // it leaves a zombie background process that really hard to be noticed.
  void Stop();
  void GetState();

 private:
  using WebsocketClient = websocketpp::client<websocketpp::config::asio_client>;
  // Limit maximum pending state getting requests.
  static constexpr int kMaxGetState = 1;
  static constexpr int kReconnectIntervalSeconds = 1;
  const std::string uri_;
  frontend::Gui& gui_;
  std::thread evloop_;
  WebsocketClient cli_;
  WebsocketClient::connection_ptr conn_;
  TaskQueue render_loop_ch_;
  std::atomic<int> get_state_count_;
  std::atomic<bool> stop_;

  WebsocketClient::connection_ptr GetOrCreateConn();
  void OnWebsocketMessage(WebsocketClient& cli, websocketpp::connection_hdl hdl,
                          WebsocketClient::message_ptr msg);
  void SendGetState();
  void ParseState(const std::string& data);
  void SendPostInput(FnLabel label, uint32_t val);
  void SendPostButton(FnLabel label);
  void SendPostCheckbox(FnLabel label, bool activate);
  void SendPostProtectedList(SideMap&& side_map);
  void SendPostData(std::string_view path, std::string&& data);

  DISALLOW_COPY_AND_ASSIGN(Client);
};

}  // namespace yrtr
