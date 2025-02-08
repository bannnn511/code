package graph

type DepthFirstOrder struct {
	marked      []bool
	pre         []int
	post        []int
	reversePost []int
}

func NewDepthFirstOrder(g *Digraph) *DepthFirstOrder {
	dfo := &DepthFirstOrder{
		marked:      make([]bool, g.V()),
		pre:         make([]int, 0), // queue
		post:        make([]int, 0), // queue
		reversePost: make([]int, 0), // stack
	}
	for v := 0; v < g.V(); v++ {
		if !dfo.marked[v] {
			dfo.dfs(g, v)
		}
	}
	return dfo
}

func (dfo *DepthFirstOrder) dfs(g *Digraph, v int) {
	dfo.marked[v] = true
	dfo.pre = append(dfo.pre, v)
	for w := range g.Adj(v) {
		if !dfo.marked[w] {
			dfo.dfs(g, w)
		}
	}
	dfo.post = append(dfo.post, v)
	dfo.reversePost = append(dfo.reversePost, v)
}

func TopologicalSort(g *Digraph) []int {
	if IsCycle(g, 0) {
		return nil
	}

	dfo := NewDepthFirstOrder(g)
	stacks := dfo.reversePost
	top := make([]int, g.V())
	for len(stacks) > 0 {
		cur := stacks[len(stacks)-1]
		stacks = stacks[:len(stacks)-1]
		top = append(top, cur)
	}

	return top
}
