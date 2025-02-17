package stuff

import (
	"reflect"
	"testing"
)

func Test_operations(t *testing.T) {
	type args struct {
		arr        []int32
		operations [][]int32
	}
	tests := []struct {
		name string
		args args
		want []int32
	}{
		{
			name: "Test 1",
			args: args{
				arr: []int32{9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
				operations: [][]int32{
					{0, 9},
					{4, 5},
					{3, 6},
					{2, 7},
					{1, 8},
					{0, 9},
				},
			},
			want: []int32{9, 1, 7, 3, 5, 4, 6, 2, 8, 0},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := operations(tt.args.arr, tt.args.operations); !reflect.DeepEqual(got, tt.want) {
				t.Errorf("operations() = %v, want %v", got, tt.want)
			}
		})
	}
}
