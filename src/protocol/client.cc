#include "protocol/client.h"

#include <chrono>

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/logging.h"
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/synchronization/mutex.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "base/thread.h"
#include "gsl/util"

namespace yrtr {

#define BIND_BUTTON(fnname)                                                  \
  gui.AddButtonListener(FnLabel::k##fnname, [this]() {                       \
    boost::asio::dispatch(cli_.get_io_service(),                             \
                          [this]() { SendPostButton(FnLabel::k##fnname); }); \
  });
#define BIND_CHECKBOX(fnname)                                         \
  gui.AddCheckboxListener(FnLabel::k##fnname, [this](bool activate) { \
    thread_pool_.enqueue([this, activate]() {                         \
      SendPostCheckbox(FnLabel::k##fnname, activate);                 \
    });                                                               \
  });

Client::Client(frontend::Gui& gui, uint16_t port)
    : uri_(std::format("ws://localhost:{}", port)),
      gui_(gui),
      conn_(nullptr),
      get_state_count_(0),
      stop_(false) {
  render_loop_ch_.SetThreadId(GetRendererThreadId());
  // This tool is used in LAN, 50ms should be enough. Large timeout queues too
  // many jobs in sending queue when the backend is not running, making the gui
  // actions, like close window, acts lagging.
  cli_.set_open_handshake_timeout(50);
  cli_.set_close_handshake_timeout(50);
  cli_.set_pong_timeout(200);
  cli_.set_access_channels(websocketpp::log::alevel::all);
  cli_.clear_access_channels(websocketpp::log::alevel::frame_payload);
  cli_.init_asio();
  cli_.set_message_handler([this](websocketpp::connection_hdl hdl,
                                  WebsocketClient::message_ptr msg) {
    OnWebsocketMessage(cli_, hdl, msg);
  });
  evloop_ = std::thread([&]() {
    SetupNetThreadOnce();
    while (!stop_.load()) {
      GetOrCreateConn();
      // Would block here. Exit if the remote endpoint resets the connection,
      // client should always retry.
      cli_.run();
    }
    LOG_F(INFO, "Client exit");
  });

  gui.AddHouseListListener([this](SideMap&& side_map) {
    UNREFERENCED_PARAMETER(side_map);
    // thread_pool_.enqueue([this, side_map = std::move(side_map)]() mutable {
    //   SendPostProtectedList(std::move(side_map));
    // });
  });
  gui.AddInputListener(FnLabel::kApply, [this](uint32_t val) {
    UNREFERENCED_PARAMETER(val);
    // thread_pool_.enqueue(
    //     [this, val]() { SendPostInput(FnLabel::kApply, val); });
  });
  BIND_BUTTON(FastBuild);
  BIND_BUTTON(DeleteUnit);
  BIND_BUTTON(ClearShroud);
  BIND_BUTTON(GiveMeABomb);
  BIND_BUTTON(UnitLevelUp);
  BIND_BUTTON(UnitSpeedUp);
  BIND_BUTTON(IAMWinner);
  BIND_BUTTON(ThisIsMine);
  // BIND_CHECKBOX(God);
  // BIND_CHECKBOX(InstBuild);
  // BIND_CHECKBOX(UnlimitSuperWeapon);
  // BIND_CHECKBOX(InstFire);
  // BIND_CHECKBOX(InstTurn);
  // BIND_CHECKBOX(RangeToYourBase);
  // BIND_CHECKBOX(FireToYourBase);
  // BIND_CHECKBOX(FreezeGapGenerator);
  // BIND_CHECKBOX(SellTheWorld);
  // BIND_CHECKBOX(BuildEveryWhere);
  // BIND_CHECKBOX(AutoRepair);
  // BIND_CHECKBOX(MakeGarrisonedMine);
  // BIND_CHECKBOX(InvadeMode);
  // BIND_CHECKBOX(UnlimitTech);
  // BIND_CHECKBOX(UnlimitFirePower);
  // BIND_CHECKBOX(InstChrono);
  // BIND_CHECKBOX(SpySpy);
  // BIND_CHECKBOX(AdjustGameSpeed);
}

#undef BIND_BUTTON
#undef BIND_CHECKBOX

Client::~Client() {}

void Client::Stop() {
  stop_.store(true);
  boost::asio::dispatch(cli_.get_io_service(), [&]() {
    if (conn_ != nullptr &&
        conn_->get_state() < websocketpp::session::state::value::closing) {
      websocketpp::lib::error_code ec;
      conn_->close(websocketpp::close::status::normal, "stop", ec);
      if (ec) {
        LOG_F(ERROR, "Failed to close connection err=[{}]{}", ec.value(),
              ec.message());
      }
    }
  });
  evloop_.join();
}

void Client::Update() {
  DCHECK(IsWithinRendererThread());
  render_loop_ch_.ExecuteTasks();
}

void Client::GetState() {
  DCHECK(IsWithinRendererThread());
  // thread_pool_.enqueue([this]() { SendGetState(); });
}

Client::WebsocketClient::connection_ptr Client::GetOrCreateConn() {
  DCHECK(IsWithinNetThread());
  if (conn_ != nullptr &&
      conn_->get_state() < websocketpp::session::state::value::closing) {
    return conn_;
  } else {
    websocketpp::lib::error_code ec;
    // Drop expired dangling connection.
    if (conn_ != nullptr) {
      conn_->close(websocketpp::close::status::normal, "stop", ec);
      // Drop error code anyway.
      UNREFERENCED_PARAMETER(ec);
    }
    // Create new one.
    conn_ = cli_.get_connection(uri_, ec);
    if (ec) {
      LOG(ERROR) << "could not create connection because: " << ec.message();
      return nullptr;
    }
    cli_.connect(conn_);
    return conn_;
  }
}

void Client::OnWebsocketMessage(WebsocketClient& cli,
                                websocketpp::connection_hdl hdl,
                                WebsocketClient::message_ptr msg) {
  LOG(INFO) << "on_message called with hdl: " << hdl.lock().get()
            << " and message: " << msg->get_payload();
  UNREFERENCED_PARAMETER(cli);
}

void Client::SendGetState() {
  DCHECK(IsWithinNetThread());
  // TODO
  // auto _ = gsl::finally([&]() { get_state_count_.fetch_sub(1); });
  // if (get_state_count_.fetch_add(1) + 1 > kMaxGetState) {
  //   return;
  // }
  // httplib::Result res = cli_.Get(kApiGetState.data());
  // if (res) {
  //   if (res->status == 200) {
  //     ParseState(res->body);
  //   } else {
  //     LOG_F(ERROR, "Failed to get state with code={}", res->status);
  //   }
  // } else {
  //   DLOG_F(ERROR, "Failed to get state with error={}",
  //          httplib::to_string(res.error()));
  // }
}

void Client::ParseState(const std::string& data) {
  DCHECK(IsWithinNetThread());
  render_loop_ch_.ExecuteOrScheduleTask([this, data = data]() {
    DCHECK(IsWithinRendererThread());
    State state = json::parse(data);
    gui_.UpdateState(state);
  });
}

void Client::SendPostInput(FnLabel label, uint32_t val) {
  DCHECK(IsWithinNetThread());
  SendPostData(kApiPostEvent, MakeInputEvent(label, val));
}

void Client::SendPostButton(FnLabel label) {
  DCHECK(IsWithinNetThread());
  SendPostData(kApiPostEvent, MakeButtonEvent(label));
}

void Client::SendPostCheckbox(FnLabel label, bool activate) {
  DCHECK(IsWithinNetThread());
  SendPostData(kApiPostEvent, MakeCheckboxEvent(label, activate));
}

void Client::SendPostProtectedList(SideMap&& side_map) {
  DCHECK(IsWithinNetThread());
  SendPostData(kApiPostEvent, MakeProtectedListEvent(std::move(side_map)));
}

void Client::SendPostData(std::string_view /*path*/, std::string&& data) {
  DCHECK(IsWithinNetThread());
  WebsocketClient::connection_ptr conn = GetOrCreateConn();
  if (conn == nullptr) {
    LOG_F(WARNING, "Failed to send post data, connection not established");
    return;
  }
  if (conn->get_state() != websocketpp::session::state::open) {
    // The connection is not prepared.
    LOG_F(WARNING,
          "Failed to send post data, connection not prepared at state={}",
          static_cast<int>(conn->get_state()));
    return;
  }
  websocketpp::lib::error_code ec =
      conn->send(data, websocketpp::frame::opcode::text);
  if (ec) {
    LOG_F(ERROR, "Failed to send post data with error=[{}]{}", ec.value(),
          ec.message());
  }
}

}  // namespace yrtr
