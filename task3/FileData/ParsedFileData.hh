#pragma once

#include "../Definitions.hh"
#include "IParsedData.hh"
#include <chrono>
#include <ostream>
#include <string>

class ParsedFileData : public IParsedData {

private:
  std::string _file_name;
  long _no_lines = 0;
  long _no_commented_lines = 0;
  long _no_code_lines = 0;
  long _no_blank_lines = 0;
  std::chrono::duration<double, std::milli> _parsing_duration;

public:
  // ParsedFileData() = default;
  // ParsedFileData(const ParsedFileData &data) = default;
  // ParsedFileData(ParsedFileData &&data) = default;
  virtual ~ParsedFileData();

  std::string getFileName() const;
  void setFileName(const std::string &file_name);

  long getNOLines() const;
  void setNOLines(const long no_lines);

  long getNOCommentedLines() const;
  void setNOCommentedLines(const long no_commented_lines);

  long getNOCodeLines() const;
  void setNOCodeLines(const long no_code_lines);

  long getNOBlankLines() const;
  void setNOBlankLines(const long no_blank_lines);

  std::chrono::duration<double, std::milli> getParsingDuration() const;
  void setParsingDuration(
      const std::chrono::duration<double, std::milli> parsing_duration);

  std::string getFormatedDuration() const;

  ParsedFileData operator+(const ParsedFileData &data);
  ParsedFileData &operator+=(const ParsedFileData &data);

  std::string print() const override;
  // friend std::ostream &operator<<(std::ostream &output,
  //                                 const ParsedFileData &data);
};
