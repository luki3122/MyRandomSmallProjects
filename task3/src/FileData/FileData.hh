#pragma once

#include "ParsedFileData.hh"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;
class FileData {
private:
  std::string _file_name;
  fs::path _file_path;
  ParsedFileData _parsed_file_data;

public:
  // FileData() = default;
  // FileData(const FileData &) = default;
  // FileData(FileData &&) = default;
  virtual ~FileData();

  //
  std::string getFileName() const;
  void setFileName(const std::string &file_name);

  fs::path getFilePath() const;
  void setFileName(const fs::path &file_path);

  ParsedFileData getParsedFileData() const;
  void setParsedFileData(const ParsedFileData &parsed_file_data);
};
