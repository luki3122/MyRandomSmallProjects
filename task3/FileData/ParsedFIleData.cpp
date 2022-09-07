#include "ParsedFileData.hh"
#include <sstream>

ParsedFileData::~ParsedFileData() {}

std::string ParsedFileData::getFileName() const { return _file_name; }

void ParsedFileData::setFileName(const std::string &file_name) {
  _file_name = file_name;
}

long ParsedFileData::getNOLines() const { return _no_lines; }

void ParsedFileData::setNOLines(const long no_lines) { _no_lines = no_lines; }

long ParsedFileData::getNOCommentedLines() const { return _no_commented_lines; }

void ParsedFileData::setNOCommentedLines(const long no_commented_lines) {
  _no_commented_lines = no_commented_lines;
}

long ParsedFileData::getNOCodeLines() const { return _no_code_lines; }

void ParsedFileData::setNOCodeLines(const long no_code_lines) {
  _no_code_lines = no_code_lines;
}

long ParsedFileData::getNOBlankLines() const { return _no_blank_lines; }

void ParsedFileData::setNOBlankLines(const long no_blank_lines) {
  _no_blank_lines = no_blank_lines;
}

std::chrono::duration<double, std::milli>
ParsedFileData::getParsingDuration() const {
  return _parsing_duration;
}

void ParsedFileData::setParsingDuration(
    const std::chrono::duration<double, std::milli> parsing_duration) {
  _parsing_duration = parsing_duration;
}

std::string ParsedFileData::getFormatedDuration() const {
  auto time = getParsingDuration();
  auto m = std::chrono::duration_cast<std::chrono::minutes>(time);
  time -= m;
  auto s = std::chrono::duration_cast<std::chrono::seconds>(time);
  time -= s;
  auto mili = std::chrono::duration_cast<std::chrono::milliseconds>(time);
  time -= mili;
  auto micro = std::chrono::duration_cast<std::chrono::microseconds>(time);
  std::stringstream ss;
  ss << m.count() << "m:" << s.count() << "s:" << mili.count() << "ms";

  return ss.str();
}

ParsedFileData ParsedFileData::operator+(const ParsedFileData &data) {
  ParsedFileData temp(*this);
  temp += data;
  return temp;
}

ParsedFileData &ParsedFileData::operator+=(const ParsedFileData &data) {
  _file_name = "";
  _no_lines += data.getNOLines();
  _no_commented_lines += data.getNOCommentedLines();
  _no_code_lines += data.getNOCodeLines();
  _no_blank_lines += data.getNOBlankLines();
  return *this;
}

std::string ParsedFileData::print() const {
  std::stringstream ss;
  if (_file_name != "")
    ss << "File name:   " << getFileName() << "\n";

  ss << "Lines:       " << getNOLines() << '\n'
     << "Comments:    " << getNOCommentedLines() << '\n'
     << "Code lines:  " << getNOCodeLines() << '\n'
     << "Blank lines: " << getNOBlankLines() << '\n'
     << "Parsed in:   " << getFormatedDuration() << '\n';
  return ss.str();
}

// std::ostream &operator<<(std::ostream &output, const ParsedFileData &data) {
//   output << data._no_lines << " " << data._no_commented_lines << " "
//          << data._no_code_lines << " " << data._no_blank_lines;
//   return output;
// }