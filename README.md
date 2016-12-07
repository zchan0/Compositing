 CS6040 Homework 3

> Get full specification on https://people.cs.clemson.edu/~dhouse/courses/404/hw/hw3/hw3.html
  
# Usage

``` 
make all // compile two programs
./composite A.img B.img // make A over B
./alphamask input.img [output.img]

// keyboard
c/C to trigger composite
w/W to save composited image, default name is "composite.png"
``` 
 
- default position of A is (0, 0)
- composited image store in B.img
- bg.jpg is example background image for compositing
- for reuse, use imgs to store image names from command line, can modify some code to composite multiple images
