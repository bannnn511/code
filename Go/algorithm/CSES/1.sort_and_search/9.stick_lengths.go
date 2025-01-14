package __sort_and_search

import "math"

func stickLengths(sticks []int) int {
	minValue := int(math.Pow(10, 9))

	for _, s := range sticks {
		sum := 0
		for _, k := range sticks {
			sum += int(math.Abs(float64(s - k)))
		}
		minValue = min(minValue, sum)
	}

	return minValue
}
