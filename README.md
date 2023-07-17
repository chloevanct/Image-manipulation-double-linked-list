# Description

A chain of nodes is constructed from an input image and one may render the chain's pixel data back into an output PNG. 

The difficulty with the following function implementations arises from the specification restriction of using only pointer assignments, without the option to allocate or deallocate nodes.

FlipHorizontal rearranges the Node structure and internal pixel data to be flipped over a vertical axis.

RotateCCW rearranges the Node structure and internal pixel data to be rotated counter-clockwise.


