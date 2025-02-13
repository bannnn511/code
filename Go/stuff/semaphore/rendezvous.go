package semaphore

import (
	"fmt"
	"sync"
)

var (
	// Two channels for bidirectional synchronization
	arrive chan struct{}
	depart chan struct{}
)

func init() {
	arrive = make(chan struct{}, 1)
	depart = make(chan struct{}, 1)
}

func task3() {
	fmt.Printf("Task 3: before\n")
	arrive <- struct{}{}
	<-depart
	fmt.Printf("Task 3: after\n")
}

func task4() {
	fmt.Printf("Task 4: before\n")
	depart <- struct{}{}
	<-arrive
	fmt.Printf("Task 4: after\n")
}

var sem1 *Zemaphore
var sem2 *Zemaphore

func init() {
	sem1 = NewZemaphore(0)
	sem2 = NewZemaphore(0)
}

func task1() {
	fmt.Printf("Task 1: before\n")
	sem1.Signal()
	sem2.Wait()
	fmt.Printf("Task 1: after\n")
}

func task2() {
	fmt.Printf("Task 2: before\n")
	sem2.Signal()
	sem1.Wait()
	fmt.Printf("Task 2: after\n")
}

type Zemaphore struct {
	value int
	cond  sync.Cond
}

func NewZemaphore(v int) *Zemaphore {
	mu := new(sync.Mutex)
	return &Zemaphore{
		value: v,
		cond:  *sync.NewCond(mu),
	}
}

func (z *Zemaphore) Signal() {
	z.cond.L.Lock()
	z.value++
	z.cond.Signal()
	z.cond.L.Unlock()
}

func (z *Zemaphore) Wait() {
	z.cond.L.Lock()
	for z.value <= 0 {
		z.cond.Wait()
	}
	z.value--

	z.cond.L.Unlock()
}
