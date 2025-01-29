package graph

type Path struct {
	g      IGraph
	edgeTo []int
	marked []bool
	final  int // source
}

func NewPath(g IGraph, start int) *Path {
	edgeTo := make([]int, g.V())
	marked := make([]bool, g.V())
	stack := make([]int, 0, g.V())

	stack = append(stack, start)
	for len(stack) > 0 {
		cur := stack[0]
		stack = stack[1:]

		for e := range g.Adj(cur) {
			if marked[e] {
				continue
			}
			marked[e] = true
			edgeTo[e] = cur
			stack = append(stack, e)
		}
	}

	return &Path{
		g:      g,
		edgeTo: edgeTo,
		marked: marked,
		final:  start,
	}
}

func (p Path) HasPathTo(v int) bool {
	return p.marked[v]
}

func (p Path) PathTo(v int) []int {
	if !p.HasPathTo(v) {
		return nil
	}

	var stack []int
	for x := v; x != p.final; x = p.edgeTo[x] {
		stack = append([]int{x}, stack...)
	}
	stack = append([]int{p.final}, stack...)

	return stack
}
