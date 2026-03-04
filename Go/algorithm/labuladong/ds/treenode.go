package ds

import "fmt"

type TreeNode struct {
	key   int
	left  *TreeNode
	right *TreeNode
}

func postOrderTraverse(root *TreeNode) {
	if root == nil {
		return
	}

	postOrderTraverse(root.left)
	postOrderTraverse(root.right)
	fmt.Printf("value %d\n", root.key)
}

func inOrderTraverse(root *TreeNode) {
	if root == nil {
		return
	}

	inOrderTraverse(root.left)
	fmt.Printf("value %d\n", root.key)
	inOrderTraverse(root.right)
}

func preOrderTraverse(root *TreeNode) {
	if root == nil {
		return
	}

	fmt.Printf("value %d\n", root.key)
	inOrderTraverse(root.left)
	inOrderTraverse(root.right)
}

type State struct {
	node  *TreeNode
	depth int
}

func levelOrderTraversal(root *TreeNode) {
	if root == nil {
		return
	}

	q := []*State{{node: root, depth: 1}}

	for len(q) > 0 {
		cur := q[0]
		fmt.Printf("%d ", cur.node.key)
		q = q[1:]

		if cur.node.left != nil {
			q = append(q, &State{cur.node.left, cur.depth + 1})
		}
		if cur.node.right != nil {
			q = append(q, &State{cur.node.right, cur.depth + 1})
		}

		fmt.Println()
	}
}
