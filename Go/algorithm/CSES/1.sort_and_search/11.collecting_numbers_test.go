package __sort_and_search

import "testing"

func Test_collectingNumbers(t *testing.T) {
	type args struct {
		array []int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "4,2,1,5,3",
			args: args{
				array: []int{4, 2, 1, 5, 3},
			},
			want: 3,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := collectingNumbers(tt.args.array); got != tt.want {
				t.Errorf("collectingNumbers() = %v, want %v", got, tt.want)
			}
		})
	}
}
