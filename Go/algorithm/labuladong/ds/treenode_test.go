package ds

import "testing"

func Test_traverse(t *testing.T) {
	//       1
	//      / \
	//     2   3
	//    / \ / \
	//   7  4 5  6
	root := &TreeNode{
		key: 1,
		left: &TreeNode{
			key: 2,
			left: &TreeNode{
				key: 7,
			},
			right: &TreeNode{
				key: 4,
			},
		},
		right: &TreeNode{
			key: 3,
			left: &TreeNode{
				key: 5,
			},
			right: &TreeNode{
				key: 6,
			},
		},
	}

	// preOrderTraverse(root)
	// inOrderTraverse(root)
	// postOrderTraverse(root)
	levelOrderTraversal(root)
}
