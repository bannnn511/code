package __Graph

import (
	"fmt"
	"testing"
)

func TestLabyrinth(t *testing.T) {
	type args struct {
		lb [][]byte
		n  int
		m  int
	}
	tests := []struct {
		name  string
		args  args
		want  bool
		want1 int
	}{
		{
			name: "1",
			args: args{
				lb: [][]byte{
					{
						'#', '#', '#', '#', '#', '#', '#', '#',
					},
					{
						'#', '0', 'A', '#', '0', '0', '0', '#',
					},
					{
						'#', '0', '#', '#', '0', '#', 'B', '#',
					},
					{
						'#', '0', '0', '0', '0', '0', '0', '#',
					},
					{
						'#', '#', '#', '#', '#', '#', '#', '#',
					},
				},
				n: 5,
				m: 8,
			},
			want:  true,
			want1: 9,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, paths := Labyrinth(tt.args.lb, tt.args.n, tt.args.m)
			if got != tt.want {
				t.Errorf("Labyrinth() got = %v, want %v", got, tt.want)
			}
			if len(paths) != tt.want1 {
				t.Errorf("Labyrinth() got1 = %v, want %v", len(paths), tt.want1)
			}

			for i := 0; i < len(paths); i++ {
				fmt.Println(paths[i])
			}
		})
	}
}
