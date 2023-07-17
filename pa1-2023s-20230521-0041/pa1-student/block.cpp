/**
 * File: block.cpp
 * Description: student implementation for Block functions
 *              for CPSC 221 2023S PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
 */

#include "block.h"

/**
 * Creates a square Block that is dimension x dimension pixels in size,
 * starting at (x, y) on img. Assumes that the requested
 * Block fits on the image (inputs are feasible)
 */
void Block::Build(PNG &img, unsigned int x, unsigned int y, unsigned int dimension)
{
	for (unsigned int x_coord = x; x_coord < dimension + x; x_coord++)
	{
		vector<HSLAPixel> x_vector;
		for (unsigned int y_coord = y; y_coord < dimension + y; y_coord++)
		{
			x_vector.emplace_back(*img.getPixel(x_coord, y_coord));
		}
		data.emplace_back(x_vector);
	}
}

/**
 * Renders this Block onto img with its upper-left corner at (x, y).
 * Assumes block fits on the image.
 */
// TODO
void Block::Render(PNG &img, unsigned int x, unsigned int y) const
{
	// vector<HSLAPixel> current_vector;
	// HSLAPixel *block_pixel;
	// HSLAPixel *img_pixel;

	for (unsigned int x_coord = 0; x_coord < Dimension(); x_coord++)
	{
		// current_vector = data[x_coord];
		for (unsigned int y_coord = 0; y_coord < Dimension(); y_coord++)
		{
			// retrieve this block's pixel
			// *block_pixel = current_vector[y_coord];
			// retrieve pointer to the image's pixel
			*(img.getPixel(x_coord + x, y_coord + y)) = data[x_coord][y_coord];
			// replace the image's pixel with the block's pixel
			// *img_pixel = *block_pixel;
		}
	}
}

/**
 * Flips the Block's pixel data horizontally by a simple reflection
 * across the vertical axis through its middle.
 */
void Block::FlipHorizontal()
{
	vector<vector<HSLAPixel>> reverse_vector;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		reverse_vector.insert(reverse_vector.begin(), data[i]);
	}

	data = reverse_vector;
}

/**
 * Rotates the Block's pixel data 90 degrees counter-clockwise.
 */
void Block::RotateCCW()
{

	// initialize the new empty rotated vector of vectors (x coordinate)
	vector<vector<HSLAPixel>> rotated_x_vector;

	// add Dimension() empty y coordinates to the rotated_x_vector
	for (unsigned int x = 0; x < Dimension(); x++)
	{
		vector<HSLAPixel> temp;
		rotated_x_vector.push_back(temp);
	}

	// start populating the rotated vector with data coordinates
	for (unsigned int x = 0; x < Dimension(); x++)
	{
		for (unsigned int y = 0; y < Dimension(); y++)
		{
			(rotated_x_vector)[y].insert((rotated_x_vector)[y].begin(), data[x][y]);

			// (*rotated_x_vector)[y].push_back(data[x][y]);
		}
	}

	// data.clear();

	data = rotated_x_vector;
	// (*rotated_x_vector).clear();
}

/**
 * Returns the dimension of the block (its width or height).
 */
unsigned int Block::Dimension() const
{
	// replace the statement below with your implementation
	return data.size();
	// valgrind --leak-check=full ./pa1
	//==105666==    definitely lost: 6,480 bytes in 270 blocks
	// ==105666==    indirectly lost: 1,766,400 bytes in 1,230 blocks
}