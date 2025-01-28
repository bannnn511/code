package graph

import "fmt"

func DFSFunc(graph Graph, start int, visitFn func(v int) bool) {
	marked := make([]bool, graph.V())
	marked[start] = true
	stack := make([]int, 0, graph.V())

	stack = append(stack, start)
	for len(stack) > 0 {
		cur := stack[len(stack)-1]
		stack = stack[:len(stack)-1]

		if visitFn(cur) {
			continue
		}

		for e := range graph.Adj(cur) {
			if marked[e] {
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
	fmt.Println()
}

func Count(graph Graph, start int) int {
	count := 0
	DFSFunc(graph, start, func(v int) bool {
		count++
		return false
	})
	fmt.Println()

	return count
}

func BFSFunc(g Graph, start int, visitFn func(v int) bool) {
	marked := make([]bool, g.V())
	marked[start] = true
	queue := make([]int, 0, g.V())

	queue = append(queue, start)
	for len(queue) > 0 {
		cur := queue[0]
		queue = queue[1:]

		if visitFn(cur) {
			continue
		}

		for e := range g.Adj(cur) {
			if marked[e] {
				continue
			}
			marked[e] = true
			queue = append(queue, e)
		}
	}
}

func BFS(g Graph, start int) {
	BFSFunc(g, start, func(v int) bool {
		fmt.Printf("%d ", v)
		return false
	})
}
