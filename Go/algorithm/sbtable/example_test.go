package sbtable_test

import (
	"fmt"

	"github.com/bannnn511/code/go/algorithm/sbtable"
)

func ExampleBinarySearchST() {
	// Create a new Binary Search Symbol Table
	bst := sbtable.NewBS[string, int]()

	// Insert key-value pairs
	bst.Put("a", 1)
	bst.Put("b", 2)
	bst.Put("c", 3)

	// Retrieve values
	fmt.Println(bst.Get("a"))
	fmt.Println(bst.Get("b"))
	fmt.Println(bst.Get("c"))

	// Check the rank of keys
	fmt.Println(bst.Rank("a"))
	fmt.Println(bst.Rank("b"))
	fmt.Println(bst.Rank("c"))

	// Output:
	// 1
	// 2
	// 3
	// 2
	// 1
	// 0
}
