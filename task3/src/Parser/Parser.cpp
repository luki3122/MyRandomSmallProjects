#include "Parser.hh"
#include "AsyncUtils.hh"
#include "ParserException.hh"
#include "ParserOptions.hh"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

void Parser::checkIfFileExists(const fs::path &path) {
  if (fs::exists(path))
    return;
  throw ParserException(
      std::string("File does not exist: ").append(path.string()));
}
void Parser::checkIfFileIsDirectory(const fs::path &path) {
  if (fs::is_directory(path))
    return;
  throw ParserException(
      std::string("File is not a directory: ").append(path.string()));
}

void Parser::checkIfFileIsRegular(const fs::path &path) {
  if (fs::is_regular_file(path))
    return;
  throw ParserException(
      std::string("File is not regular file: ").append(path.string()));
}

bool Parser::fileShouldBeParsed(const fs::path &path) {
  return fs::is_regular_file(path) &&
         std::find(getOptions().getExtensions().begin(),
                   getOptions().getExtensions().end(),
                   path.extension()) != getOptions().getExtensions().end();
}

Parser::Parser(ParserOptions &options) : _options(options) {}

// Parser::Parser(Parser &&parser) {}

Parser::~Parser() {}

const ParserOptions &Parser::getOptions() const { return _options; }

void Parser::setOptions(const ParserOptions &options) { _options = options; }

IParsedData *Parser::parse(const fs::path &path) {
  checkIfFileExists(path);
  FileData result;
  if (fs::is_directory(path))
    return new ParsedDirectoryData(parseDirectory(path));
  else
    return new ParsedFileData(parseFile(path));
}

void getAndFormatLine(std::ifstream &input, std::string &line) {

  std::getline(input, line);
  line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
}

ParsedFileData Parser::parseFile(const fs::path &path) {
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
    getAndFormatLine(input, line);

    if (line.size() == 0) {
      blank++;
      continue;
    }
    if (line.starts_with("//")) {
      comments++;
      continue;
    }
    if (line.starts_with("/*")) {
      comments++;
      while (input.good() && line.find("*/") == -1) {
        getAndFormatLine(input, line);
      }
      comments++;
      continue;
    }
    code++;
  }
  std::chrono::time_point end = std::chrono::steady_clock::now();
  std::chrono::duration<double, std::milli> duration = end - start;
  ParsedFileData temp;
  temp.setFileName(path.filename());
  temp.setNOLines(lines);
  temp.setNOCodeLines(code);
  temp.setNOBlankLines(blank);
  temp.setNOCommentedLines(comments);
  temp.setParsingDuration(duration);
  return temp;
}

ParsedDirectoryData Parser::parseDirectory(const fs::path &path) {
  checkIfFileExists(path);
  checkIfFileIsDirectory(path);
  ParsedDirectoryData temp;

  std::chrono::time_point start = std::chrono::steady_clock::now();
  for (auto &p : fs::recursive_directory_iterator(path)) {
    if (fileShouldBeParsed(p.path()))
      temp.addFileData(parseFile(p.path()));
    continue;
  }

  std::chrono::time_point end = std::chrono::steady_clock::now();
  std::chrono::duration<double, std::milli> duration = end - start;

  temp.setDirName(path.filename());
  temp.setParsingDuration(duration);
  return temp;
}

ParsedDirectoryData Parser::parseDirectoryAsync(const fs::path &path) {
  checkIfFileExists(path);
  checkIfFileIsDirectory(path);
  std::chrono::time_point start = std::chrono::steady_clock::now();

  ThreadController<fs::path, ParsedFileData> th_controller(
      [&](const fs::path &path) -> ParsedFileData { return parseFile(path); },
      NUMBER_OF_THREADS);

  th_controller.start();

  for (auto &p : fs::recursive_directory_iterator(path)) {
    if (fileShouldBeParsed(p.path()))
      th_controller.addData(p.path());
    continue;
  }

  ParsedDirectoryData temp;

  temp.setFileDataList(th_controller.join());
  temp.setDirName(path.filename());

  std::chrono::time_point end = std::chrono::steady_clock::now();
  std::chrono::duration<double, std::milli> duration = end - start;
  temp.setParsingDuration(duration);
  return temp;
}
