#include "ThreadPool.h"
#include "DirSize.h"
#include <iostream>
#include <atomic>
#include <mutex>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

int main() {
	// ����Ҫ������Ŀ¼·��
	string directoryPath = R"(C:\Users\Public\AppData)";

	// ��ʼ��ԭ�ӱ�������ͳ���ܴ�С���ļ�����
	uintmax_t totalSize(0);
	int fileCount(0);

	DirSize::traverseDirectory(directoryPath, totalSize, fileCount);

	// ������ս��
	cout << "\nͳ�����!" << endl;
	cout << "·��Ϊ: " << directoryPath << endl;
	cout << "���ļ���: " << fileCount << endl;
	cout << "�ܴ�С: " << totalSize << " �ֽ� (" << fixed << setprecision(4) << totalSize / (1024.0 * 1024.0) << " MB)" << endl;

	return 0;
}