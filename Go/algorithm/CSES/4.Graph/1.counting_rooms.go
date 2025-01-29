package __Graph

func CountingRooms(n, m int, grid [][]byte) int {
	// 4 directions
	dx := []int{0, 0, 1, -1}
	dy := []int{1, -1, 0, 0}

	// dfs
	var dfs func(x, y int)

	dfs = func(x, y int) {
		if x < 0 || x >= n || y < 0 || y >= m || grid[x][y] == '#' {
			return
		}
		grid[x][y] = '#'
		for i := 0; i < 4; i++ {
			dfs(x+dx[i], y+dy[i])
		}
	}

	rooms := 0
	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			if grid[i][j] != '#' {
				rooms++
				dfs(i, j)
			}
		}
	}
	return rooms
}
