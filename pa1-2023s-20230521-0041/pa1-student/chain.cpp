/**
 * File: chain.cpp
 * Description: student implementation for Chain functions
 *              for CPSC 221 2023S PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
 */

#include <math.h>

#include "chain.h"
#include <iostream>

/**
 * Constructs a Chain from an input image
 * Each Node will cover a nodedimension x nodedimension (in pixels)
 * region in the source image.
 * @param img the input image
 * @param nodedimension number of pixels along the width and height of the Block region
 * @pre input image's width and height are evenly divisible by nodedimension
 */
Chain::Chain(PNG &img, unsigned int nodedimension)
{
	// TODO
	Node *previous_node = NULL;
	length_ = 0;
	for (unsigned int y = 0; y <= img.height() - nodedimension; y += nodedimension)
	{
		for (unsigned int x = 0; x <= img.width() - nodedimension; x += nodedimension)
		{
			
			Block curr_block;
			curr_block.Build(img, x, y, nodedimension);

			// cout << "Block dimension: " << curr_block->Dimension() << endl;

			Node *curr_node = new Node;
			curr_node->data = curr_block;
			curr_node->prev = previous_node;

			if (previous_node != NULL)
			{
				curr_node->prev->next = curr_node;
			}

			if (x == 0 && y == 0)
			{
				NW = curr_node;
			}

			if (x == (img.width() - nodedimension) && y == (img.height() - nodedimension))
			{
				SE = curr_node;
			}

			length_++;
			previous_node = curr_node;
		}
	}
}

/**
 * Renders the Chain's pixel data into an output PNG.
 * The PNG should be sized according to the number of nodes
 * specified by the cols parameter.
 * It is possible that there may not be enough nodes to fill
 * the bottom row of the PNG. If this happens, just render
 * as many nodes as there are, from left to right, and leave
 * the rest of the row as opaque white pixels.
 * @pre this Chain is not empty
 * @param cols the number of Blocks to use for the width of the image
 */
PNG Chain::Render(unsigned int cols)
{
	unsigned int node_dim = this->NW->data.Dimension();
	unsigned int img_width = node_dim * cols;
	unsigned int img_height = ceil(double(length_) / double(cols)) * node_dim;

	unsigned int node_counter = 0;

	PNG img = PNG(img_width, img_height);

	Node *current_node = NW;

	for (unsigned int y = 0; y < img.height(); y += node_dim)
	{
		for (unsigned int x = 0; x < img.width() && node_counter < length_; x += node_dim)
		{
			current_node->data.Render(img, x, y);
			// cout << node_counter << endl;
			// cout << current_node->data.Dimension() << endl;
			current_node = current_node->next;
			node_counter++;
		}
	}

	// PNG(unsigned int width, unsigned int height)

	return img;
}

/**
 * Inserts a new Node containing ndata at the back of the Chain
 */
void Chain::InsertBack(const Block &ndata)
{

	Node *new_node = new Node(ndata);
	new_node->prev = this->SE->next;
	this->SE->next = new_node;
	new_node->next = NULL;
	SE = new_node->next;
}

/**
 * Reverses the structure of the list. This must be done by
 * pointer assignments. You may not deallocate or allocate any Nodes.
 * Example:
 *	before:	NW -> A <-> B <-> C <-> D <-> E <-> F <-> G <-> H <- SE
 *
 *  after:	NW -> H <-> G <-> F <-> E <-> D <-> C <-> B <-> A <- SE
 */
void Chain::Reverse()
{

	Node *current = NW;
	Node *prev_head = NW;
	Node *prev_tail = SE;
	Node *temp = NULL;

	while (current != NULL)
	{
		temp = current->prev;
		current->prev = current->next;
		current->next = temp;
		current = current->prev;
	}

	NW = prev_tail;
	SE = prev_head;
}

/**
 * Rearranges the Node structure and internal pixel data to be flipped over a vertical axis.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *  before, flipping with 3 columns (2 rows):
 *
 *		NW -> A> <-> B> <-> C> <->
 *            D> <-> E> <-> F> <- SE
 *
 *  after, visualized with 3 columns (2 rows):
 *
 *		NW -> <C <-> <B <-> <A <->
 *            <F <-> <E <-> <D <- SE
 *
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
 */
void Chain::FlipHorizontal(unsigned int cols)
{
	// complete your implementation below

	unsigned int rows = length_ / cols;

	unsigned int assigned_nodes = 0;
	Node *current = NULL;
	Node *next_row_node = NW; // starts at first node of the row
	Node *temp = NULL;
	Node *prev = NULL;

	cout << "starting the i loop" << endl;
	for (unsigned int i = 0; i < rows; i++) // keep track of pos among rows
	{
		current = next_row_node;
		cout << "i =" << i << endl;
		cout << "starting the j loop" << endl;
		for (unsigned int j = 0; j < cols; j++) // keep track of pos within row (column)
		{
			cout << "j =" << j << endl;

			if (j == (cols - 1)) // is this node at end of the row
			{
				cout << "found the last node of the row when j =" << j << endl;
				if (assigned_nodes == 0)
				{
					NW = current;
				}

				next_row_node = current->next;

				cout << "traversing back to reassign nodes" << j << endl;
				for (unsigned int z = 0; z < cols; z++)
				{
					if (z == 0 && prev != NULL)
					{
						prev->next = current;
					}

					// modifying current node
					temp = current->prev;
					current->next = temp;
					current->data.FlipHorizontal();
					current->prev = prev;

					// if last item in the reverse row, temporarily assign its next to null
					if (z == cols - 1)
					{
						current->next = NULL;
					}

					// incrementing accumulators
					prev = current;
					current = current->next;
					assigned_nodes++;

					// if prev is the first item in our row and is in the last row, make it tail
					if (i == (rows - 1) && z == (cols - 1))
					{
						SE = prev;
					}
				}
			}
			else // if this node is not at the end of the row
			{
				current = current->next;
			}
		}
	}
}

