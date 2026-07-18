/*===============================================
*   文件名称：main.cpp
*   创 建 者：Autumnker
*   创建日期：2026年07月04日
*   描    述：
================================================*/
#include<chrono>
#include<random>
#include<iostream>
#include<string>
#include<signal.h>
#include<fstream>
#include<filesystem>
#include<atomic>
#include"thread_pool.h"
#include"utils.h"

// 定义中断信号处理函数
bool stop = false;
void stopHandler(int singal) {
    if (singal == SIGINT) {
        stop = true;
    }
    {
        lock_guard<mutex> locker(mutex_io);
        cout << "\nStopping..." << endl;
    }
}

vector<std::string> path_visited;
vector<std::string> path_denied;
// 遍历目录并统计文件数量和大小（会出现错误：
// “遍历中断：filesystem error: cannot increment recursive directory iterator: No such file or directory”）
void traversal_directory_incomplete(const std::string& path, uintmax_t& fileSize, size_t& fileNum) {
    Thread_Pool pool;
    std::atomic<uintmax_t> size_ = 0;
    std::atomic<size_t> num_ = 0;

    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(
            path,
            std::filesystem::directory_options::skip_permission_denied)) {
            try
            {
                // enrty按值捕获，需要在多线程中保持专属副本，否则很可能悬垂引用
                pool.addTask([entry, &size_, &num_]() {
                    if (std::filesystem::is_regular_file(entry)) {
                        size_ += std::filesystem::file_size(entry);
                        num_++;
                    }
                    });
            }
            catch (const std::exception& e)
            {
                std::cerr << "addTask中断：" << e.what() << '\n';
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "遍历中断：" << e.what() << '\n';
    }

    pool.waitAll();
    fileSize = size_;
    fileNum = num_;
}

// 遍历目录并统计文件数量和大小（用迭代方式模拟递归）
void traversal_directory(const std::string& path, uintmax_t& fileSize, size_t& fileNum) {
    Thread_Pool pool;
    std::atomic<uintmax_t> size_ = 0;
    std::atomic<size_t> num_ = 0;

    // 目录列表
    std::vector<std::filesystem::path> dir_stack;
    dir_stack.push_back(path);

    // 遍历
    while (!dir_stack.empty()) {
        auto current_dir = dir_stack.back();
        dir_stack.pop_back();

        try // 遍历current_dir
        {
            for (const auto& entry : std::filesystem::directory_iterator(
                current_dir,
                std::filesystem::directory_options::skip_permission_denied)) {
                try // 对单个目录进行操作
                {
                    if (std::filesystem::is_directory(entry)) {
                        dir_stack.push_back(entry);
                    }
                    else if (std::filesystem::is_regular_file(entry)) {
                        path_visited.push_back(entry.path().string());
                        // if (entry.path().filename().string()[0] == '.') {
                        //     std::cout << "[Skipping file] " << entry.path().filename() << std::endl;
                        //     continue;
                        // }
                        try // 添加进线程池
                        {
                            // 拷贝entry，避免悬垂引用
                            pool.addTask([entry, &size_, &num_]() {
                                size_ += std::filesystem::file_size(entry);
                                num_++;
                                // std::this_thread::sleep_for(std::chrono::microseconds(size_ % 1100));
                                });
                        }
                        catch (const std::exception& e)
                        {
                            std::cerr << "未能将文件添加进线程池：" << e.what() << '\n';
                        }
                    }
                }
                catch (const std::exception& e)
                {
                    std::cerr << "跳过目录：" << e.what() << '\n';
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::filesystem::path denied_path = e.path1();
            path_denied.push_back(denied_path.string());
            std::cerr << "无法访问目录：" << e.what() << '\n';
        }
        catch (const std::exception& e)
        {
            std::cerr << "未知错误：" << e.what() << '\n';
        }
    }

    pool.waitAll();
    fileSize = size_;
    fileNum = num_;
}

int main(int argc, char const* argv[]) {
    // 注册中断信号
    signal(SIGINT, stopHandler);

    // 待统计目录
    std::string filePath{};

    if (argc == 2) {
        std::filesystem::path path = std::filesystem::absolute(argv[1]);
        filePath.assign(path.string());
    }
    else {
        filePath = R"(C:\Windows\WinSxS)";
    }

    // 遍历文件
    uintmax_t fileSize = 0;
    size_t fileNum = 0;
    std::chrono::time_point time_start = std::chrono::system_clock::now();
    traversal_directory(filePath, fileSize, fileNum);
    std::chrono::time_point time_finish = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = time_finish - time_start;

    // 输出日志
    std::ofstream ofs("./log.md", std::ios::trunc);
    ofs << "[visited]" << '\n';
    for (const auto& path : path_visited) { ofs << path << '\n'; }
    ofs << "[denied]" << '\n';
    for (const auto& path : path_denied) { ofs << path << '\n'; }
    ofs.close();

    std::cout << "目录：" << filePath << std::endl;
    std::cout << "文件数量：" << fileNum << std::endl;
    std::cout << "总大小：" << fileSize << " Bytes | " << fixed << setprecision(4) << fileSize / (1024.0 * 1024.0) << " MB" << std::endl;
    std::cout << "共耗时：" << elapsed.count() << " 秒" << std::endl;

    return 0;
}
