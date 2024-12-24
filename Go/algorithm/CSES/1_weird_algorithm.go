package CSES

import "fmt"

func weirdAlgorithm(n int) {
	for n > 1 {
		if n%2 == 0 {
			n /= 2
		} else {
			n = n*3 + 1
		}
		fmt.Printf("%d ", n)
	}
}
