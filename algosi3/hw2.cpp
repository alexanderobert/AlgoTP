#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

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

ListGraph cin_graph() {
    int v = 0, r = 0;
    std::cin >> v >> r;
    ListGraph graph(v);
    for (int i = 0; i < r; ++i) {
        int from = 0, to = 0, w = 0;
        std::cin >> from >> to >> w;
        graph.AddEdge(from, to, w);
        graph.AddEdge(to, from, w);
    }
    return graph;
}

int main(int argc, const char * argv[]) {

    ListGraph graph = cin_graph();
    int v = 0, w = 0;

    std::cin >> v >> w;
    std::cout << bfs_min_dist(graph, v, w);

    return 0;
}