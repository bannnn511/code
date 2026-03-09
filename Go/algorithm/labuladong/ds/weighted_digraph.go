package ds

import "fmt"

type WeightedDiGraph struct {
	graph [][]Edge
}

var _ Graph = (*WeightedDiGraph)(nil)

func NewWeightedDiGraph(n int) *WeightedDiGraph {
	return &WeightedDiGraph{
		graph: make([][]Edge, n),
	}
}

// AddEdge implements [Graph].
func (w *WeightedDiGraph) AddEdge(from int, to int, weight int) {
	w.graph[from] = append(w.graph[from], Edge{to: to, weight: weight})
}

// HasEdge implements [Graph].
func (w *WeightedDiGraph) HasEdge(from int, to int) bool {
	for _, e := range w.graph[from] {
		if e.to == to {
			return true
		}
	}

	return false
}

// Neighbors implements [Graph].
func (w *WeightedDiGraph) Neighbors(v int) []Edge {
	return w.graph[v]
}

// RemoveEdge implements [Graph].
func (w *WeightedDiGraph) RemoveEdge(from int, to int) {
	for i, e := range w.graph[from] {
		if e.to == to {
			w.graph[from] = append(w.graph[from][:i], w.graph[from][i+1:]...)
		}
	}
}

// Size implements [Graph].
func (w *WeightedDiGraph) Size() int {
	return len(w.graph)
}

// Weight implements [Graph].
func (w *WeightedDiGraph) Weight(from int, to int) int {
	for _, e := range w.graph[from] {
		if e.to == to {
			return e.weight
		}
	}

	return -1
}

func traverse(graph Graph, s int, visited []bool) {
	if s < 0 || s >= graph.Size() {
		return
	}

	if visited[s] {
		return
	}

	visited[s] = true
	fmt.Println("visit", s)
	for _, e := range graph.Neighbors(s) {
		traverse(graph, e.to, visited)
	}
}

func traverseEdges(g Graph, s int, visited [][]bool) {
	if s < 0 || s >= g.Size() {
		return
	}

	for _, e := range g.Neighbors(s) {
		if visited[s][e.to] {
			continue
		}

		visited[s][e.to] = true
		fmt.Printf("visit edge: %d -> %d\n", s, e.to)
		traverseEdges(g, e.to, visited)
	}
}

func trarversePaths(g Graph, src, dest int) {
	if src < 0 || src > g.Size() {
		return
	}

	onPath := make([]bool, g.Size())
	paths := []int{}

	var traversePath func(g Graph, src, dest int)
	traversePath = func(g Graph, src, dest int) {
		if src < 0 || src > g.Size() {
			return
		}

		if onPath[src] {
			return
		}

		if src == dest {
			for _, path := range paths {
				fmt.Printf("%d->", path)
			}
			fmt.Printf("%d\n", src)
			return
		}

		onPath[src] = true
		paths = append(paths, src)
		for _, e := range g.Neighbors(src) {
			traversePath(g, e.to, dest)
		}
		paths = paths[:len(paths)-1]
		onPath[src] = false
	}

	traversePath(g, src, dest)
}

func bfs(g Graph, s int) {
	visited := make([]bool, g.Size())
	q := []int{s}
	visited[s] = true

	depth := 0
	for len(q) > 0 {
		size := len(q)
		levels := []int{}
		for i := 0; i < size; i++ {
			for _, e := range g.Neighbors(q[i]) {
				if visited[e.to] {
					continue
				}
				visited[e.to] = true
				q = append(q, e.to)
				levels = append(levels, e.to)
			}
		}

		q = q[size:]
		depth++
	}
}
