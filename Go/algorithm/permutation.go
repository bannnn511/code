package algorithm

import "fmt"

func generate(k int, a []int) {
	if k == 1 {
		output(a)
		return
	}
	generate(k-1, a)

	for i := 0; i < k-1; i++ {
		if k%2 == 0 {
			a[i], a[k-1] = a[k-1], a[i]
		} else {
			a[0], a[k-1] = a[k-1], a[0]
		}
		generate(k-1, a)
	}
}

func output(a []int) {
	for _, x := range a {
		fmt.Printf("%d ", x)
	}
	fmt.Println()
}
