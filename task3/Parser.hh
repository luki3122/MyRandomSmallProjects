#pragma once

#include <bits/types/time_t.h>
#include <chrono>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <list>
#include <map>
#include <mutex>
#include <ratio>
#include <string>
#include <thread>
#include <vector>

#define NUMBER_OF_THREADS 4

namespace fs = std::filesystem;

class ParserException : public std::exception {
private:
  std::string message;

public:
  ParserException(const std::string &message);
  virtual const char *what() const noexcept override;
};

class FileData {
private:
  std::mutex _file_data_mutex;
  long _no_lines;
  long _no_commented_lines;
  long _no_code_lines;
  long _no_blank_lines;
  bool _is_a_directory;
  long _no_files;
  std::chrono::duration<double, std::milli> _time_elapsed;

public:
  FileData();
  FileData(const long &no_lines, const long &no_commented_lines,
           const long &no_code_lines, const long &no_blank_lines,
           const long &no_files, const bool &is_a_directory,
           const auto _time_elapsed) noexcept;
  FileData(const FileData &data) noexcept;
  // virtual ~FileData();

  long getNOLines() const;
  long getNOCommentedLines() const;
  long getNOCodeLines() const;
  long getNOBlankLines() const;
  long getNOFiles() const;
  bool getIsADirectory() const;
  std::chrono::duration<double, std::milli> getTimeElapsed() const;
  std::string getFormatedTime() const;

  FileData operator+(const FileData &data);
  FileData &operator+=(const FileData &data);

  friend std::ostream &operator<<(std::ostream &output, const FileData &data);
};

class Parser {
private:
  fs::path _path;
  fs::path _log_file_path;
  std::list<fs::path> _extensions;
  std::vector<std::thread> _threads;

  std::mutex _files_mutex;
  std::list<fs::path> _files;

  std::mutex _running_mutex;
  bool _running;
  size_t _number_of_threads = NUMBER_OF_THREADS;

  // std::condition_variable condition_v;
  std::mutex _condition_mutex;

  std::mutex _result_mutex;
  FileData _result;
  // std::vector<FileData> _parsed_data;

  void parseArguments(const int argc, const char **args);
  static void checkIfFileExists(const fs::path &path);
  static void checkIfFileIsDirectory(const fs::path &path);
  static void checkIfFileIsRegular(const fs::path &path);

  fs::path popFile();
  void addFile(fs::path const &path);
  bool hasFiles() const;

  void threadFunction();
  void startThreads();
  FileData getResult();
  bool getRunning();
  void setRunning(bool running);

  FileData parseFile(const fs::path &path);
  FileData parseDirectory(const fs::path &path, const fs::path &extension);
  FileData parseDirectory(const fs::path &path,
                          const std::list<fs::path> &extensions);

public:
  Parser();
  Parser(const fs::path &path);
  Parser(const fs::path &path, const std::list<fs::path> &extensions);
  Parser(const int argc, const char **args);
  // virtual ~Parser();

  FileData parse();
  void parseToFile();
};
