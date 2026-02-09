#pragma once
#include <string>

#include "base/logging.h"
#include "base/windows_shit.h"

namespace yrtr {
namespace utils {

template <unsigned int CodePage>
std::string Utf16To(std::wstring_view u16str) {
  if (u16str.empty()) {
    return "";
  }
  int buflen = WideCharToMultiByte(CodePage, 0, u16str.data(), -1, nullptr, 0,
                                   nullptr, nullptr);
  if (buflen == 0) {
    LOG_F(ERROR, "Failed to determine buffer size for wide string.");
    return "";
  }
  std::string mbstr(buflen, '\0');
  int result = WideCharToMultiByte(CodePage, 0, u16str.data(), -1, &mbstr[0],
                                   buflen, nullptr, nullptr);
  if (result == 0) {
    LOG_F(ERROR, "Failed to convert wide string to multibyte string.");
    return "";
  }
  mbstr.resize(buflen - 1);
  return mbstr;
}

inline std::string Utf16ToUtf8(std::wstring_view u16str) {
  return Utf16To<CP_UTF8>(u16str);
}

inline std::string Utf16ToAcp(std::wstring_view u16str) {
  return Utf16To<CP_ACP>(u16str);
}

template <unsigned int CodePage>
std::wstring Utf16From(std::string_view mbstr) {
  if (mbstr.empty()) {
    return L"";
  }
  int buflen = MultiByteToWideChar(CodePage, 0, mbstr.data(),
                                   static_cast<int>(mbstr.size()), nullptr, 0);
  if (buflen == 0) {
    LOG_F(ERROR, "Failed to determine buffer size for multibyte string.");
    return L"";
  }
  std::wstring u16str(buflen, L'\0');
  int result = MultiByteToWideChar(CodePage, 0, mbstr.data(),
                                   static_cast<int>(mbstr.size()),
                                   u16str.data(), buflen);
  if (result == 0) {
    LOG_F(ERROR, "Failed to convert utf-8 string to utf-16 string.");
    return L"";
  }
  return u16str;
}

inline std::string Utf8ToAcp(std::string_view mbstr) {
  return Utf16ToAcp(Utf16From<CP_UTF8>(mbstr));
}

inline std::wstring Utf8ToUtf16(std::string_view mbstr) {
  return Utf16From<CP_UTF8>(mbstr);
}

inline std::wstring AcpToUtf16(std::string_view mbstr) {
  return Utf16From<CP_ACP>(mbstr);
}

}  // namespace utils
}  // namespace yrtr
