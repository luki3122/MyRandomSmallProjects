#pragma once

#include "../../Definitions.hh"
#include "../FileData/FileData.hh"
#include "../FileData/ParsedDirectoryData.hh"
#include "ParserOptions.hh"
#include <filesystem>
#include <list>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

class Parser {
private:
  ParserOptions _options;
  static void checkIfFileExists(const fs::path &path);
  static void checkIfFileIsDirectory(const fs::path &path);
  static void checkIfFileIsRegular(const fs::path &path);

  bool fileShouldBeParsed(const fs::path &path);

public:
  Parser() = default;
  Parser(ParserOptions &options);
  Parser(Parser &) = delete;
  // Parser(Parser &&parser);
  virtual ~Parser();

  const ParserOptions &getOptions() const;
  void setOptions(const ParserOptions &options);

  IParsedData *parse(const fs::path &path);
  ParsedFileData parseFile(const fs::path &path);
  ParsedDirectoryData parseDirectory(const fs::path &path);
  ParsedDirectoryData parseDirectoryAsync(const fs::path &path);
};
