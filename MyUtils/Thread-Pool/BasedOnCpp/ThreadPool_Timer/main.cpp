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
#include"thread_pool.h"
#include"utils.h"

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
// 统计目录下的文件数量和总大小
void traversal_directory(const std::string& path, uintmax_t& fileSize, size_t& fileNum) {
    atomic<uintmax_t> size_ = 0;
    atomic<size_t> num_ = 0;
    Thread_Pool pool;

    std::vector<std::filesystem::path> dir_stack;
    dir_stack.push_back(path);

    while (!dir_stack.empty()) {
        const auto current_dir = dir_stack.back();
        dir_stack.pop_back();

        try // 遍历目录
        {
            for (const auto& entry : std::filesystem::directory_iterator(
                current_dir,
                std::filesystem::directory_options::skip_permission_denied)) {
                try // 尝试访问entry
                {
                    if (std::filesystem::is_directory(entry)) {
                        dir_stack.push_back(entry.path());
                    }
                    else if (std::filesystem::is_regular_file(entry)) {
                        path_visited.push_back(entry.path().string());
                        try // 添加任务到线程池
                        {
                            // 拷贝entry，避免悬垂指针
                            pool.addTask([entry, &size_, &num_]() {
                                size_ += std::filesystem::file_size(entry);
                                num_++;
                                });
                        }
                        catch (const std::exception& e)
                        {
                            std::cerr << "添加任务到线程池失败：" << e.what() << '\n';
                        }
                    }
                }
                catch (const std::exception& e)
                {
                    std::cerr << "访问entry失败：" << e.what() << '\n';
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::filesystem::path denied_path = e.path1();
            path_denied.push_back(denied_path.string());
            std::cerr << "进入目录失败：" << e.what() << '\n';
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

int main(int argc, char const* argv[])
{
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
    std::cout << "总大小：" << fileSize << " Bytes | " << fixed << setprecision(4) << fileSize / (1024.00 * 1024.00) << " MB" << std::endl;
    std::cout << "共耗时：" << elapsed.count() << " 秒" << std::endl;

    return 0;
}
