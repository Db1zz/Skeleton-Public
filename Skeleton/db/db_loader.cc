#include "db_loader.h"
#include <vector>
#include <iostream>

namespace db {

CsvDatabase::CsvDatabase(const string_view db_path)
    : db_path_(db_path) {}

bool CsvDatabase::Connect() {
  if (db_path_.empty()) {
    return false;
  }

  read_stream_.open(db_path_, std::ios::in);

  // TODO: the write stream deletes all data in csv file
  // we have to fix it when the write stream will be needed.
  // write_stream_.open(db_path_);
  // read_stream_.good() && write_stream_.good()

  if (read_stream_.good()) {
    return true;
  } else {
    Close();
    std::cout << "Cannot connect to the Csv Database \n";
    return false;
  }
}

void CsvDatabase::Close() {
    read_stream_.close();
    write_stream_.close();
}

string CsvDatabase::Read() {
  if (!read_stream_.is_open()) {
    return "";
  }

  string buffer = {""};
  std::getline(read_stream_, buffer);

  return buffer;
}

bool CsvDatabase::Write(const string_view data_to_write) {
  if (!write_stream_.is_open()) {
    return false;
  }

  write_stream_ << data_to_write;
  if(!write_stream_.good()) {
    return false;
  }

  return true;
}

void CsvDatabase::SetDbPath(const string_view db_path) {
  if (db_path == "") {
    return;
  }
  db_path_ = db_path;
}

CsvDatabaseParser::CsvDatabaseParser(const string_view database_path) 
    : CsvDatabase(database_path) {}

vector<string> CsvDatabaseParser::SplitLine(const string *line) {
  vector<string> result;
  std::stringstream input(*line);
  string cell;

  while (std::getline(input, cell, ',')) {
    result.push_back(cell);
  }

  return result;
}

vector<vector<string>> CsvDatabaseParser::ParseCsv() {
  if (!read_stream_.good()) {
    if (!Connect()) std::exit(-1);
  }

  vector<vector<string>> result;
  result.reserve(110);
  string line = "";

  while (std::getline(read_stream_, line)) {
    if (line[0] == '#') {
      continue;
    }

    vector<string> data = SplitLine(&line);
    result.push_back(data);
  }

  return result;
}

} // namespace db