package main

import "fmt"

// import (
// 	"fmt"
// 	"os"
// 	"os/signal"
// 	"runtime"
// 	"syscall"
// 	"time"
// )
//
// var pid int
//
// const loop = 1e9
//
// func stingy(money *int) {
// 	for i := 0; i < loop; i++ {
// 		*money += 10
// 		// syscall.Kill(pid, syscall.SIGURG)
// 	}
// 	fmt.Println("Stingy Done")
// }
// func spendy(money *int) {
// 	for i := 0; i < loop; i++ {
// 		*money -= 10
// 		// syscall.Kill(pid, syscall.SIGURG)
// 	}
// 	fmt.Println("Spendy Done")
// }
//
// func main() {
// 	pid = syscall.Getpid()
// 	c := make(chan os.Signal, 1)
// 	signal.Notify(c, syscall.SIGURG)
// 	go func() {
// 		for {
// 			select {
// 			case <-c:
// 				fmt.Println("signal -----", pid)
// 			default:
// 			}
// 		}
// 	}()
//
// 	runtime.GOMAXPROCS(1)
// 	money := 100
// 	go spendy(&money)
// 	go stingy(&money)
// 	time.Sleep(1 * time.Second)
// 	println("Money in bank account: ", money)
// }

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
		fmt.Println(isHeap)
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
}
