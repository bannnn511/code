package ds

type WeightedDiGraph2 struct {
	matrix [][]int
}

var _ Graph = (*WeightedDiGraph2)(nil)

func NewWeightedDiGraphMatrix(n int) *WeightedDiGraph2 {
	matrix := make([][]int, n)
	for i := range matrix {
		matrix[i] = make([]int, n)
	}
	return &WeightedDiGraph2{
		matrix: matrix,
	}
}

// AddEdge implements [Graph].
func (w *WeightedDiGraph2) AddEdge(from int, to int, weight int) {
	w.matrix[from][to] = weight
}

// HasEdge implements [Graph].
func (w *WeightedDiGraph2) HasEdge(from int, to int) bool {
	return w.matrix[from][to] != 0
}

// Neighbors implements [Graph].
func (w *WeightedDiGraph2) Neighbors(v int) []Edge {
	res := []Edge{}
	for i := 0; i < len(w.matrix[v]); i++ {
		if w.matrix[v][i] != 0 {
			res = append(res, Edge{to: i, weight: w.matrix[v][i]})
		}
	}

	return res
}

// RemoveEdge implements [Graph].
func (w *WeightedDiGraph2) RemoveEdge(from int, to int) {
	for _, e := range w.matrix[from] {
		if e == to {
			w.matrix[from][to] = 0
		}
	}
}

// Size implements [Graph].
func (w *WeightedDiGraph2) Size() int {
	return len(w.matrix)
}

// Weight implements [Graph].
func (w *WeightedDiGraph2) Weight(from int, to int) int {
	return w.matrix[from][to]
}
