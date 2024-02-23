#ifndef DB_LOADER_H_
#define DB_LOADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace db {

using std::string_view;
using std::string;
using std::vector;

class Database {
  public:
    virtual bool Connect() = 0;
    virtual string Read() = 0;
    virtual bool Write(const string_view data) = 0;
    virtual void Close() = 0;
};

class CsvDatabase : public Database {
  public:
    CsvDatabase() = default;

    CsvDatabase(const string_view relative_db_path);

    bool Connect() override;

    void Close() override;

    string Read() override;

    bool Write(const string_view data) override;

    void SetDbPath(const string_view relative_db_path);

  protected:
    std::filesystem::path db_path_;
    std::ifstream read_stream_;
    std::ofstream write_stream_;
};

class CsvDatabaseParser : public CsvDatabase {
  public:
    CsvDatabaseParser() = default;
    CsvDatabaseParser(const string_view database_path);

    vector<string> SplitLine(const string *line);  
    vector<vector<string>> ParseCsv();
};

} // namespace db

#endif // DB_LOADER_H_