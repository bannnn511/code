package ds

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestWeightedDiGraphMatrix(t *testing.T) {
	graph := NewWeightedDiGraphMatrix(3)
	graph.AddEdge(0, 1, 1)
	graph.AddEdge(1, 2, 2)
	graph.AddEdge(2, 0, 3)
	graph.AddEdge(2, 1, 4)

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
