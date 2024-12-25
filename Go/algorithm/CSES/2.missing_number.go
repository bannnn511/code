package CSES

func findMissingNumber(x int, a []int) int {
	sum := 0
	for i := 1; i <= x; i++ {
		sum += i
	}

	for _, b := range a {
		sum -= b
	}

	return sum
}
