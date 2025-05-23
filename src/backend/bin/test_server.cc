#include <signal.h>

#include <chrono>
#include <filesystem>
namespace fs = std::filesystem;
#include <memory>

#include "backend/config.h"
#include "backend/hook/mock_trainer.h"
#include "base/logging.h"
#include "base/macro.h"
__YRTR_BEGIN_THIRD_PARTY_HEADERS
#include "absl/debugging/symbolize.h"
__YRTR_END_THIRD_PARTY_HEADERS
#include "base/thread.h"
#include "protocol/server.h"

namespace yrtr {
static std::unique_ptr<backend::hook::MockTrainer> trainer;
static std::unique_ptr<Server> server;

static void Init(const char* exe_path) {
  absl::InitializeSymbolizer(exe_path);
  logging::InitLogging();
  // Setup thread id.
  SetupGameLoopThreadOnce();
  CHECK(backend::Config::Load(fs::canonical(fs::path(exe_path)).parent_path()));
  trainer = std::make_unique<backend::hook::MockTrainer>();
  server = std::make_unique<Server>(trainer.get(),
                                    backend::Config::instance()->port());
}

static void Update() {
  DCHECK(IsWithinGameLoopThread());
  DCHECK_NOTNULL(trainer);
  trainer->Update(/*delta*/ 0.015);
  DCHECK_NOTNULL(server);
  server->Update();
}

static void OnExit() {
  server->Stop();
  server.reset();
  trainer.reset();
}
}  // namespace yrtr

volatile bool should_stop = false;

static void SignalHandlerToStopServer(int signal) {
  should_stop = true;
}

int main(int argc, char* argv[]) {
  signal(SIGINT, SignalHandlerToStopServer);
  // Simulate game loop.
  yrtr::Init(argv[0]);
  while (!should_stop) {
    yrtr::Update();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
  yrtr::OnExit();
}
