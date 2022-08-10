#include "Parser.hh"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <mutex>
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
                   const long &no_files, const bool &is_a_directory) noexcept
    : _no_lines(no_lines), _no_commented_lines(no_commented_lines),
      _no_code_lines(no_code_lines), _no_blank_lines(no_blank_lines),
      _no_files(no_files), _is_a_directory(is_a_directory) {}

FileData::FileData(const FileData &data) noexcept
    : FileData(data.getNOLines(), data.getNOCommentedLines(),
               data.getNOCodeLines(), data.getNOBlankLines(), data.getNOFiles(),
               data.getIsADirectory()) {}

long FileData::getNOLines() const { return _no_lines; }

long FileData::getNOCommentedLines() const { return _no_commented_lines; }

long FileData::getNOCodeLines() const { return _no_code_lines; }

long FileData::getNOBlankLines() const { return _no_blank_lines; }

bool FileData::getIsADirectory() const { return _is_a_directory; }

long FileData::getNOFiles() const { return _no_files; }

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
  _is_a_directory = false;
  return *this;
}

std::ostream &operator<<(std::ostream &output, const FileData &data) {
  output << "Lines:           " << data.getNOLines() << '\n'
         << "Lines of code:   " << data.getNOCodeLines() << '\n'
         << "Commented lines: " << data.getNOCommentedLines() << '\n'
         << "Blank lines:     " << data.getNOBlankLines() << '\n'
         << "Files parsed:    " << data.getNOFiles() << '\n'
         << "Is a directory:  " << data.getIsADirectory() << '\n';
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
  temp.push_back(fs::path(".cpp"));
  temp.push_back(fs::path(".hpp"));
  temp.push_back(fs::path(".c"));
  temp.push_back(fs::path(".h"));

  if (argc <= 1) {
    _path = "./";
    _extensions = temp;
  } else

    try {
      if (argc > 1) {
        _path = fs::path(args[1]);
      }
      if (argc > 2) {
        for (int i = 2; i < argc; i++) {
          _extensions.push_back(fs::path(args[i]));
        }
      } else {
        _extensions = temp;
      }

    } catch (std::exception e) {
      std::cerr << e.what() << '\n';
      _path = "";
      _extensions.clear();
    }
}

fs::path Parser::popFile() {
  std::unique_lock lock(_files_mutex);
  fs::path p = _files.front();
  _files.pop_front();
  return p;
};
void Parser::addFile(fs::path const &path) {
  std::unique_lock lock(_files_mutex);
  _files.push_back((path));
};

bool Parser::hasFiles() const { return !_files.empty(); };

void Parser::threadFunction() {

  FileData data;
  while (getRunning()) {
    // std::unique_lock lock(_condition_mutex);
    //  std::cerr << "pop: " << std::this_thread::get_id() << '\n';
    // lock.unlock();

    while (hasFiles()) {
      fs::path path = popFile();
      data += parseFile(path);
    }
    // lock.lock();
  }

  std::unique_lock shit(_shit_mutex);
  _result += data;
};

void Parser::startThreads() {
  for (size_t i = _number_of_threads; i; i--) {
    _threads.push_back(std::thread(&Parser::threadFunction, this));
  }
}

void Parser::notifyThread() {

  _condition_mutex.unlock();
  condition_v.notify_one();
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

  if (fs::is_directory(_path))
    return parseDirectory(_path, _extensions);
  else
    return parseFile(_path);
}

FileData Parser::parseFile(const fs::path &path) {
  checkIfFileExists(path);
  checkIfFileIsRegular(path);

  std::ifstream input(path);
  long lines = 0;
  long comments = 0;
  long code = 0;
  long blank = 0;

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
  return FileData(lines, comments, code, blank, 1, false);
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
    notifyThread();
  }
  std::cerr << "end\n";
  return getResult();
}