package ds

import (
	"bytes"
	"fmt"
)

type MyLinkedList struct {
	head *Node
	tail *Node
	size int
}

type Node struct {
	val  int
	next *Node
	prev *Node
}

func NewLinkedList() MyLinkedList {
	head := &Node{}
	tail := &Node{}
	head.next = tail
	tail.prev = head

	return MyLinkedList{
		head: head,
		tail: tail,
		size: 0,
	}
}

func (this *MyLinkedList) getNode(index int) *Node {
	if !this.isElementIndex(index) {
		return nil
	}

	cur := this.head.next
	for i := 0; i < index; i++ {
		cur = cur.next
	}

	return cur
}

func (this *MyLinkedList) Get(index int) int {
	if !this.isElementIndex(index) {
		return -1
	}

	cur := this.head.next
	for i := 0; i < index; i++ {
		cur = cur.next
	}

	return cur.val
}

func (this *MyLinkedList) AddAtHead(val int) {
	temp := &Node{val: val}
	head := this.head.next

	temp.next = head
	head.prev = temp

	this.head.next = temp
	temp.prev = this.head
	this.size++
}

func (this *MyLinkedList) AddAtTail(val int) {
	temp := &Node{val: val}
	tail := this.tail.prev

	temp.next = this.tail
	temp.prev = tail

	tail.next = temp
	this.tail.prev = temp
	this.size++
}

func (this *MyLinkedList) AddAtIndex(index int, val int) {
	if !this.isPositionIndex(index) {
		return
	}

	if index == this.size {
		this.AddAtTail(val)
		return
	}

	cur := this.getNode(index)
	if cur == nil {
		return
	}

	temp := &Node{val: val}
	temp.next = cur
	prev := cur.prev
	prev.next = temp
	cur.prev = temp
	temp.prev = prev

	this.size++
}

func (this *MyLinkedList) DeleteAtIndex(index int) {
	if !this.isElementIndex(index) {
		return
	}

	cur := this.getNode(index)
	if cur == nil {
		return
	}

	prev := cur.prev
	prev.next = cur.next
	cur.next.prev = prev
	this.size--
}

func (this *MyLinkedList) isElementIndex(index int) bool {
	return index >= 0 && index < this.size
}

func (this *MyLinkedList) isPositionIndex(index int) bool {
	return index >= 0 && index <= this.size
}

func (this *MyLinkedList) String() string {
	var buffer bytes.Buffer
	cur := this.head.next
	for cur != this.tail {
		buffer.WriteString(fmt.Sprintf("%d ", cur.val))
		cur = cur.next
	}
	return buffer.String()
}
