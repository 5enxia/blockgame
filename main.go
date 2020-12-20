package main

// #cgo LDFLAGS: -lncurses
// #include<ncurses.h>
import "C"


func main() {
    C.initscr()
    C.getch()
    C.endwin()
}
