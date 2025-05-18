#include "backend/hook/memory_api.h"

#include "base/windows_shit.h"
#define EAT_SHIT_FIRST  // prevent linter move windows shit down
#include "base/logging.h"

namespace yrtr {
namespace backend {
namespace hook {

class HandleGuard {
 public:
  HandleGuard(HANDLE handle) : handle_(handle) {}
  ~HandleGuard() { CloseHandle(handle_); }
  HandleGuard(HandleGuard&&) = delete;
  HandleGuard& operator=(HandleGuard&&) = delete;

  HANDLE handle() const { return handle_; }

 private:
  HANDLE handle_;

  DISALLOW_COPY_AND_ASSIGN(HandleGuard);
};

class AllocGuard {
 public:
  AllocGuard(HANDLE handle, LPVOID mem) : handle_(handle), mem_(mem) {}
  AllocGuard(AllocGuard&&) = delete;
  AllocGuard& operator=(AllocGuard&&) = delete;

  LPVOID data() const { return mem_; }

  ~AllocGuard() {
    if (!VirtualFreeEx(handle_, mem_, 0, MEM_RELEASE)) {
      PLOG(WARNING) << "VirtualFreeEx failed";
    }
  }

 private:
  HANDLE handle_;
  LPVOID mem_;

  DISALLOW_COPY_AND_ASSIGN(AllocGuard);
};

std::unique_ptr<MemoryAPI> MemoryAPI::inst_;

void MemoryAPI::Init() {
  inst_ = std::make_unique<MemoryAPI>();
}

void MemoryAPI::Destroy() {
  inst_.reset();
}

MemoryAPI::MemoryAPI() {
  HANDLE hProcess = GetCurrentProcess();
  PCHECK(hProcess != NULL);
  handle_ = std::make_unique<HandleGuard>(hProcess);
}

MemoryAPI::~MemoryAPI() {
  RestoreAllHooks();
}

bool MemoryAPI::CheckHandle() const { return handle_ != nullptr; }

#define CHECK_HANDLE_OR_RETURN_FALSE() \
  if (!CheckHandle()) {                \
    LOG_F(WARNING, "Invalid handle");  \
    return false;                      \
  }

bool MemoryAPI::ReadMemory(uint32_t address, std::span<uint8_t> data) const {
  CHECK_HANDLE_OR_RETURN_FALSE();
  DWORD oldprotect;
  if (!VirtualProtectEx(handle_->handle(), (void*)address, data.size(),
                        PAGE_EXECUTE_READWRITE, &oldprotect)) {
    PLOG_F(WARNING, "VirtualProtectEx failed on addr={}", (void*)address);
    return false;
  }
  if (!ReadProcessMemory(handle_->handle(), (void*)address, data.data(),
                         data.size(), nullptr)) {
    PLOG_F(WARNING, "ReadProcessMemory failed on addr={}", (void*)address);
    return false;
  }
  if (!VirtualProtectEx(handle_->handle(), (void*)address, data.size(),
                        oldprotect, &oldprotect)) {
    PLOG_F(WARNING, "VirtualProtectEx failed on addr={}", (void*)address);
    return false;
  }
  return true;
}

bool MemoryAPI::ReadAddress(uint32_t address, uint32_t* data) const {
  CHECK_HANDLE_OR_RETURN_FALSE();
  CHECK(data != nullptr);
  return ReadMemory(
      address,
      std::span<uint8_t>(reinterpret_cast<uint8_t*>(data), sizeof(uint32_t*)));
}

bool MemoryAPI::WriteMemory(uint32_t address,
                            std::span<const uint8_t> data) const {
  CHECK_HANDLE_OR_RETURN_FALSE();
  DWORD oldprotect;
  if (!VirtualProtectEx(handle_->handle(), (void*)address, data.size(),
                        PAGE_EXECUTE_READWRITE, &oldprotect)) {
    PLOG_F(WARNING, "VirtualProtectEx failed on addr={}", (void*)address);
    return false;
  }
  if (!WriteProcessMemory(handle_->handle(), (void*)address, data.data(),
                          data.size(), nullptr)) {
    PLOG_F(WARNING, "WriteProcessMemory failed on addr={}", (void*)address);
    return false;
  }
  if (!VirtualProtectEx(handle_->handle(), (void*)address, data.size(),
                        oldprotect, &oldprotect)) {
    PLOG_F(WARNING, "VirtualProtectEx failed on addr={}", (void*)address);
    return false;
  }
  return true;
}

bool MemoryAPI::HasHook(const HookPoint hook_point) const {
  return hooks_.contains(hook_point.first);
}

bool MemoryAPI::HookJump(const HookPoint hook_point, void* dest) {
  CHECK_HANDLE_OR_RETURN_FALSE();
  uint32_t addr = hook_point.first;
  uint32_t asm_len = hook_point.second;
  if (hooks_.contains(addr)) {
    return true;
  }
  // jmp <function>
  if (asm_len < 5) {
    asm_len = 5;
  }
  absl::InlinedVector<uint8_t, 10> asm_code(asm_len);
  asm_code[0] = static_cast<uint32_t>(0xE9);  // jmp
  *reinterpret_cast<int32_t*>(&asm_code[1]) =
      static_cast<int32_t>(reinterpret_cast<uint32_t>(dest) - (addr + 5));
  for (size_t i = 5; i < asm_len; i++) {
    asm_code[i] = static_cast<uint32_t>(0x90);  // nop
  }
  return WriteHook(addr, asm_code);
}

bool MemoryAPI::HookNop(const HookPoint hook_point) {
  CHECK_HANDLE_OR_RETURN_FALSE();
  uint32_t addr = hook_point.first;
  uint32_t asm_len = hook_point.second;
  if (hooks_.contains(addr)) {
    return true;
  }
  absl::InlinedVector<uint8_t, 10> asm_code(asm_len, 0x90);
  return WriteHook(addr, asm_code);
}

bool MemoryAPI::RestoreHook(const HookPoint hook_point) {
  uint32_t addr = hook_point.first;
  auto it = hooks_.find(addr);
  if (it == hooks_.end()) {
    return true;
  }
  const absl::InlinedVector<uint8_t, 10>& original_code = it->second;
#ifdef YRTR_DEBUG
  uint32_t asm_len = hook_point.second;
  DCHECK_EQ(asm_len, original_code.size());
#endif
  if (!WriteMemory(addr, original_code)) {
    return false;
  }
  DLOG_F(INFO, "RestoreHook at={:08X} len={}", addr, original_code.size());
  hooks_.erase(it);
  return true;
}

bool MemoryAPI::AutoAssemble(std::string_view script, bool activate) const {
  CHECK_HANDLE_OR_RETURN_FALSE();
  // return _autoassemble(handle_->handle(), std::string(script),
  //                      static_cast<int>(activate));
  // Auto assemble is nice, but it's difficult to plug in complex filters writen
  // in cpp. MSVC inline asm is more "spaghetti" but easier to interact with
  // cpp. Sincerely, it's a tough trade-off.
  UNREFERENCED_PARAMETER(script);
  UNREFERENCED_PARAMETER(activate);
  NOT_IMPLEMENTED();
}

#undef CHECK_HANDLE_OR_RETURN_FALSE

bool MemoryAPI::WriteHook(uint32_t addr, std::span<const uint8_t> code) {
  absl::InlinedVector<uint8_t, 10> original_code(code.size(), 0);
  if (!ReadMemory(addr, /*out*/ original_code)) {
    return false;
  }
  if (!WriteMemory(addr, code)) {
    return false;
  }
  DLOG_F(INFO, "Hook at={:08X} len={}", addr, code.size());
  hooks_.emplace(addr, std::move(original_code));
  return true;
}

void MemoryAPI::RestoreAllHooks() {
  for (const auto& [addr, original_code] : hooks_) {
    DLOG_F(INFO, "RestoreHook at={:08X} len={}", addr, original_code.size());
    WriteMemory(addr, original_code);
  }
  hooks_.clear();
}

}  // namespace hook
}  // namespace backend
}  // namespace yrtr
