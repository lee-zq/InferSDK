#pragma once
/** 
 *  @file     ConfReader.cpp
 *  @brief    INI格式的配置文件读写类.
 *  @version  0.0.1
 *  @note     无
 */

#include <string>
#include <cstring>
#include <fstream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "com/conf_reader.h"


#if defined(_MSC_VER)
#pragma warning(disable:4996)
#endif


#define MAX_INI_FILE_SIZE 1024*10

/**
 *@fn                ConfReader::ConfReader(const string & fileName)
 *@brief             创建该对象
 *@param[in] fileName   文件名
 */
ConfReader::ConfReader(const string& fileName)
{
    m_fileName = fileName;
}

ConfReader::~ConfReader(void)
{
}

const string & ConfReader::getFileName() const
{
    return m_fileName;
}

/**
 *@fn                ConfReader::setSection(const string &section)
 *@brief             设置当前需要读取的配置段
 *@param[in] section 配置段字符串\n
 *                   配置段指INI格式的配置文件中的[SECNAME]
 */
void ConfReader::setSection(const string &section)
{
    m_section = section;
}

/**
 *@fn           ConfReader::write(const string &key, const string & value) const
 *@brief        写入一项新的配置项内容,值为字符串格式
 *@return       是否完成写入配置
 *@retval  true  写入成功
 *@retval  false 写入失败
 *@attention     该方法未测试
 */
bool ConfReader::write(const string &key, const string & value) const 
{
    return write_profile_string(m_section.c_str(),key.c_str(),value.c_str(),m_fileName.c_str())==1? true:false;
}

/**
 *@fn           ConfReader::write(const string &key, int value) const 
 *@brief        写入一项新的配置项内容,值为int格式
 *@return       是否完成写入配置
 *@retval  true  写入成功
 *@retval  false 写入失败
 *@attention     该方法未测试
 */
bool ConfReader::write(const string &key, int value) const 
{
    ostringstream strStream;
    strStream << value;

    string tmp = strStream.str();
    return write(key, tmp.c_str());
}

/**
 *@fn                ConfReader::readStr(const string &key,const string &default_value) const
 *@brief             通过关键字读取配置项内容
 *@param[in] key     关键字
 *@param[in] default_value     如关键字不能找到,使用的默认值
 *
 *@return            关键字对应的配置项字符串
 *@attention         配置项如跨行,或包括空格字符,则会被截断
 */
string ConfReader::readStr(const string &key,const string &default_value) const
{
    char buf[1024];
    read_profile_string(m_section.c_str(),key.c_str(),buf,sizeof(buf),default_value.c_str(),m_fileName.c_str());
    for(int i=0; i<1024; i++)
    {
        if(buf[i] == '\t' || buf[i] == ' ')
            buf[i] = '\0';

        if(buf[i] == '\0')
            break;
    }
    return buf;
}

/**
 *@fn                ConfReader::readInt(const string &key, int default_value) const
 *@brief             通过关键字读取配置项内容
 *@param[in] key     关键字
 *@param[in] default_value     如关键字不能找到,使用的默认值
 *
 *@return            关键字对应的配置项,返回十进制数字.
 */
int ConfReader::readInt(const string &key, int default_value) const
{
    return read_profile_int(m_section.c_str(),key.c_str(),default_value,m_fileName.c_str());
}


/**
 *@fn           ConfReader::getSection() const
 *@brief        返回当前设置的配置段名称
 *@return       配置段名字符串
 *@attention    为减少一次内存复制,该函数返回内部string的引用\n
 *              如返回值需长期使用,调用者必须先复制.
 */
const string& ConfReader::getSection() const
{
    return m_section;
}

