package __Graph

import "testing"

func TestCountingRooms(t *testing.T) {
	type args struct {
		n    int
		m    int
		grid [][]byte
	}
	tests := []struct {
		name string
		args args
		want int
	}{
		{
			name: "1.",
			args: args{
				n: 5,
				m: 8,
				grid: [][]byte{
					{
						'#', '#', '#', '#', '#', '#', '#', '#',
					},
					{

						'#', '0', '0', '#', '0', '0', '0', '#',
					},
					{

						'#', '#', '#', '#', '0', '#', '0', '#',
					},
					{
						'#', '0', '0', '#', '0', '0', '0', '#',
					},
					{
						'#', '#', '#', '#', '#', '#', '#', '#',
					},
				},
			},
			want: 3,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := CountingRooms(tt.args.n, tt.args.m, tt.args.grid); got != tt.want {
				t.Errorf("CountingRooms() = %v, want %v", got, tt.want)
			}
		})
	}
}
