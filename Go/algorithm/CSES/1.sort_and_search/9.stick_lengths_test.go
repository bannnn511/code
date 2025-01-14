package __sort_and_search

import "testing"

func Test_stickLengths(t *testing.T) {
	type args struct {
		sticks []int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "1",
			args: args{sticks: []int{2, 3, 1, 5, 2}},
			want: 5,
		},
		{
			name: "2",
			args: args{sticks: []int{3, 4, 4, 4, 4, 4, 4}},
			want: 1,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := stickLengths(tt.args.sticks); got != tt.want {
				t.Errorf("stickLengths() = %v, want %v", got, tt.want)
			}
		})
	}
}
