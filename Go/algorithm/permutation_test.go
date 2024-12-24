package algorithm

import "testing"

func Test_generate(t *testing.T) {
	type args struct {
		k int
		a []int
	}
	tests := []struct {
		name string
		args args
	}{
		{
			name: "1. 1 item",
			args: args{
				k: 1,
				a: []int{1},
			},
		},
		{
			name: "2. 2 items",
			args: args{
				k: 2,
				a: []int{1, 2},
			},
		},
		{
			name: "3. 3 items",
			args: args{
				k: 3,
				a: []int{1, 2, 3},
			},
		},
		{
			name: "4. 4 items",
			args: args{
				k: 4,
				a: []int{1, 2, 3, 4},
			},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			generate(tt.args.k, tt.args.a)
		})
	}
}
