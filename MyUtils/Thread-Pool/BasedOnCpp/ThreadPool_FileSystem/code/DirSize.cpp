#include "DirSize.h"
#include <iostream>
#include "ThreadPool.h"

void DirSize::traverseDirectory(const std::string& directoryPath, uintmax_t& fileSize, int& fileCount) {
	ThreadPool pool;	// �����̳߳ض���
	atomic<uintmax_t> totalSize;
	atomic<int> totalCount;

	try {
		for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
			if (entry.is_regular_file()) {
				fs::path filePath = entry.path();
				pool.addTask([filePath, &totalSize, &totalCount]() {
					try {
						// ԭ�������ܴ�С���ļ�����
						totalSize += fs::file_size(filePath);
						++totalCount;
					}
					catch (const fs::filesystem_error& e) {
						std::cerr << "�޷���ȡ�ļ���С: " << filePath << " - " << e.what() << std::endl;
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

	// �ȴ������������
	while (pool.getActiveThreadNum() > 0) {
		this_thread::sleep_for(chrono::milliseconds(500));
	}

	// ��ֵ������������
	fileSize = totalSize;
	fileCount = totalCount;
}
