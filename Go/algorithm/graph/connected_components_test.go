package graph

import (
	"testing"
)

func TestConnectedComponents(t *testing.T) {
	g := NewUndirected(13)

	edges := [][2]int{
		{0, 6}, {0, 2}, {0, 1}, {0, 5},
		{1, 0},
		{2, 0},
		{3, 5}, {3, 4},
		{4, 5}, {4, 6}, {4, 3},
		{5, 3}, {5, 4}, {5, 0},
		{6, 0}, {6, 4},
		{7, 8},
		{8, 7},
		{9, 11}, {9, 10}, {9, 12},
		{10, 9},
		{11, 9}, {11, 12},
		{12, 11}, {12, 9},
	}

	for _, edge := range edges {
		g.AddEdge(edge[0], edge[1])
	}

	count, cc := ConnectedComponents(g)
	if count != 3 {
		t.Errorf("count: %d, want %d", count, 3)
	}

	if cc[0] != 0 || cc[1] != 0 {
		t.Errorf("cc[0]: %d, cc[1]: %d, want 0", cc[0], cc[1])
	}

	if cc[7] != 1 || cc[8] != 1 {
		t.Errorf("cc[7]: %d, cc[8]: %d, want 1", cc[7], cc[8])
	}

	if cc[9] != 2 || cc[10] != 2 || cc[11] != 2 || cc[12] != 2 {
		t.Errorf("cc[9]: %d, cc[10]: %d, cc[11]: %d, cc[12]: %d, want 2", cc[9], cc[10], cc[11], cc[12])

	}
}
