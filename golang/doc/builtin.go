package main

func main() {

	/*
	 * append
	 */
	 slice := []byte{'h','e', 'l', 'l', 'o',' ','w', 'o', 'r', 'l', 'd',','}

	 // append(slice, elem1, elem2)
	 slice = append(slice, 'x', 'u', 'e', ' ')
	 // append(slice, slice2...)
	 slice = append(slice, []byte{'k', 'u', 'n', ' '}...)
	 // append([]byte, string...)
	 str := "lun\n"
	 slice = append(slice, str...)

	 println("append:\t", string(slice))

	/*
	 * cap && len
	 */
	 // array, &array: cap 10, len 10
	 array := [10]int{0, 1, 2, 3, 4}
	 println("cap(array) = ", cap(array), "\t\tlen(array) = ", len(array))
	 p_array := &array
	 println("cap(p_array) = ", cap(p_array), "\tlen(p_array) = ", len(p_array))
	 // slice: cap 10, len 5,[0-4]; slice[>=5] error?
	 slice1 := array[0:5]
	 slice1[0] = 10
	 slice1[1] = 10
	 slice1[2] = 10
	 slice1[3] = 10
	 slice1[4] = 10
	 //slice1[5] = 10	// error?
	 println("cap(slice1) = ", cap(slice1), "\tlen(slice1) = ", len(slice1))
	 // chan: cap 10, len (3-1 = 2) 
	 channel := make(chan int, 10)
	 channel <- 0
	 channel <- 1
	 channel <- 2
	 println(<-channel)		// 3-1=2
	 println("cap(channel) = ", cap(channel), "\tlen(channel) = ", len(channel))
	 // map: no cap(), can 'delete'
	 map1 := map[string]int{"zero": 0, "one":1, "two": 2, "three":3, "four":4}
	 delete(map1, "zero")
	 println("\t\t\tlen(map1) = ", len(map1), " after 'delete'\n")
	 // string: no cap()
	 str1 := "hello"
	 println("\t\t\tlen(str1) = ", len(str1))
	 println("\n")

	/*
	 * make
	 */
	 // make(slice, len, cap)
	 slice2 := make([]int, 5, 10)
	 //slice2 := make([]int, 5)	 // len 5, cap 5
	 println("make cap(slice2) = ", cap(slice2), "\tlen(slice2) = ", len(slice2))

	 // make(map, [ignore]); len 0.
	 map2 := make(map[string]int)	// map2 := map[string]int{}
	 println("make\t\t\tlen(map2) = ", len(map2))

	 // make(chan Type, cap)
	 channel2 := make(chan int, 10)
	 println("make cap(channel2) = ", cap(channel2), "\tlen(channel2) = ", len(channel2))
	 channel2 <- 10
	 channel2 <- 11

	 /*
	  * close
	  */
	 // Shut down the channel after it is read to empty. After which, ok will be false
	 close(channel2)
	 ch, ok := <-channel2
	 println("\t//chan test0: ch ", ch, " ok ", ok)
	 ch, ok = <-channel2
	 println("\t//chan test1: ch ", ch, " ok ", ok, " after close()")
	 ch, ok = <-channel2	// channel is empty and shutted down. it will result in deadlock with out the 'close' before.
	 println("\t//chan test2: ch ", ch, " ok ", ok, " after close()")
}
