package loadbalance

import (
	"fmt"
	"testing"
)

// func TestLFUupdateFreq(t *testing.T) {
// cache := NewLFU(2)
// cache.updateFreq(1)
// cache.updateFreq(2)
// }

func TestLFU(t *testing.T) {
	cache := NewLFU(2)
	cache.Put(1, 1)
	fmt.Println(cache)
	cache.Put(2, 2)
	fmt.Println(cache)
	if cache.Get(1) != 1 {
		t.Errorf("Get(1) != 1")
	}
	fmt.Println(cache)

	cache.Put(3, 3)
	fmt.Println(cache)
	if cache.Get(2) != -1 {
		t.Errorf("Get(2) != -1")
	}
	fmt.Println(cache)
	if cache.Get(3) != 3 {
		t.Errorf("Get(3) != 3")
	}
	fmt.Println(cache)
	if cache.Get(1) != 1 {
		t.Errorf("Get(1) != 1")
	}
	fmt.Println(cache)
}
