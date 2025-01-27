package graph

import "fmt"

func DFSFunc(graph Graph, start int, visitFn func(v int) bool) {
	marked := make([]bool, graph.V())
	stack := make([]int, 0, graph.V())

	stack = append(stack, start)
	for len(stack) > 0 {
		cur := stack[0]
		stack = stack[1:]

		for e := range graph.Adj(cur) {
			if marked[e] || visitFn(e) {
				continue
			}

			marked[e] = true
			stack = append(stack, e)
		}
	}
}

func DFS(graph Graph, start int) {
	DFSFunc(graph, start, func(v int) bool {
		fmt.Printf("%d ", v)
		return false
	})
}

func Count(graph Graph, start int) int {
	count := 0
	DFSFunc(graph, start, func(v int) bool {
		count++
		return false
	})

	return count
}
