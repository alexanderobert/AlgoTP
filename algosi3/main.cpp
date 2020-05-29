#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

class ListGraph{
public:
    explicit ListGraph(size_t vertices_count) : vertices(vertices_count) {
    }

    // Добавление ребра от from к to.
    void AddEdge(int from, int to, int w){
        vertices[from].push_back({to, w});
    }

    int VerticesCount() const {
        return vertices.size();
    }

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const  {
        std::vector<std::pair<int, int>> result;

        result.resize(vertices[vertex].size());
        std::copy(vertices[vertex].begin(), vertices[vertex].end(), result.begin());

        return result;
    }

private:
    std::vector<std::vector<std::pair<int, int>>> vertices;
};

int64_t  bfs_min_dist(const ListGraph& graph, int from, int to) {
    std::vector<int64_t > points(graph.VerticesCount(), INT64_MAX);
    std::multimap<int, int> q; // расстояние/значение
    points[from] = 0;
    q.insert({points[from], from});
    while (!q.empty()) {
        int v = q.begin()->second;
        q.erase(q.begin());
        std::vector<std::pair<int, int>> next_vertices = graph.GetNextVertices(v);
        for (std::pair<int, int> child : next_vertices) {
            if (points[child.first] == INT64_MAX) {
                points[child.first] = points[v] + child.second;
                q.insert({points[child.first] ,child.first});
            } else if (points[child.first] > (points[v] + child.second)) {
                auto range = q.equal_range(points[child.first]);
                for (auto it = range.first; it != range.second; ++it) {
                    if (it->second == child.first) {
                        points[child.first] = points[v] + child.second;
                        q.erase(it);
                        q.insert({points[child.first], child.first});
                        break;
                    }
                }
            }
        }
    }
    return points[to];
}

int main() {
    int a, b;
    int v;
    int f, to;
    cin >> a >> b >> v >> f >> to;
    ListGraph graph(v);
    for (int i = 0; i < v; ++i) {
        graph.AddEdge(i, ((i + 1) % v), a);
        graph.AddEdge(i, ((i * i + 1) % v), b);
    }
    cout << bfs_min_dist(graph, f, to);
}
