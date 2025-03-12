package main

import (
	"fmt"
	"os"
	"os/signal"
	"regexp"
	"runtime"
	"slices"
	"syscall"
	"time"
)

var pid int

func stingy(money *int, loop int) {
	for i := 0; i < loop; i++ {
		*money += 10
		// syscall.Kill(pid, syscall.SIGURG)
	}
	fmt.Println("Stingy Done")
}

func spendy(money *int, loop int) {
	for i := 0; i < loop; i++ {
		*money -= 10
		// syscall.Kill(pid, syscall.SIGURG)
	}
	fmt.Println("Spendy Done")
}

func concurrentUpdate(loop int) int {
	pid = syscall.Getpid()
	c := make(chan os.Signal, 1)
	signal.Notify(c, syscall.SIGURG)
	go func() {
		for {
			select {
			case <-c:
				fmt.Println("signal -----", pid)
			default:
			}
		}
	}()

	runtime.GOMAXPROCS(1)
	money := 100
	go spendy(&money, loop)
	go stingy(&money, loop)
	time.Sleep(1 * time.Second)
	println("Money in bank account: ", money)
	return money
}

type blah struct {
	b int
}

type test struct {
	a *blah
}

func testHeap() test {
	blah1 := new(blah)
	blah1.b = 2
	b := new(test)
	b.a = blah1

	i := 0
	for {
		isHeap := &blah{}
		if i == 10 {
			break
		}
		i++
		_ = isHeap
	}

	return *b
}

func testString(a *string) {
	test := "hello"
	a = &test
}

/*
go build -gcflags=-m=3  main.go
*/
func main() {
	testHeap()
	b := "world"
	testString(&b)
	fmt.Println(b)

	a := []int{1, 2, 3}
	slices.Sort(a)
	c := append(a, 4, 5, 6)
	fmt.Println(a, c)
}

func ChangeArray(slice []int) {
	slice = append(slice, 4, 5)
}

func Append(slice []int, data ...int) []int {
	l := len(slice)
	if l+len(data) > cap(slice) {
		newSlice := make([]int, (l+len(data))*2)
		copy(newSlice, slice)
		slice = newSlice
	}

	slice = slice[0 : l+len(data)]
	copy(slice[l:], data)

	return slice
}

var digitRegexp = regexp.MustCompile(`\d+`)

func CopyDigits(filename string) []byte {
	b, _ := os.ReadFile(filename)
	b = digitRegexp.Find(b)
	c := make([]byte, len(b))
	copy(c, b)
	return c
}
