//
// Created by moira-q on 17.05.20.
//

#ifndef ALGOSI3_MATRIXGRAPH_H
#define ALGOSI3_MATRIXGRAPH_H

#include "IGraph.h"


class MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(size_t vertices_count):matrix(vertices_count) {
        for (int i = 0; i < vertices_count; ++i) {
            matrix[i].resize(vertices_count, false);
        }
    }

    MatrixGraph(const IGraph& graph): matrix(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); ++i) {
            matrix[i].resize(graph.VerticesCount(), false);
        }
        for (int i = 0; i < graph.VerticesCount(); ++i) {
            for (const auto& v: graph.GetNextVertices(i)) {
                AddEdge(i, v);
            }
        }
    }

    ~MatrixGraph() override = default ;
    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) override{
        matrix[from][to] = true;
    }

    virtual int VerticesCount() const override{
        return matrix.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override{
        std::vector<int> result;
        for (int i = 0; i < VerticesCount(); ++i) {
            if (matrix[vertex][i]) {
                result.push_back(i);
            }
        }

        return result;
    }
    std::vector<int> GetPrevVertices(int vertex) const  override{
        std::vector<int> result;
        for (int i = 0; i < VerticesCount(); ++i) {
            if (matrix[i][vertex]) {
                result.push_back(i);
            }
        }
        return result;
    }

private:
    std::vector<std::vector<bool>> matrix;
};

#endif //ALGOSI3_MATRIXGRAPH_H


/*
    explicit MatrixGraph(size_t vertices_count) {}

    MatrixGraph(const IGraph& graph)  {

    }

    ~MatrixGraph() override = default ;
    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
*/