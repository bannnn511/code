package CSES

import (
	"bufio"
	"errors"
	"fmt"
	"io/fs"
	"os"
	"strconv"
	"testing"
)

func check(t *testing.T, err error) {
	t.Helper()
	if err != nil {
		t.Fatalf(err.Error())
	}
}

func ReadInts(t *testing.T) []int {
	t.Helper()
	var inputs []int
	for i := 1; ; i++ {
		file, err := os.OpenFile(fmt.Sprintf("tests/%d.in", i), os.O_RDONLY, 0)
		if errors.Is(err, fs.ErrNotExist) {
			break
		} else if err != nil {
			check(t, err)
		}
		scan := bufio.NewScanner(file)
		for scan.Scan() {
			input, err := strconv.Atoi(scan.Text())
			check(t, err)
			inputs = append(inputs, input)
		}
	}

	return inputs
}

func Test_weirdAlgorithm(t *testing.T) {
	tests := ReadInts(t)
	for i, tt := range tests {
		t.Run(fmt.Sprintf("%d", i), func(t *testing.T) {
			weirdAlgorithm(tt)
		})
	}
}
