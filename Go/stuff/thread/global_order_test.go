package thread

import (
	"fmt"
	"sync"
	"testing"
)

func Test_addVector(t *testing.T) {
	v1 := NewVector([]int{1, 2, 3})
	v2 := NewVector([]int{1, 1, 1})

	loops := 10000
	threads := 2
	wg := sync.WaitGroup{}
	for i := 0; i < threads; i++ {
		wg.Add(1)
		go func() {
			for j := 0; j < loops; j++ {
				x, y := v1, v2
				a, b := 0, 1
				if j%2 == 1 {
					x, y = v2, v1
					a, b = 1, 0
				}

				fmt.Printf("-> add(%v,%v)\n", a, b)
				addVector(x, y)
				fmt.Printf("			<- add(%v,%v)\n", a, b)
			}
			wg.Done()
		}()
	}
	wg.Wait()
}
