#pragma once
#include "IParsedData.hh"
#include "ParsedFileData.hh"
#include <list>

class ParsedDirectoryData : public IParsedData {
private:
  std::string _dir_name;
  std::list<ParsedFileData> _file_data_list;
  std::chrono::duration<double, std::milli> _parsing_duration;

public:
  virtual ~ParsedDirectoryData();

  std::string getDirName() const;
  void setDirName(const std::string &dir_name);
  const std::list<ParsedFileData> &getFileDataList() const;
  void setFileDataList(const std::list<ParsedFileData> &file_data_list);
  void addFileData(const ParsedFileData &file_data);
  std::chrono::duration<double, std::milli> getParsingDuration() const;
  void setParsingDuration(
      const std::chrono::duration<double, std::milli> parsing_duration);

  std::string print() const override;
};