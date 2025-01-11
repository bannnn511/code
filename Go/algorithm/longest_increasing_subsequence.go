package algorithm

func longestIncreasingSubsequence(array []int, n int) int {
	length := make([]int, len(array))
	for k := 0; k <= n; k++ {
		length[k] = 1
		for i := 0; i < k; i++ {
			if array[i] < array[k] {
				length[k] = max(length[k], length[i]+1)
			}
		}
	}

	return length[n]
}

func longestIncreasingSubsequence2(array []int, n int) int {
	value := make([]int, len(array))
	cache := make([]int, len(array))
	value[0] = 1
	for i := 1; i <= n; i++ {
		if array[i] > array[i-1] {
			value[i] = value[i-1] + 1
		} else {
			value[i] = 1
			cache[i] = max(array[i-1], array[i])
		}
	}

	return value[n]
}
