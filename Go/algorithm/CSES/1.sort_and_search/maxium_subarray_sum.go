package __sort_and_search

import (
	"cmp"
	"fmt"
)

func max[T cmp.Ordered](a, b T) T {
	if a > b {
		return a
	}

	return b
}

func maximumSubArraySum(a []int, x int) int {
	best := a[0]
	for k := 0; k < len(a); k++ {
		sum := 0
		for i := k; i < len(a); i++ {
			sum += a[i]
			fmt.Printf("k: %d, sum: %d, best: %d\n", k, sum, best)
			best = max(best, sum)
		}
	}

	return best
}

func maximumSubArraySum2(a []int, x int) int {
	best := 0
	sum := 0
	for k := 0; k < len(a); k++ {
		sum += a[k]
		best = max(best, sum)
		if sum < 0 {
			sum = 0
		}
		fmt.Printf("k: %d, sum: %d, best: %d\n", k, sum, best)
	}

	return best
}