/**
 * Rearranges the Node structure and internal pixel data to be rotated counter-clockwise.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *	before, rotating with 3 columns (2 rows):
 *            ^     ^     ^
 *      NW -> A <-> B <-> C <->
 *            ^     ^     ^
 *            D <-> E <-> F <- SE
 *
 *  after, visualized with 2 columns (3 rows):
 *
 *      NW -> <C <-> <F <->
 *            <B <-> <E <->
 *            <A <-> <D <- SE
 *
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
 */
void Chain::RotateCCW(unsigned int cols)
{
	cout << "starting rotate" << endl;
	Node *next_node = NULL;
	// *next_node = *NW;
	cout << "starting rotate" << endl;
	Node *curr_node = NULL;
	Node *prev_node = NULL;
	Node *first_node = NULL;

	unsigned int nodes_left = length_;
	unsigned int num_nodes_to_get = length_ / cols;
	unsigned int assigned_nodes = 0;

	for (unsigned int i = cols; i > 0; i--)
	{
		next_node = NW;
		cout << "starting i loop at i=" << i << endl;
		for (unsigned int j = 0; j < nodes_left; j++)
		{
			cout << "starting j loop at j=" << j << endl;
			curr_node = next_node;
			next_node = curr_node->next;

			if ((j + 1) % i == 0)
			{
				cout << "dereferncing at j=" << j << endl;

				if (assigned_nodes == 0)
				{
					first_node = curr_node;
				}

				if (curr_node->prev != NULL)
				{
					cout << "back" << endl;
					curr_node->prev->next = curr_node->next;
				}

				if (curr_node->next != NULL)
				{
					cout << "next" << endl;
					curr_node->next->prev = curr_node->prev;
				}

				// modify this node
				// cout << "assigning current node's prev based on prev_node" << j << endl;
				curr_node->prev = prev_node;

				// cout << "rotating the node's data " << j << endl;
				curr_node->data.RotateCCW();

				if (prev_node != NULL)
				{
					// cout << "assigning prev's node next to be curr_node" << j << endl;
					prev_node->next = curr_node;
				}

				// cout << " assigning cur_node's next to be NULL" << j << endl;

				curr_node->next = NULL;
				// cout << "incrementing assigned_nodes " << j << endl;
				assigned_nodes++;
				cout << "assigned nodes: " << assigned_nodes << endl;
				cout << "length_ :" << length_ << endl;

				// cout << "assigning curr_node to prev_node " << j << endl;
				prev_node = curr_node;

				// cout << "modifying this node one more time 4x " << j << endl;
				if (assigned_nodes == length_)
				{
					// cout << "modifying this node one more time 5x " << j << endl;
					// this->SE = curr_node;

					this->SE = prev_node;
					// cout << "modifying this node one more time 6x " << j << endl;
					this->NW = first_node;
					cout << "terminating the loops" << j << endl;
					cout << "assigned nodes: " << assigned_nodes << endl;
					return;
				}
			}
		}
		nodes_left -= num_nodes_to_get;
		// cout << "modifying this node one more time 9x " << endl;
		next_node = this->NW;
	}

	// complete your implementation below
	cout << "rotation function finished" << endl;
}

/**
 * Destroys all dynamically allocated memory associated with
 * this Chain object. Called by destructor and operator=.
 * You must complete its implementation for PA1.
 */
void Chain::Clear()
{

	Node * current = NW;
	
	while (current) {
		Node * next_node = current->next;
		delete current;
		current = next_node;
	}

	NW = NULL;
	SE = NULL;
	length_ = 0;


	// cout << "calling clear" << endl;
	// Node *current_node = NULL;
	// Node *next_node = NW;

	// cout << "loop starting" << endl;
	// for (unsigned int i = 0; i < length_; i++)
	// {
	// 	// initialize current node
	// 	current_node = next_node;
	// 	cout << "current_node = next_node passed" << endl;

	// 	// preserves connection to the next node
	// 	next_node = next_node->next;

	// 	// delete the block data
	// 	delete current_node;

	// 	// set the next node
	// 	current_node = next_node;

	// 	// delete &(current_node->data);

	// 	// delete current_node;

	// 	// set the next node
	// }

	// cout << "loop finished" << endl;

	// current_node = NULL;
	// delete next_node;
	// next_node = NULL;

	// NW = NULL;
	// SE = NULL;
	// length_ = 0;

	// cout << "finisihed clear" << endl;
}

/**
 * Copies the parameter other Chain into the current Chain.
 * Does not free any memory. Called by copy constructor and
 * operator=.
 * You must complete its implementation for PA1.
 * @param other The Chain to be copied.
 */
void Chain::Copy(const Chain &other)
{
	//Clear();
	Node *other_node = other.NW;
	Node *prev_node = NULL;

	// while (other_node != NULL)
	for (unsigned int i = 0; i < other.length_; i++)
	{
		Node *new_node = new Node(other_node->data);

		if (prev_node != NULL)
		{
			prev_node->next = new_node;
			new_node->prev = prev_node;
		}
		else
		{
			NW = new_node;
		}
		
		prev_node = new_node;
		other_node = other_node->next;
	}

	SE = prev_node;
	length_ = other.length_;
}

/**
 * If you have declared any private helper functions in chain_private.h,
 * add your completed implementations below.
 */
