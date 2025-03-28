package thread

import (
	"fmt"
	"sync"
)

type vector struct {
	arr  []int
	lock *sync.Mutex
}

func NewVector(arr []int) *vector {
	return &vector{
		arr:  arr,
		lock: &sync.Mutex{},
	}
}

func addVector(vDst *vector, vSrc *vector) {
	v1 := fmt.Sprintf("%p", vDst.lock)
	v2 := fmt.Sprintf("%p", vSrc.lock)
	// fmt.Printf("v1: %v, v2: %v\n", v1, v2)
	if v1 < v2 {
		vDst.lock.Lock()
		vSrc.lock.Lock()
	} else if v1 > v2 {
		vSrc.lock.Lock()
		vDst.lock.Lock()
	} else {
		vSrc.lock.Lock()
	}

	for i := range vDst.arr {
		vDst.arr[i] += vSrc.arr[i]
	}

	vSrc.lock.Unlock()
	if v1 != v2 {
		vDst.lock.Unlock()
	}
}
