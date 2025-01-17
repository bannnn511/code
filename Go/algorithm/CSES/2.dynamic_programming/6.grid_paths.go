package __dynamic_programming

func gridPath(squares [][]int) int {
	length := len(squares)

	paths := make([][]int, length)
	for i := range length {
		paths[i] = make([]int, length)
	}

	for x := 1; x < length; x++ {
		for y := 1; y < length; y++ {
			if x == 1 &&
				y == 1 &&
				squares[1][1] != -1 {
				paths[x][y] = 1
				continue
			}

			if squares[x][y] == -1 {
				paths[x][y] = 0
			} else {
				paths[x][y] = paths[x-1][y] + paths[x][y-1]
			}
		}
	}

	return paths[length-1][length-1]
}
