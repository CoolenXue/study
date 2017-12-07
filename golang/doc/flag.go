package main

import (
	. "fmt"
	"flag"
	"os"
)

func main(){
	Println("os.Args: ", os.Args)

	// two bind methods, pointer/variable 
	variable := 0
	flag.IntVar(&variable, "v", 1, "help message for variable")	//0 to 1 now

	pointer := flag.Int("p", 1, "help message for pointer")
	boo := flag.Bool("b", false, "help message for bool") //1, 0, t, f, T, F, true, false, TRUE, FALSE, True, False
	str := flag.String("s", "str", "help message for string")

    //Flag parsing stops just before the first non-flag argument or after the terminator "--".
	flag.Parse()

	//Usage: The function is a variable that may be changed to point to a custom function.
	flag.Usage()

	// cmd flag non-flag-argument
	/*
	-flag=x
	-flag	 // boolean only
	-flag x  // non-boolean flags only
	*/
	Println("*pointer: ", *pointer)
	Println("variable: ", variable)
	Println("bool: ", *boo)
	Println("string: ", *str)

	//non-flag-argument
	Println("NArg: ", flag.NArg())
	Println("Args: ", flag.Args())
	Println("Arg(0): ", flag.Arg(0))
}
