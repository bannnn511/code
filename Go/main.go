package main

import (
	"fmt"
	"time"
)

func main() {
	start := time.Now()
	arr := []int{1, -5, 3, 4, 5}

	sum := 0
	best := 0
	for _, num := range arr {
		sum = max(num, sum+num)
		best = max(best, sum)
	}
	end := time.Since(start)

	fmt.Println("Sum of array is: ", sum)
	fmt.Println("time: ", end)
}

func max(a, b int) int {
	if a > b {
		return a
	}

	return b
}
