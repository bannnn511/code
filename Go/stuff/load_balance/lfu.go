package loadbalance

import (
	"fmt"
	"sort"
)

type LFU struct {
	cache   map[int]int
	counter map[int]int
	freq    map[int][]int
	count   int
	capcity int
	min     int
}

func NewLFU(capacity int) *LFU {
	return &LFU{
		cache:   make(map[int]int),
		freq:    make(map[int][]int),
		counter: make(map[int]int),
		count:   0,
		capcity: capacity,
	}
}

func (l *LFU) incrementFreq(key int) int {
	freq := l.counter[key]
	freq++
	if freq == 1 {
		l.min = 1
	}
	l.counter[key] = freq
	return freq
}

func (l *LFU) leastUseKey() int {
	least := l.freq[l.min]
	fmt.Println("min", l.min, "list", least)
	return least[0]
}

func (l *LFU) updateFreq(freq int, key int) {
	list := l.freq[freq]
	list = append(list, key)
	sort.Ints(list)
	l.freq[freq] = list
}

func (l *LFU) Get(key int) int {
	fmt.Printf("Get(%d)\n", key)
	v, ok := l.cache[key]
	if !ok {
		return -1
	}

	freq := l.incrementFreq(key)
	l.updateFreq(freq, key)

	l.count++

	return v
}

func (l *LFU) Put(key int, value int) {
	fmt.Printf("Put(%d, %d)\n", key, value)

	freq := l.incrementFreq(key)
	l.updateFreq(freq, key)

	if l.count >= l.capcity {
		// TODO: choose key to be removed
		least := l.leastUseKey()
		fmt.Println("delete", least)
	}

	l.cache[key] = value
	l.count++
}

func (l *LFU) String() string {
	s := ""
	for k, v := range l.cache {
		if v == -1 {
			continue
		}
		s += fmt.Sprintf("key: %d, value: %d, cnt(%v)=%d\n", k, v, k, l.counter[k])
	}
	return s
}
