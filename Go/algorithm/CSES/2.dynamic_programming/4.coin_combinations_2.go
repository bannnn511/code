package __dynamic_programming

func coinCombination2(coins []int, x int) int {
	ready := make([]int, x+1)
	ready[0] = 1

	for _, c := range coins {
		for k := 1; k <= x; k++ {
			if (k - c) < 0 {
				continue
			}
			ready[k] += ready[k-c]
		}
	}

	return ready[x]
}
