package main

import (
	"fmt"
)

func main() {
  var name string = "Roberto"
  if name == ""{
    fmt.Println("Error, no name input.")
  }else{
    fmt.Println("Hello", name, ", Welcome to the jungle")  
  }
}