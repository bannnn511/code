package algorithm

func subsetOf(n int) [][]int {
	var subsets [][]int
	for b := 0; b < (1 << n); b++ {
		var subset []int
		for i := 0; i < n; i++ {
			if b&(1<<i) != 0 {
				subset = append(subset, i)
			}
		}
		subsets = append(subsets, subset)
	}
	return subsets
}
