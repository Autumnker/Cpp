#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

class Logger
{
public:
    // 删除拷贝、移动，使Logger变为单例模式
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    // 获取唯一实例
    static Logger& instance()
    {
        static Logger s_instance;
        return s_instance;
    }

    void info(const char* file = "", const char* function = "", int line = 0, const char* msg = "");
    void warn(const char* file = "", const char* function = "", int line = 0, const char* msg = "");
    void fail(const char* file = "", const char* function = "", int line = 0, const char* msg = "");
private:
    Logger() = default;
};

#endif // __LOGGER_HPP__
