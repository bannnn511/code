package __dynamic_programming

func coinCombination(coins []int, x int) int {
	ready := make([]int, x+1)
	ready[0] = 1

	for k := 1; k <= x; k++ {
		for _, c := range coins {
			if (k - c) < 0 {
				continue
			}
			ready[k] += ready[k-c]
		}
	}

	return ready[x]
}
