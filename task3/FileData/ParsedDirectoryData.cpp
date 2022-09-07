#include "ParsedDirectoryData.hh"
#include "ParsedFileData.hh"

ParsedDirectoryData::~ParsedDirectoryData() {}

std::string ParsedDirectoryData::getDirName() const { return _dir_name; }

void ParsedDirectoryData::setDirName(const std::string &dir_name) {
  _dir_name = dir_name;
}

const std::list<ParsedFileData> &ParsedDirectoryData::getFileDataList() const {
  return _file_data_list;
}

void ParsedDirectoryData::setFileDataList(
    const std::list<ParsedFileData> &file_data_list) {
  _file_data_list = file_data_list;
}

void ParsedDirectoryData::addFileData(const ParsedFileData &file_data) {
  _file_data_list.push_front(file_data);
}

std::chrono::duration<double, std::milli>
ParsedDirectoryData::getParsingDuration() const {
  return _parsing_duration;
}

void ParsedDirectoryData::setParsingDuration(
    const std::chrono::duration<double, std::milli> parsing_duration) {
  _parsing_duration = parsing_duration;
}

std::string ParsedDirectoryData::print() const {
  ParsedFileData temp;
  for (const ParsedFileData &data : getFileDataList()) {
    temp += data;
  }
  temp.setFileName(getDirName());
  return temp.print();
}
