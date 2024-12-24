package algorithm

import "fmt"

func subsetOf(n int) [][]int {
	var subsets [][]int
	for b := 0; b < (1 << n); b++ {
		fmt.Println("=======")
		var subset []int
		for i := 0; i < n; i++ {
			fmt.Println(b, i, 1<<i, b&(1<<i))
			if b&(1<<i) != 0 {
				fmt.Println(b, i, 1<<i, b&(1<<i))
				subset = append(subset, i)
			}
		}
		subsets = append(subsets, subset)
	}
	return subsets
}
