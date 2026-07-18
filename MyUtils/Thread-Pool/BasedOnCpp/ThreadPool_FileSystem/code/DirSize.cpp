#include "DirSize.h"
#include <iostream>
#include "ThreadPool.h"

std::vector<std::string> path_visited;
std::vector<std::string> path_denied;
void DirSize::traverseDirectory_incomplete(const std::string& directoryPath, uintmax_t& fileSize, size_t& fileCount) {
	ThreadPool pool;	// 创建线程池对象
	atomic<uintmax_t> totalSize = 0;
	atomic<size_t> totalCount = 0;

	try {
		for (const auto& entry : fs::recursive_directory_iterator(
			directoryPath,
			std::filesystem::directory_options::skip_permission_denied)) {
			if (entry.is_regular_file()) {
				path_visited.push_back(entry.path().string());
				pool.addTask([entry, &totalSize, &totalCount]() {
					try {
						// 原子增加总大小和文件计数
						totalSize += fs::file_size(entry);
						++totalCount;
					}
					catch (const fs::filesystem_error& e) {
						std::cerr << "无法获取文件大小: " << e.what() << '\n';
					}
					});
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		std::filesystem::path denied_path = e.path1();
		path_denied.push_back(denied_path.string());
		std::cerr << "DirSize Error: " << e.what() << '\n';
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << '\n';
	}

	// 等待所有任务完成
	while (pool.getActiveThreadNum() > 0) {
		this_thread::sleep_for(chrono::milliseconds(500));
	}

	// 将值传给传出变量
	fileSize = totalSize;
	fileCount = totalCount;
}

void DirSize::traverseDirectory(const std::string& directoryPath, uintmax_t& fileSize, size_t& fileCount) {
	ThreadPool pool;
	atomic<uintmax_t> size_ = 0;
	atomic<size_t> num_ = 0;

	vector<std::filesystem::path> dir_stack;
	dir_stack.push_back(directoryPath);

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
	fileCount = num_;
}
