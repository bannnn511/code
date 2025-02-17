package stuff

func reverse(arr []int32) []int32 {
	for i, j := 0, len(arr)-1; i < j; i, j = i+1, j-1 {
		arr[i], arr[j] = arr[j], arr[i]
	}
	return arr
}

func operations(arr []int32, operations [][]int32) []int32 {
	for _, operation := range operations {
		start := operation[0]
		end := operation[1]

		arr = append(arr[:start], append(reverse(arr[start:end+1]), arr[end+1:]...)...)

	}
	return arr
}
