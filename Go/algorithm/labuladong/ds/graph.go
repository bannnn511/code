package ds

type Edge struct {
	to     int
	weight int
}

type Graph interface {
	// add an edge (with weight)
	AddEdge(from int, to int, weight int)

	// remove an edge
	RemoveEdge(from int, to int)

	// check if two nodes are adjacent
	HasEdge(from int, to int) bool

	// return the weight of an edge
	Weight(from int, to int) int

	// return all neighboring nodes and corresponding weights of a given node
	Neighbors(v int) []Edge

	// return the total number of nodes
	Size() int
}
