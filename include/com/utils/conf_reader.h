#pragma once
/*
  Usage:

  ConfReader ini("/where/is/config.cfg");
  ini.setSection("MysqlDB");
  ini.readStr("mysqlServer", "");   // On error,"" is returned.
  ini.readInt("mysqlPort", 3306);   // On error,3306 is returned.
*/

#include <string>
#include <vector>

using namespace std;

/** INI格式的配置文件读写类\n
      该类源代码摘自互联网.
   */
class ConfReader
{
public:
    ConfReader(const string &fileName);

public:
    virtual ~ConfReader(void);

    const string &getFileName() const;

    const string &getSection() const;
    void setSection(const string &section);

    bool write(const string &key, const string &value) const;
    bool write(const string &key, int value) const;

    string readStr(const string &key, const string &default_value) const;
    int readInt(const string &key, int default_value) const;
    float readFloat(const string& key, float default_value)const;
    bool readBool(const string& key, bool default_value)const;
    std::vector<std::string> readStrArray(const string& key, std::string sep=",")const;
    std::vector<int> readIntArray(const string& key, std::string sep=",")const;
public:
    static int read_profile_string(const char *section, const char *key, char *value,
                                   int size, const char *default_value, const char *file);
    static int read_profile_int(const char *section, const char *key, int default_value,
                                const char *file);
    static int write_profile_string(const char *section, const char *key,
                                    const char *value, const char *file);

private:
    static int load_ini_file(const char *file, char *buf, int *file_size);
    static int newline(char c);
    static int end_of_string(char c);
    static int left_barce(char c);
    static int right_brace(char c);
    static int parse_file(const char *section, const char *key, const char *buf, int *sec_s, int *sec_e,
                          int *key_s, int *key_e, int *value_s, int *value_e);

private:
    string m_fileName;
    string m_section;
};
