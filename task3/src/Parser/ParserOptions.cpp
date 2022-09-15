#include "ParserOptions.hh"

ParserOptions::ParserOptions() {
  _extensions.push_back(fs::path(".c"));
  _extensions.push_back(fs::path(".cpp"));
  _extensions.push_back(fs::path(".h"));
  _extensions.push_back(fs::path(".hpp"));
  _extensions.push_back(fs::path(".hh"));
}

ParserOptions::ParserOptions(std::list<fs::path> &extensions)
    : _extensions(extensions) {}

const std::list<fs::path> &ParserOptions::getExtensions() const {
  return _extensions;
}

void ParserOptions::setExtensions(const std::list<fs::path> &extensions) {
  _extensions = extensions;
}

void ParserOptions::addExtension(const fs::path &extension) {
  _extensions.push_back(extension);
}

void ParserOptions::removeExtension(const fs::path &extension) {
  _extensions.remove(extension);
}
