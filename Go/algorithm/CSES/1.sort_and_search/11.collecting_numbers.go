package __sort_and_search

func collectingNumbers(array []int) int {
	count := 0
	for i := range array {
		if i == len(array)-1 {
			break
		}
		if array[i] > array[i+1] {
			count++
		}
	}

	return count
}
