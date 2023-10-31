#pragma once
// 全局错误码列表
#include <map>
#include <sstream>
#include <string>

/**
 * Error.h
 * @brief define error code.
 */
typedef enum ErrorCodeType
{
    // 通用错误码
    ERR_SUCCESS = 0,
    ERR_GENERAL,
    ERR_UNKNOWN,
    ERR_TIMEOUT,
    ERR_NOT_IMPLEMENTED,
    ERR_NOT_SUPPORTED,
    ERR_NOT_FOUND,

    // 系统运行时错误码
    ERR_ARCH_GENERAL = 1000,
    ERR_UNINITIALIZED,
    ERR_UNAUTHORIZED,
    ERR_LOAD_RES_FAILED,
    ERR_UNLOAD_RES_FAILED,
    ERR_CREATE_INSTANCE_FAILED,
    ERR_DESTROY_INSTANCE_FAILED,
    ERR_INVALID_INSTANCE,
    ERR_CREATE_MODULE_FAILED,
    ERR_DESTROY_MODULE_FAILED,
    ERR_INVALID_MODULE,

    // 有效性检查错误码
    ERR_INVALID_GENERAL = 2000,
    ERR_INVALID_PARAM,
    ERR_INVALID_VALUE,
    ERR_INVALID_STATE,
    ERR_INVALID_SIZE,
    ERR_INVALID_POINTER,
    ERR_INVALID_MEMORY,
    ERR_INVALID_FORMAT,
    ERR_INVALID_DEVICE,
    ERR_INVALID_ADDRESS,

} ErrorCode;

class Error
{
public:
    static std::string getErrorMsg(ErrorCode errorCode, std::string fmt, ...);

private:
    static std::map<ErrorCode, const char*> ERROR_MSG;
};