int ConfReader::load_ini_file(const char *file, char *buf,int *file_size)
{
    FILE *in = NULL;
    int i=0;
    *file_size =0;

    assert(file !=NULL);
    assert(buf !=NULL);

    in = fopen(file, "r");
    if( NULL == in) {
        return 0;
    }

    buf[i]=fgetc(in);

    //load initialization file
    while( buf[i]!= (char)EOF) {
        i++;
        assert( i < MAX_INI_FILE_SIZE ); //file too big, you can redefine MAX_INI_FILE_SIZE to fit the big file 
        buf[i]=fgetc(in);
    }

    buf[i]='\0';
    *file_size = i;

    fclose(in);
    return 1;
}

int ConfReader::newline(char c)
{
    return ('\n' == c ||  '\r' == c )? 1 : 0;
}
int ConfReader::end_of_string(char c)
{
    return '\0'==c? 1 : 0;
}
int ConfReader::left_barce(char c)
{
    return '[' == c? 1 : 0;
}
int ConfReader::right_brace(char c )
{
    return ']' == c? 1 : 0;
}
int ConfReader::parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
                      int *key_s,int *key_e, int *value_s, int *value_e)
{
    const char *p = buf;
    int i=0;
    int keyLen = 0;
    int sectionLen = 0;

    assert(buf!=NULL);
    assert(section != NULL && strlen(section));
    assert(key != NULL && strlen(key));

    sectionLen = strlen(section);
    keyLen = strlen(key);

    *sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;

    while( !end_of_string(p[i]) ) {
        //find the section
        if( ( 0==i ||  newline(p[i-1]) ) && left_barce(p[i]) )
        {
            int section_start=i+1;

            //find the ']'
            do {
                i++;
            } while( !right_brace(p[i]) && !end_of_string(p[i]));

#ifdef WIN32
            if((sectionLen == i - section_start) && 0 == strnicmp(p+section_start,section, sectionLen))
#else
            if((sectionLen == i - section_start) && 0 == strncasecmp(p+section_start,section, sectionLen))
#endif
            {
                int newline_start=0;

                i++;

                //Skip over space char after ']'
                while(isspace(p[i])) {
                    i++;
                }

                //find the section
                *sec_s = section_start;
                *sec_e = i;

                while( ! (newline(p[i-1]) && left_barce(p[i])) 
                    && !end_of_string(p[i]) )
                {
                    int j=0;
                    //get a new line
                    newline_start = i;

                    while( !newline(p[i]) &&  !end_of_string(p[i]) ) {
                        i++;
                    }

                    //now i  is equal to end of the line
                    j = newline_start;
                    int valid = j;

                    if('#' != p[j]) //skip over comment
                    { 
                        while(j < i && p[j]!='=')
                        {
                            j++;

                            if(' ' != p[j] &&  '\t' != p[j] && '=' != p[j])
                                valid = j;

                            if('=' == p[j])
                            {
#ifdef WIN32
                                if((keyLen == valid - newline_start + 1) && strnicmp(key,p+newline_start, keyLen)==0)
#else
                                if((keyLen == valid - newline_start + 1) && strncasecmp(key,p+newline_start, keyLen)==0)
#endif
                                {
                                    //find the key ok
                                    *key_s = newline_start;
                                    *key_e = j-1;

                                    valid = j+1;
                                    while(' ' == p[valid] || '\t' == p[valid])
                                        valid++;
                                    *value_s = valid;
                                    *value_e = i;

                                    return 1;
                                }
                            }
                        }
                    }

                    i++;
                }
            }
        }
        else
        {
            i++;
        }
    }
    return 0;
}

