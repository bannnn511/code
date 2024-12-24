package CSES

import "fmt"

func weirdAlgorithm(n int) string {
	out := ""
	out += fmt.Sprintf("%d", n)
	for n > 1 {
		if n%2 == 0 {
			n /= 2
		} else {
			n = n*3 + 1
		}
		out += fmt.Sprintf(" %d", n)
	}

	return out
}
