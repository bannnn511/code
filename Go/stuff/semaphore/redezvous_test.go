package semaphore

import (
	"sync"
	"testing"
)

func TestRendezvous(t *testing.T) {
	go task2()
	go task1()
}

func TestRendezvous2(t *testing.T) {
	var wg sync.WaitGroup
	wg.Add(2)

	go func() {
		defer wg.Done()
		task3()
	}()

	go func() {
		defer wg.Done()
		task4()
	}()

	wg.Wait()
}