/**
 *@brief               read string in initialization file\n
 *                     retrieves a string from the specified section in an initialization file
 *@param[in] section     name of the section containing the key name
 *@param[in] key         name of the key pairs to value 
 *@param[in] value       pointer to the buffer that receives the retrieved string
 *@param[in] size        size of result's buffer 
 *@param[in] default_value   default value of result
 *@param[in] file        path of the initialization file
 *@return 1 : read success; \n 0 : read fail
*/
int ConfReader::read_profile_string( const char *section, const char *key,char *value, 
                        int size, const char *default_value, const char *file)
{
    char buf[MAX_INI_FILE_SIZE]={0};
    int file_size;
    int sec_s,sec_e,key_s,key_e, value_s, value_e;

    //check parameters
    assert(section != NULL && strlen(section));
    assert(key != NULL && strlen(key));
    assert(value != NULL);
    assert(size > 0);
    assert(file !=NULL &&strlen(key));

    if(!load_ini_file(file,buf,&file_size))
    {
        if(default_value!=NULL)
        {
            strncpy(value,default_value, size);
        }
        return 0;
    }

    if(!parse_file(section,key,buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e))
    {
        if(default_value!=NULL)
        {
            strncpy(value,default_value, size);
        }
        return 0; //not find the key
    }
    else
    {
        int cpcount = value_e -value_s;

        if( size-1 < cpcount)
        {
            cpcount =  size-1;
        }

        //memset(value, 0, size);
        memcpy(value,buf+value_s, cpcount );
        value[cpcount] = '\0';

        return 1;
    }
}

/**
 *@brief            read int value in initialization file\n
 *                  retrieves int value from the specified section in an initialization file
 *@param[in] section name of the section containing the key name
 *@param[in] key     name of the key pairs to value 
 *@param[in] default_value default value of result
 *@param[in] file      path of the initialization file
 *@return profile int value,if read fail, return default value
*/
int ConfReader::read_profile_int( const char *section, const char *key,int default_value, 
                                            const char *file)
{
    char value[32] = {0};
    if(!read_profile_string(section,key,value, sizeof(value),NULL,file))
    {
        return default_value;
    }
    else
    {
        return atoi(value);
    }
}

/**
 * @brief                write a profile string to a ini file
 * @param[in] section      name of the section,can't be NULL and empty string
 * @param[in] key          name of the key pairs to value, can't be NULL and empty string
 * @param[in] value        profile string value
 * @param[in] file         path of ini file
 * @return 1 : success\n 0 : failure
*/
int ConfReader::write_profile_string(const char *section, const char *key,
                         const char *value, const char *file)
{
    char buf[MAX_INI_FILE_SIZE]={0};
    char w_buf[MAX_INI_FILE_SIZE]={0};
    int sec_s,sec_e,key_s,key_e, value_s, value_e;
    int value_len = (int)strlen(value);
    int file_size;
    FILE *out;

    //check parameters
    assert(section != NULL && strlen(section));
    assert(key != NULL && strlen(key));
    assert(value != NULL);
    assert(file !=NULL &&strlen(key));

    if(!load_ini_file(file,buf,&file_size))
    {
        sec_s = -1;
    }
    else
    {
        parse_file(section,key,buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e);
    }

    if( -1 == sec_s)
    {
        if(0==file_size)
        {
            sprintf(w_buf+file_size,"[%s]\n%s=%s\n",section,key,value);
        }
        else
        {
            //not find the section, then add the new section at end of the file
            memcpy(w_buf,buf,file_size);
            sprintf(w_buf+file_size,"\n[%s]\n%s=%s\n",section,key,value);
        }
    }
    else if(-1 == key_s)
    {
        //not find the key, then add the new key=value at end of the section
        memcpy(w_buf,buf,sec_e);
        sprintf(w_buf+sec_e,"%s=%s\n",key,value);
        sprintf(w_buf+sec_e+strlen(key)+strlen(value)+2,buf+sec_e, file_size - sec_e);
    }
    else
    {
        //update value with new value
        memcpy(w_buf,buf,value_s);
        memcpy(w_buf+value_s,value, value_len);
        memcpy(w_buf+value_s+value_len, buf+value_e, file_size - value_e);
    }

    out = fopen(file,"w");
    if(NULL == out)
    {
        return 0;
    }

    if(-1 == fputs(w_buf,out) )
    {
        fclose(out);
        return 0;
    }

    fclose(out);
    return 1;
}