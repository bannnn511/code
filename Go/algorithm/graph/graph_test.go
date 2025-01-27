package graph

import (
	"fmt"
	"testing"
)

func TestAdjacencyList(t *testing.T) {
	g := NewAdjacencyList(5)

	if g.V() != 5 {
		t.Errorf("g.V() = %d, want: %d", g.V(), 5)
	}

	if g.E() != 0 {
		t.Errorf("g.E() = %d, want: %d", g.E(), 0)
	}

	g.AddEdge(0, 1)
	g.AddEdge(0, 2)
	g.AddEdge(1, 2)
	g.AddEdge(1, 3)
	g.AddEdge(3, 4)

	if g.E() != 5 {
		t.Errorf("g.E() = %d, want: %d", g.E(), 5)
	}

	expectedAdj := map[int][]int{
		0: {2, 1},
		1: {3, 2, 0},
		2: {1, 0},
		3: {4, 1},
		4: {3},
	}

	for v, expected := range expectedAdj {
		var result []int

		for x := range g.Adj(v) {
			result = append(result, x)
		}

		if len(result) != len(expected) {
			t.Errorf("len(g.Adj(%d)) = %d, want: %d", v, len(result), len(expected))
		}

		for i, w := range result {
			if w != expected[i] {
				t.Errorf("g.Adj(%d)[%d] = %d, want: %d", v, i, w, expected[i])
			}
		}
	}

	if degree := g.Degree(0); degree != 2 {
		t.Errorf("g.Degree(0)= %d, want: %d", degree, 2)
	}

	if degree := g.MaxDegree(); degree != 3 {
		t.Errorf("g.MaxDegree()= %d, want: %d", degree, 3)
	}

	fmt.Println(g.String())
}
