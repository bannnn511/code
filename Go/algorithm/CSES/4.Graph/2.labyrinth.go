package __Graph

type loc struct {
	x int
	y int
}

func Labyrinth(lb [][]byte, n, m int) (bool, []loc) {
	dx := []int{0, 0, 1, -1}
	dy := []int{1, -1, 0, 0}
	isFound := false

	var start loc
	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			if lb[i][j] == 'A' {
				start.x = i
				start.y = j
				break
			}
		}
	}

	queue := make([]loc, 0, n+m)
	queue = append(queue, start)

	prev := make(map[loc]loc)
	prev[start] = loc{-1, -1}

	var final loc
	for len(queue) > 0 {
		cur := queue[0]
		queue = queue[1:]

		for i := 0; i < 4; i++ {
			x := cur.x + dx[i]
			y := cur.y + dy[i]

			if x < 0 || x >= n || y < 0 || y >= m || lb[x][y] == '#' {
				continue
			}

			if _, ok := prev[loc{x, y}]; !ok {
				prev[loc{x, y}] = cur
				queue = append(queue, loc{x, y})
			}

			if lb[x][y] == 'B' {
				isFound = true
				final.x = x
				final.y = y
				goto here
			}
		}
	}

here:
	var paths []loc
	if isFound {
		cur := final
		for cur != start {
			paths = append([]loc{cur}, paths...)
			cur = prev[cur]
		}
	}

	return isFound, paths
}
