package graph

func dfs(graph IGraph, v int, marked []bool, ids []int, count int) {
	marked[v] = true
	ids[v] = count
	for e := range graph.Adj(v) {
		if marked[e] {
			continue
		}
		dfs(graph, e, marked, ids, count)
	}
}

func ConnectedComponents(graph IGraph) (int, []int) {
	marked := make([]bool, graph.V())
	ids := make([]int, graph.V())
	count := 0
	for v := 0; v < graph.V(); v++ {
		if marked[v] {
			continue
		}
		dfs(graph, v, marked, ids, count)
		count++
	}

	return count, ids
}
