package algorithm

type UnionFind struct {
	id    []int
	rank  []int // Added for union by rank
	count int
}

// NewUnionFind creates a new UnionFind data structure with n elements
func NewUnionFind(n int) *UnionFind {
	if n < 0 {
		panic("size must be non-negative")
	}

	id := make([]int, n)
	rank := make([]int, n)
	for i := range id {
		id[i] = i
	}

	return &UnionFind{
		id:    id,
		rank:  rank,
		count: n,
	}
}

// Find returns the component identifier for element p
func (uf *UnionFind) Find(p int) int {
	return uf.id[p]
}

// Union connects elements p and q
func (uf *UnionFind) Union(p, q int) {
	rootP := uf.Find(p)
	rootQ := uf.Find(q)

	if rootP == rootQ {
		return
	}

	// Union by rank
	if uf.rank[rootP] < uf.rank[rootQ] {
		uf.id[rootP] = rootQ
	} else if uf.rank[rootP] > uf.rank[rootQ] {
		uf.id[rootQ] = rootP
	} else {
		uf.id[rootQ] = rootP
		uf.rank[rootP]++
	}

	uf.count--
}

// Count returns the number of components
func (uf *UnionFind) Count() int {
	return uf.count
}

// Connected returns true if elements p and q are in the same component
func (uf *UnionFind) Connected(p, q int) bool {
	return uf.Find(p) == uf.Find(q)
}

// IDs returns the distinct component identifiers
func (uf *UnionFind) IDs() []int {
	ids := make([]int, 0, uf.count)
	seen := make(map[int]bool)

	for i := range uf.id {
		root := uf.Find(i)
		if !seen[root] {
			seen[root] = true
			ids = append(ids, root)
		}
	}
	return ids
}
