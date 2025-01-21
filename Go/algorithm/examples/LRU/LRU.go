package main

//
// import (
// 	"fmt"
// )
//
// const isPrint = false
//
// type node struct {
// 	key   int
// 	value int
// 	next  *node
// 	prev  *node
// }
//
// func newNode(key, value int) *node {
// 	return &node{
// 		key:   key,
// 		value: value,
// 	}
// }
//
// type LRU struct {
// 	count int
// 	cap   int
// 	cache []*node
// 	head  *node
// 	tail  *node
// }
//
// func (l *LRU) removeNode(n *node) {
// 	if n == nil {
// 		return
// 	}
// 	prevNode := n.prev
// 	nextNode := n.next
// 	prevNode.next = nextNode
// 	nextNode.prev = prevNode
// }
//
// func (l *LRU) removeLastUse() {
// 	tmp := l.tail.prev
// 	l.removeNode(tmp)
// }
//
// func (l *LRU) addNode(leaf *node) {
// 	if leaf == nil {
// 		return
// 	}
// 	tmp := l.head.next
// 	tmp.prev = leaf
// 	leaf.next = tmp
// 	leaf.prev = l.head
// 	l.head.next = leaf
// }
//
// // func (l *LRU) find(key int) *node {
// // 	tmp := l.head.next
// // 	for tmp != l.tail {
// // 		if tmp.key == key {
// // 			return tmp
// // 		}
// // 	}
// //
// // 	return nil
// // }
//
// func NewLRU(cap int) *LRU {
// 	head := new(node)
// 	tail := new(node)
// 	head.next = tail
// 	tail.prev = head
//
// 	return &LRU{
// 		cap:   cap,
// 		cache: make([]*node, 10000),
// 		head:  head,
// 		tail:  tail,
// 	}
// }
//
// func (l *LRU) Get(key int) int {
// 	l.debug()
// 	n := l.cache[key]
// 	if n == nil {
// 		return -1
// 	}
//
// 	l.removeNode(n)
// 	l.addNode(n)
// 	l.debug()
// 	return n.value
// }
//
// func (l *LRU) Set(key int, value int) {
// 	if l.count == l.cap {
// 		l.removeLastUse()
// 		l.debug()
// 	}
//
// 	n := newNode(key, value)
// 	l.cache[key] = n
// 	l.addNode(n)
//
// 	if l.count < l.cap {
// 		l.count++
// 	}
// 	l.debug()
// }
//
// func (l *LRU) debug() {
// 	if !isPrint {
// 		return
// 	}
// 	tmp := l.head
// 	for tmp != l.tail {
// 		fmt.Printf("%d/%d ", tmp.key, tmp.value)
// 		tmp = tmp.next
// 	}
// 	fmt.Println()
// }
//
// func CacheLRU() {
// 	cap := 0
// 	Q := 0
// 	fmt.Scanf("%d", &cap)
// 	fmt.Scanf("%d", &Q)
//
// 	lru := NewLRU(cap)
//
// 	for i := 0; i < Q; i++ {
// 		var cmd string
//
// 		fmt.Scanf("%s", &cmd)
// 		key := 0
// 		value := 0
// 		if cmd == "SET" {
// 			fmt.Scanf("%d %d", &key, &value)
// 			lru.Set(key, value)
// 		} else if cmd == "GET" {
// 			fmt.Scanf("%d", &key)
// 			v := lru.Get(key)
// 			fmt.Printf("%d ", v)
// 		} else {
// 			fmt.Printf("invalid cmd")
// 			return
// 		}
// 	}
// }
