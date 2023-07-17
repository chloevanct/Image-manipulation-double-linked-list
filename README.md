# Description

A chain of nodes is constructed from an input image and one may render the chain's pixel data back into an output PNG. 

The difficulty with the following function implementations arises from the specification restriction of using only pointer assignments, without the option to allocate or deallocate nodes.

FlipHorizontal rearranges the Node structure and internal pixel data to be flipped over a vertical axis.
- Example, length = 6:
  before, flipping with 3 columns (2 rows):

NW -> A> <-> B> <-> C> <->
      D> <-> E> <-> F> <- SE

  after, visualized with 3 columns (2 rows):

NW -> <C <-> <B <-> <A <->
      <F <-> <E <-> <D <- SE

RotateCCW rearranges the Node structure and internal pixel data to be rotated counter-clockwise.

- Example, length = 6:
  before, rotating with 3 columns (2 rows):
             ^     ^     ^
       NW -> A <-> B <-> C <->
             ^     ^     ^
             D <-> E <-> F <- SE
 
   after, visualized with 2 columns (3 rows):
       NW -> <C <-> <F <->
             <B <-> <E <->
             <A <-> <D <- SE


