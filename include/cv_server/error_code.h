#ifndef CV_SERVER_ERROR_CODE_H
#define CV_SERVER_ERROR_CODE_H
// 全局错误码列表
#include<map>
#include <string>
#include <sstream>

/**
 * Error.h
 * @brief define error code.
 */
typedef enum ErrorCodeType {
    // 通用错误码
    ERR_SUCCESS = 1000,
    ERR_GENERAL,
    ERR_UNKNOWN,
    ERR_TIMEOUT,
    ERR_NOT_IMPLEMENTED,
    ERR_NOT_SUPPORTED,
    ERR_NOT_FOUND,

    // 系统运行时错误码
    ERR_ARCH_GENERAL = 2000,
    ERR_UNINITIALIZED,
    ERR_UNAUTHORIZED,
    ERR_LOAD_RES_FAILED,
    ERR_UNLOAD_RES_FAILED,
    ERR_CREATE_INSTANCE_FAILED,
    ERR_DESTROY_INSTANCE_FAILED,
    ERR_INVALID_INSTANCE,

    // 有效性检查错误码
    ERR_INVALID_GENERAL = 3000,
    ERR_INVALID_PARAM,
    ERR_INVALID_VALUE,
    ERR_INVALID_STATE,
    ERR_INVALID_SIZE,
    ERR_INVALID_POINTER,
    ERR_INVALID_MEMORY,
    ERR_INVALID_FORMAT,
    ERR_INVALID_VALUE,
    ERR_INVALID_DEVICE,
    ERR_INVALID_ADDRESS,

} ErrorCode;

class Error {
public:
    static std::string getErrorMsg(ErrorCode errorCode, std::string fmt, ...);
private:
    static std::map<ErrorCode, const char *>    ERROR_MSG;
};
std::map<ErrorCode, const char *> Error::ERROR_MSG = {
    {ERR_SUCCESS, "success"},
    {ERR_GENERAL, "general error"},
    {ERR_UNKNOWN, "unknown error"},
    {ERR_TIMEOUT, "timeout"},
    {ERR_NOT_IMPLEMENTED, "not implemented"},
    {ERR_NOT_SUPPORTED, "not supported"},
    {ERR_NOT_FOUND, "not found"},

    {ERR_ARCH_GENERAL, "architecture general error"},
    {ERR_UNINITIALIZED, "uninitialized"},
    {ERR_UNAUTHORIZED, "unauthorized"},
    {ERR_LOAD_RES_FAILED, "load resource failed"},
    {ERR_UNLOAD_RES_FAILED, "unload resource failed"},
    {ERR_CREATE_INSTANCE_FAILED, "create instance failed"},
    {ERR_DESTROY_INSTANCE_FAILED, "destroy instance failed"},
    {ERR_INVALID_INSTANCE, "invalid instance"},

    {ERR_INVALID_GENERAL, "invalid general error"},
    {ERR_INVALID_PARAM, "invalid parameter"},
    {ERR_INVALID_VALUE, "invalid value"},
    {ERR_INVALID_STATE, "invalid state"},
    {ERR_INVALID_SIZE, "invalid size"},
    {ERR_INVALID_POINTER, "invalid pointer"},
    {ERR_INVALID_MEMORY, "invalid memory"},
    {ERR_INVALID_FORMAT, "invalid format"},
    {ERR_INVALID_VALUE, "invalid value"},
    {ERR_INVALID_DEVICE, "invalid device"},
    {ERR_INVALID_ADDRESS, "invalid address"},
};

std::string Error::getErrorMsg(ErrorCode errorCode, std::string fmt, ...) {
    std::stringstream ss;
    ss << "Code: ", static_cast<long>(errorCode);
    ss << " Msg: " << ERROR_MSG[errorCode];
    ss << " \"" << fmt << "\"";
    std::string errorCodeStr = ss.str();
    return errorCodeStr;
}

#endif //CV_SERVER_ERROR_CODE_H
