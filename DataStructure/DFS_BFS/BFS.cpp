/**
 * 输入起点终点,根据BFS查询所有可行路径
 */

#include "head.h"
#include <queue>

// 全局变量
vector<string> paths;
NodeIndex start = NodeIndex::A;
NodeIndex target = NodeIndex::G;

// 递归方式
void BFS(string path, vector<bool> visited, queue<NodeIndex> rows) {
    while (!rows.empty()) {
        NodeIndex node = rows.front();
        rows.pop();

        if (node == target) {
            path += char('A' + node);
            // visited[node] = true;
            paths.push_back(path);
        }
        else {
            queue<NodeIndex> newRows;
            string tmpPath = path;
            for (int i = 0; i < NODE_NUM; ++i) {
                if (global_map[node][i] != 0 && !visited[i]) {
                    newRows.push((NodeIndex)i);
                }
            }
            tmpPath += char('A' + node);
            visited[node] = true;
            BFS(tmpPath, visited, newRows);
        }
    }
}

int main(int argc, char const* argv[])
{
    string path;
    vector<bool> visited(NODE_NUM, false);
    queue<NodeIndex> rows;
    for (int i = 0; i < NODE_NUM; ++i) {
        if (global_map[start][i] != 0) {
            rows.push((NodeIndex)i);
        }
    }
    path = char('A' + start);
    visited[start] = true;

    BFS(path, visited, rows);

    print_vector(paths);

    return 0;
}
