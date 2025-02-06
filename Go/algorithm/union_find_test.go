package algorithm

import (
	"fmt"
	"testing"
)

func TestUnionFind(t *testing.T) {
	uf := NewUnionFind(10)

	type pair struct {
		p, q int
	}
	tests := []pair{
		{4, 3},
		{3, 8},
		{6, 5},
		{9, 4},
		{2, 1},
		{5, 0},
		{7, 2},
		{6, 1},
	}

	for _, test := range tests {
		if uf.Connected(test.p, test.q) {
			continue
		}
		uf.Union(test.p, test.q)
	}

	if uf.Count() != 2 {
		t.Errorf("Expected 2, got %d", uf.Count())
	}

	ids := uf.IDs()
	fmt.Println(ids)
}
