#include "DirSize.h"
#include <iostream>
#include "ThreadPool.h"

void DirSize::traverseDirectory(const std::string& directoryPath, uintmax_t& fileSize, int& fileCount) {
	ThreadPool pool;	// 创建线程池对象
	atomic<uintmax_t> totalSize;
	atomic<int> totalCount;

	try {
		for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
			if (entry.is_regular_file()) {
				fs::path filePath = entry.path();
				pool.addTask([filePath, &totalSize, &totalCount]() {
					try {
						// 原子增加总大小和文件计数
						totalSize += fs::file_size(filePath);
						++totalCount;
					}
					catch (const fs::filesystem_error& e) {
						std::cerr << "无法获取文件大小: " << filePath << " - " << e.what() << std::endl;
					}
					});
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "DirSize Error: " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	// 等待所有任务完成
	while (pool.getActiveThreadNum() > 0) {
		this_thread::sleep_for(chrono::milliseconds(500));
	}

	// 将值传给传出变量
	fileSize = totalSize;
	fileCount = totalCount;
}
