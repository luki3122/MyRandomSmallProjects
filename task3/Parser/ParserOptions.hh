#pragma once

#include <filesystem>
#include <list>

namespace fs = std::filesystem;
class ParserOptions {
private:
  std::list<fs::path> _extensions;

public:
  ParserOptions();
  ParserOptions(std::list<fs::path> &extensions);

  const std::list<fs::path> &getExtensions() const;
  void setExtensions(const std::list<fs::path> &extensions);
  void addExtension(const fs::path &extension);
  void removeExtension(const fs::path &extension);
};