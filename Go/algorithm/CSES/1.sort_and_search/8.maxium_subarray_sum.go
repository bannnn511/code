package __sort_and_search

import (
	"fmt"
)

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
