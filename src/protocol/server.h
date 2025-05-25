#pragma once
#include <set>
#include <thread>

#include "backend/hook/trainer.h"
#include "base/macro.h"
#include "base/task_queue.h"
#include "protocol/model.h"
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

namespace yrtr {

class Server {
 public:
  Server(backend::hook::ITrainer* trainer, uint16_t port);
  void Stop();
  void Update();

 private:
  using WebsocketServer = websocketpp::server<websocketpp::config::asio>;
  backend::hook::ITrainer* trainer_;
  std::thread evloop_;
  WebsocketServer svr_;
  // Record connection to propagate state.
  std::set<websocketpp::connection_hdl,
           std::owner_less<websocketpp::connection_hdl>>
      conns_;
  TaskQueue game_loop_ch_;

  void OnOpenConn(websocketpp::connection_hdl hdl);
  void OnCloseConn(websocketpp::connection_hdl hdl);
  void OnMessage(WebsocketServer& svr, websocketpp::connection_hdl hdl,
                 WebsocketServer::message_ptr msg);
  void OnStateUpdated(State state);
  void SendState(State&& state, websocketpp::connection_hdl hdl);
  void OnPostInputEvent(Event<uint32_t>&& event);
  void OnPostButtonEvent(Event<int>&& event);
  void OnPostCheckboxEvent(Event<bool>&& event);
  void OnPostProtectedListEvent(Event<SideMap>&& event);

  DISALLOW_COPY_AND_ASSIGN(Server);
};

}  // namespace yrtr
