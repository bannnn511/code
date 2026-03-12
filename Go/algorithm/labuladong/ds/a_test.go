package ds

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

type ListNode struct {
	Val  int
	Next *ListNode
}

func partition(head *ListNode, x int) *ListNode {
	dummy1 := &ListNode{}
	dummy2 := &ListNode{}

	p1 := dummy1
	p2 := dummy2
	for head != nil {
		if head.Val < x {
			p1.Next = head
			p1 = p1.Next
		} else {
			p2.Next = head
			p2 = p2.Next
		}

		head = head.Next
	}

	p1.Next = dummy2.Next

	return dummy1.Next
}

func TestPartition(t *testing.T) {
	head := &ListNode{Val: 1}
	head.Next = &ListNode{Val: 4}
	head.Next.Next = &ListNode{Val: 3}
	head.Next.Next.Next = &ListNode{Val: 2}
	head.Next.Next.Next.Next = &ListNode{Val: 5}
	head.Next.Next.Next.Next.Next = &ListNode{Val: 2}

	result := partition(head, 3)
	assert.Equal(t, 1, result.Val)
	assert.Equal(t, 2, result.Next.Val)
	assert.Equal(t, 2, result.Next.Next.Val)
	assert.Equal(t, 4, result.Next.Next.Next.Val)
	assert.Equal(t, 3, result.Next.Next.Next.Next.Val)
	assert.Equal(t, 5, result.Next.Next.Next.Next.Next.Val)
}
