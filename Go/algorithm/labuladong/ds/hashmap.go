package ds

type KVNode struct {
	key   int
	value int
}

type LinearProbingHashMap struct {
	table []*KVNode
}

func NewLinearProbingHashMap() *LinearProbingHashMap {
	var hashMap LinearProbingHashMap
	hashMap.table = make([]*KVNode, 10)

	return &hashMap
}

func (m *LinearProbingHashMap) hash(key int) int {
	return key % len(m.table)
}

func (m *LinearProbingHashMap) Put(key int, value int) {
	index := m.hash(key)

	node := m.table[index]
	newNode := KVNode{
		key:   key,
		value: value,
	}
	if node == nil {
		m.table[index] = &newNode
	} else {
		for index < len(m.table) && m.table[index].key != key {
			index++
		}

		m.table[index] = &newNode
	}
}

func (m *LinearProbingHashMap) Get(key int) int {
	hashKey := m.hash(key)

	for hashKey < len(m.table) && m.table[hashKey] != nil && m.table[hashKey].key != key {
		hashKey++
	}

	if m.table[hashKey] == nil {
		return -1
	}

	return m.table[hashKey].value
}

func (m *LinearProbingHashMap) Remove(key int) {
	index := m.hash(key)
	for index < len(m.table) && m.table[index] != nil && m.table[index].key != key {
		index++
	}
	m.table[index] = nil
}
