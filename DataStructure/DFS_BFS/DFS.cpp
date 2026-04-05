/**
 * 输入起点终点,通过DFS查询所有可行路径
 */

#include "head.h"
#include <stack>
#include <string>
#include <algorithm>

#if 0   // 迭代方式(待完善)
vector<string> DFS(const int global_map[][NODE_NUM], NodeIndex startNode, NodeIndex targetNode) {
    vector<string> paths;
    stack<int> st;
    vector<bool> visited(NODE_NUM, false);

    st.push(startNode);
    visited[startNode] = true;
    while (!st.empty()) {
        int row = st.top();
        for (int i = 0; i < NODE_NUM; ++i) {
            if (global_map[row][i] != 0 && !visited[i]) {   // 如果i不在map中,会在map中插入这个key并将value设置为false
                // 当达到目标节点时
                if (i == targetNode) {
                    stack<int> tmp;
                    string path;
                    path += char('A' + i);

                    // 记录当前路径
                    while (!st.empty()) {
                        int index = st.top(); st.pop();
                        path += char('A' + index);
                        tmp.push(index);
                    }
                    reverse(path.begin(), path.end());
                    paths.push_back(path);

                    // 重新装填栈
                    while (!tmp.empty()) {
                        st.push(tmp.top());
                        tmp.pop();
                    }
                }
                else {
                    st.push(i);
                    visited[i] = true;
                }
            }

            // 弹出上个节点
            if (i == NODE_NUM - 1) {
                st.pop();
                visited[row] = false;
            }
        }
    }

    return paths;
}
#endif

// 全局变量
vector<string> paths;
NodeIndex start = NodeIndex::A;
NodeIndex target = NodeIndex::G;

// 递归方式
void DFS(string path, vector<bool> visited, NodeIndex row) {
    path += char('A' + row);
    visited[row] = true;

    if (row == target) {
        paths.push_back(path);
        return;
    }

    for (int i = 0; i < NODE_NUM; ++i) {
        if (global_map[row][i] != 0 && !visited[i]) {
            DFS(path, visited, (NodeIndex)i);
        }
    }
}

int main(int argc, char const* argv[])
{
    vector<bool> visited(NODE_NUM, false);
    string path;

    DFS(path, visited, start);

    print_vector(paths);

    return 0;
}
