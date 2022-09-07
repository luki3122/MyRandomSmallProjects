#include "FileData.hh"

std::string FileData::getFileName() const { return _file_name; }

void FileData::setFileName(const std::string &file_name) {
  _file_name = file_name;
}

fs::path FileData::getFilePath() const { return _file_path; }

void FileData::setFileName(const fs::path &file_path) {
  _file_path = file_path;
}

ParsedFileData FileData::getParsedFileData() const { return _parsed_file_data; }

void FileData::setParsedFileData(const ParsedFileData &parsed_file_data) {
  _parsed_file_data = parsed_file_data;
}
FileData::~FileData() {}
