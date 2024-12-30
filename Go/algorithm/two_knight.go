package algorithm

import (
	"math"
)

type Point struct {
	Row int
	Col int
}

// GetNextPoint returns the next point in the matrix
func GetNextPoint(matrixSize int, current Point) *Point {
	if current.Row < 0 || current.Row >= matrixSize || current.Col < 0 || current.Col >= matrixSize {
		return nil // Invalid input
	}

	// Calculate next column
	nextCol := current.Col + 1
	nextRow := current.Row

	// Move to the next row if needed
	if nextCol >= matrixSize {
		nextCol = 0
		nextRow++
	}

	// If the next point is out of bounds, return nil
	if nextRow >= matrixSize {
		return nil
	}

	return &Point{Row: nextRow, Col: nextCol}
}

type TwoKnight struct {
	n     int
	count int
}

func NewTwoKnight(n int) *TwoKnight {
	return &TwoKnight{
		n:     n,
		count: 0,
	}
}

func (q *TwoKnight) check(p1, p2 Point) bool {
	x := math.Pow(float64(p1.Row-p2.Row), 2)
	y := math.Pow(float64(p1.Col-p2.Col), 2)
	return x+y == 5
}

func (q *TwoKnight) search() {
	for x := 0; x < q.n; x++ {
		for y := 0; y < q.n; y++ {
			next := GetNextPoint(q.n, Point{x, y})
			for next != nil {
				if q.check(Point{x, y}, *next) {
					next = GetNextPoint(q.n, *next)
					continue
				}
				q.count++
				next = GetNextPoint(q.n, *next)
			}
		}
	}
}
