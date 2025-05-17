#pragma once
#include <array>
#include <filesystem>
namespace fs = std::filesystem;
#include <format>
#include <ranges>
#include <vector>

template <>
struct std::formatter<fs::path> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
  auto format(const fs::path& path, std::format_context& ctx) const {
    return std::format_to(ctx.out(), "{}", path.string());
  }
};

template <class T>
struct std::formatter<std::vector<T>> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
  auto format(const std::vector<T>& vec, std::format_context& ctx) const {
    if (vec.empty()) {
      return std::format_to(ctx.out(), "[]");
    }
    std::stringstream ss;
    ss << "[";
    for (auto& val : vec | std::views::take(vec.size() - 1)) {
      ss << val << ", ";
    }
    ss << vec[vec.size() - 1] << "]";
    return std::format_to(ctx.out(), "{}", ss.str());
  }
};

template <class T, size_t N>
struct std::formatter<std::array<T, N>> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
  auto format(const std::array<T, N>& vec, std::format_context& ctx) const {
    if (vec.empty()) {
      return std::format_to(ctx.out(), "[]");
    }
    std::stringstream ss;
    ss << "[";
    for (auto& val : vec | std::views::take(vec.size() - 1)) {
      ss << val << ", ";
    }
    ss << vec[vec.size() - 1] << "]";
    return std::format_to(ctx.out(), "{}", ss.str());
  }
};
