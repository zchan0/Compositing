 * CS604 Homework 3,
 * Get full specification on https://people.cs.clemson.edu/~dhouse/courses/404/hw/hw3/hw3.html
 *
 * use 'make all' to compile two programs
 * bg.jpg is example background image for compositing
 *
 * alphamask Program function:
 * - Read two associated color images and composite
 * - Display composited image
 * - Write composited image
 *
 * Usage:
 * - > composite A.img B.img // make A over B
 * - c/C to trigger composite
 * - w/W to save composited image, default name is "composite.png"
 *
 * Other:
 * - default position of A is (0, 0)
 * - composited image store in B.img
 * - for reuse, use imgs to store image names from command line, can modify some code to composite multiple images

 * composite Program function:
 * - Generate alpha mask for image
 * - Display generated image
 * - Save generated image
 *
 * Usage:
 * - alphamask input.img output.img
 *
 * author: Cencen Zheng 9/27/2016