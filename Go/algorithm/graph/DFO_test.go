package graph

import (
	"testing"
)

func TestTopologicalSort(t *testing.T) {
	dg := NewDigraph(13)
	edges := [][2]int{
		{0, 1}, {0, 5}, {0, 6},
		{},
		{2, 0}, {2, 3},
		{3, 5},
		{},
		{5, 4},
		{6, 4}, {6, 9},
		{6, 9},
		{7, 6},
		{8, 7},
		{9, 10}, {9, 11}, {9, 12},
		{},
		{11, 12},
	}

	for _, edge := range edges {
		dg.AddEdge(edge[0], edge[1])
	}

	topSort := TopologicalSort(dg)
	expected := [...]int{8, 7, 2, 3, 0, 6, 9, 10, 11, 12, 1, 5}
	for i, v := range topSort {
		if v != expected[i] {
			t.Errorf("topSort[%d]: %d, want %d", i, v, expected[i])
		}
	}
}
