#include "Parser.hh"
#include <algorithm>
#include <bits/chrono.h>
#include <chrono>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <mutex>
#include <ratio>
#include <sstream>
#include <string>
#include <system_error>
#include <thread>
// ParseExeption
ParserExeption::ParserExeption(const std::string &message) : message(message){};
const char *ParserExeption::what() const noexcept { return message.c_str(); };

// FileData
FileData::FileData()
    : _no_lines(0), _no_commented_lines(0), _no_blank_lines(0),
      _no_code_lines(0), _no_files(0), _is_a_directory(false) {}

FileData::FileData(const long &no_lines, const long &no_commented_lines,
                   const long &no_code_lines, const long &no_blank_lines,
                   const long &no_files, const bool &is_a_directory,
                   const auto time_elapsed) noexcept
    : _no_lines(no_lines), _no_commented_lines(no_commented_lines),
      _no_code_lines(no_code_lines), _no_blank_lines(no_blank_lines),
      _no_files(no_files), _is_a_directory(is_a_directory),
      _time_elapsed(time_elapsed) {}

long FileData::getNOLines() const { return _no_lines; }

long FileData::getNOCommentedLines() const { return _no_commented_lines; }

long FileData::getNOCodeLines() const { return _no_code_lines; }

long FileData::getNOBlankLines() const { return _no_blank_lines; }

bool FileData::getIsADirectory() const { return _is_a_directory; }

std::chrono::duration<double, std::milli> FileData::getTimeElapsed() const {
  return _time_elapsed;
}

std::string FileData::getFormatedTime() const {
  std::string res = "";
  auto time = getTimeElapsed();
  auto m = std::chrono::duration_cast<std::chrono::minutes>(time);
  time -= m;
  auto s = std::chrono::duration_cast<std::chrono::seconds>(time);
  time -= s;
  auto mili = std::chrono::duration_cast<std::chrono::milliseconds>(time);
  time -= mili;
  auto micro = std::chrono::duration_cast<std::chrono::microseconds>(time);
  std::stringstream ss;
  ss << m.count() << "m:" << s.count() << "s:" << mili.count() << "ms";
  ss >> res;
  return res;
}

long FileData::getNOFiles() const { return _no_files; }

FileData::FileData(const FileData &data) noexcept
    : FileData(data.getNOLines(), data.getNOCommentedLines(),
               data.getNOCodeLines(), data.getNOBlankLines(), data.getNOFiles(),
               data.getIsADirectory(), data.getTimeElapsed()) {}

FileData FileData::operator+(const FileData &data) {
  FileData temp(*this);
  temp += data;
  return temp;
}

FileData &FileData::operator+=(const FileData &data) {
  std::unique_lock lock(_file_data_mutex);
  _no_lines += data.getNOLines();
  _no_commented_lines += data.getNOCommentedLines();
  _no_code_lines += data.getNOCodeLines();
  _no_blank_lines += data.getNOBlankLines();
  _no_files += data.getNOFiles();
  _is_a_directory = true;
  _time_elapsed += data.getTimeElapsed();
  return *this;
}

std::ostream &operator<<(std::ostream &output, const FileData &data) {
  output << "Lines:           " << data.getNOLines() << '\n'
         << "Lines of code:   " << data.getNOCodeLines() << '\n'
         << "Commented lines: " << data.getNOCommentedLines() << '\n'
         << "Blank lines:     " << data.getNOBlankLines() << '\n'
         << "Files parsed:    " << data.getNOFiles() << '\n'
         << "Is a directory:  " << data.getIsADirectory() << '\n'
         << "Time elapsed:    " << data.getFormatedTime() << '\n';
  return output;
}

void Parser::checkIfFileExists(const fs::path &path) {
  if (fs::exists(path))
    return;
  throw ParserExeption(
      std::string("File does not exist: ").append(path.string()));
}
void Parser::checkIfFileIsDirectory(const fs::path &path) {
  if (fs::is_directory(path))
    return;
  throw ParserExeption(
      std::string("File is not a directory: ").append(path.string()));
}

void Parser::checkIfFileIsRegular(const fs::path &path) {
  if (fs::is_regular_file(path))
    return;
  throw ParserExeption(
      std::string("File is not regular file: ").append(path.string()));
}

