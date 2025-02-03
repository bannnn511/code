package graph

func IsCycle(g IGraph, start int) bool {
	stack := []int{start}
	visited := make([]bool, g.V())
	parent := make([]int, g.V())
	for i := range parent {
		parent[i] = -1
	}

	for len(stack) > 0 {
		cur := stack[len(stack)-1]
		stack = stack[:len(stack)-1]

		visited[cur] = true
		for e := range g.Adj(cur) {
			if !visited[e] {
				parent[e] = cur
				stack = append(stack, e)
			} else if parent[cur] != e {
				return true
			}
		}
	}

	return false
}
