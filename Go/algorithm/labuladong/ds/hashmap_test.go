package ds

import (
	"testing"
)

func TestLinearProbing(t *testing.T) {
	hashMap := NewLinearProbingHashMap()
	hashMap.Put(1, 10)
	hashMap.Put(2, 20)
	if v := hashMap.Get(1); v != 10 {
		t.Errorf("expected %d, got %d", 10, v)
	}
	if v := hashMap.Get(2); v != 20 {
		t.Errorf("expected %d, got %d", 20, v)
	}
	if v := hashMap.Get(3); v != -1 {
		t.Errorf("expected %d, got %d", -1, v)
	}

	hashMap.Put(2, 30)
	if v := hashMap.Get(2); v != 30 {
		t.Errorf("expected %d, got %d", 30, v)
	}

	hashMap.Remove(2)
	if v := hashMap.Get(2); v != -1 {
		t.Errorf("expected %d, got %d", -1, v)
	}

	hashMap.Put(22, 40)
	if v := hashMap.Get(22); v != 40 {
		t.Errorf("expected %d, got %d", 40, v)
	}
}
