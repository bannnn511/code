package __Graph

import (
	"github.com/bannnn511/code/go/algorithm"
)

type road struct {
	x int
	y int
}

func buildingRoads(n int, m int, roads []road) []int {
	uf := algorithm.NewUnionFind(n + 1)
	for i := range m {
		r := roads[i]
		if uf.Connected(r.x, r.y) {
			continue
		}
		uf.Union(r.x, r.y)
	}

	return uf.IDs()
}
