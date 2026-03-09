package ds

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestWeightedDiGraph(t *testing.T) {
	graph := NewWeightedDiGraph(3)
	graph.AddEdge(0, 1, 1)
	graph.AddEdge(1, 2, 2)
	graph.AddEdge(2, 0, 3)
	graph.AddEdge(2, 1, 4)

	visited := make([]bool, graph.Size())
	traverse(graph, 0, visited)
	trarversePaths(graph, 0, 2)

	visitedEdges := make([][]bool, graph.Size())
	for i := range visitedEdges {
		visitedEdges[i] = make([]bool, graph.Size())
	}
	traverseEdges(graph, 0, visitedEdges)

	fmt.Printf("BFS\n")
	bfs(graph, 0)

	assert.Equal(t, graph.HasEdge(0, 1), true)
	assert.Equal(t, graph.HasEdge(1, 0), false)

	for i, edge := range graph.Neighbors(2) {
		if i == 0 {
			assert.Equal(t, edge.to, 0)
			assert.Equal(t, edge.weight, 3)
		}
		if i == 1 {
			assert.Equal(t, edge.to, 1)
			assert.Equal(t, edge.weight, 4)
		}
	}

	graph.RemoveEdge(0, 1)
	assert.Equal(t, graph.HasEdge(0, 1), false)
}
