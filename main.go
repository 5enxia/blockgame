package main

// #cgo LDFLAGS: -lncurses
// #include<ncurses.h>
// #include"main.h"
import "C"


func main() {
    C.initscr()

    row := 12
    col := 30
    message := "Hello, World!"
    C.myMvprintw(C.int(row), C.int(col), C.CString(message))

    for {
        ch := C.getch()
        if ch == C.int('q') {
            break
        }
    }

    C.endwin()
}
