package __sort_and_search

import "sort"

func missingCoinSum(coins []int) int {
	sort.Ints(coins)
	sum := 1
	for _, c := range coins {
		if c > sum {
			return sum
		}
		sum += c
	}

	return sum
}