void Parser::parseArguments(const int argc, const char **args) {
  std::list<fs::path> temp;
  _extensions.push_back(fs::path(".cpp"));
  _extensions.push_back(fs::path(".hpp"));
  _extensions.push_back(fs::path(".c"));
  _extensions.push_back(fs::path(".h"));

  if (argc <= 1) {
    _path = "./";
  }
  try {
    if (argc > 1) {
      _path = fs::path(args[1]);
    }
    if (argc > 2) {
      _log_file_path = fs::path(args[2]);
    }
  } catch (std::exception e) {
    std::cerr << e.what() << '\n';
    _path = "";
    _log_file_path = "";
    _extensions.clear();
  }
  std::cerr << "Path: " << _path << "\nExt: " << _extensions.front() << '\n';
}

fs::path Parser::popFile() {
  std::unique_lock lock(_files_mutex);
  fs::path p;
  p.clear();
  if (hasFiles()) {
    p = _files.front();
    _files.pop_front();
  }
  return p;
};
void Parser::addFile(fs::path const &path) {
  std::unique_lock lock(_files_mutex);
  _files.push_back((path));
};

bool Parser::hasFiles() const { return !_files.empty(); };

void Parser::threadFunction() {

  FileData data;
  while (getRunning() || hasFiles()) {
    fs::path path = popFile();
    if (!path.empty())
      data += parseFile(path);
  }

  std::unique_lock res(_result_mutex);
  _result += data;
};

void Parser::startThreads() {
  setRunning(true);
  for (size_t i = _number_of_threads; i; i--) {
    _threads.push_back(std::thread(&Parser::threadFunction, this));
  }
}

FileData Parser::getResult() {
  setRunning(false);
  for (std::thread &thr : _threads) {
    thr.join();
  }
  return _result;
}

bool Parser::getRunning() {
  std::unique_lock lock(_running_mutex);
  return _running;
}

void Parser::setRunning(bool running) {
  std::unique_lock lock(_running_mutex);
  _running = running;
}

Parser::Parser() : _path("") {}

Parser::Parser(const fs::path &path) : _path(path) {}

Parser::Parser(const fs::path &path, const std::list<fs::path> &extensions)
    : Parser(path) {
  _extensions = extensions;
}

Parser::Parser(const int argc, const char **args) {
  parseArguments(argc, args);
}

FileData Parser::parse() {
  checkIfFileExists(_path);
  FileData result;
  if (fs::is_directory(_path))
    return parseDirectory(_path, _extensions);
  else
    return parseFile(_path);
}

void Parser::parseToFile() {
  std::ofstream output(_log_file_path);

  output << parse();
}

FileData Parser::parseFile(const fs::path &path) {
  checkIfFileExists(path);
  checkIfFileIsRegular(path);

  std::ifstream input(path);
  long lines = 0;
  long comments = 0;
  long code = 0;
  long blank = 0;
  std::chrono::time_point start = std::chrono::steady_clock::now();
  while (input.good()) {
    lines++;
    std::string line;
    std::getline(input, line);
    // naive
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

    if (line.size() == 0) {
      blank++;
      continue;
    }
    if (line.starts_with("//")) {
      comments++;
      continue;
    }
    code++;
  }
  std::chrono::time_point end = std::chrono::steady_clock::now();
  std::chrono::duration<double, std::milli> duration = end - start;
  return FileData(lines, comments, code, blank, 1, false, duration);
}

FileData Parser::parseDirectory(const fs::path &path,
                                const fs::path &extension) {
  std::list<fs::path> ls;
  ls.push_back(extension);
  return parseDirectory(path, ls);
}

FileData Parser::parseDirectory(const fs::path &path,
                                const std::list<fs::path> &extensions) {
  checkIfFileExists(path);
  checkIfFileIsDirectory(path);

  startThreads();
  for (auto &p : fs::recursive_directory_iterator(path)) {
    if (std::find(extensions.begin(), extensions.end(), p.path().extension()) ==
        extensions.end())
      continue;
    addFile(p.path());
  }
  return getResult();
}