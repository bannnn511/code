package __dynamic_programming

func minimizingCoins(coins []int, x int) int {
	ready := make([]int, x+1)
	for i := 1; i < len(ready); i++ {
		ready[i] = x + 1
	}

	for k := 1; k <= x; k++ {
		for _, c := range coins {
			if k < c {
				continue
			}
			ready[k] = min(ready[k], ready[k-c]+1)
		}
	}

	return ready[x]
}
