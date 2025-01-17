package __sort_and_search

import "testing"

func Test_longestSequence(t *testing.T) {
	type args struct {
		playlist []int
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1",
			args: args{playlist: []int{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
			want: 1,
		},
		{
			name: "2 2 1 1 2 1 2 1 2 1",
			args: args{playlist: []int{2, 2, 1, 1, 2, 1, 2, 1, 2, 1}},
			want: 2,
		},
		{
			name: " 1 2 1 3 2 7 4",
			args: args{playlist: []int{1, 2, 1, 3, 2, 7, 4}},
			want: 5,
		},
		{
			name: "3 3 3 3 5 1 5 1 1 4",
			args: args{playlist: []int{3, 3, 3, 3, 5, 1, 5, 1, 1, 4}},
			want: 3,
		},
		{
			name: "1 1 3 4 5",
			args: args{playlist: []int{1, 1, 3, 4, 5}},
			want: 4,
		},
		{
			name: "1 2 2 3 4 3 4 2 1",
			args: args{playlist: []int{1, 2, 2, 3, 4, 3, 4, 2, 1}},
			want: 4,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := longestSequence(tt.args.playlist); got != tt.want {
				t.Errorf("longestSequence() = %v, want %v", got, tt.want)
			}
		})
	}
}
