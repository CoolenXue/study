package main

import . "fmt"

func main(){
	//Print/ln/f
	var i int
	var s string 
	var b bool
	length, ret := Scanln(&i, &s, &b)
	Printf("Input(%v-%v): %v, %v, %v\n", length, ret, i, s, b)
}
