/*******************************************************************************************************************//**
 * @file ComputerVision_HW1.cpp
 * @brief A rudementary implementation of MS Paint
 * @author Shaun Sartin
 **********************************************************************************************************************/

// include necessary dependencies
#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"

/*
*/
cv::Mat compress_img(cv::Mat img_in, int window_length)
{
	int row_count = img_in.rows;
	int col_count = img_in.cols;
	cv::Mat img_out = cv::Mat(row_count - (window_length - 1), col_count - (window_length - 1));

	if((row_count <= window_length) || (col_count <= window_length))
	{
		std::cout << "No work to do. Returning input image." << std::endl;
		return img_in;
	}
	
	for(int i = 0; i < row_count - (window_length - 1); i++)
	{
		for(int j = 0; j < col_count - (window_length - 1); j++)
		{
			int red_vals[window_length * window_length];
		}
	}

	return img_out

}

/*******************************************************************************************************************//**
 * @brief program entry point
 * @param[in] argc number of command line arguments
 * @param[in] argv string array of command line arguments
 * @return return code (0 for normal termination)
 * @author Shaun Sartin
 **********************************************************************************************************************/
int main(int argc, char **argv)
{
	std::string inputFileName;

	if(argc > 1)
	{
        	inputFileName = argv[1];
    	}
    	else
    	{
        	std::cout << "Error: You must pass argument for an image filename" << std::endl;
        	return 0;
    	}

    	// check for file error
    	if(!imgCommands.image.data)
	{
		std::cout << "Error while opening file " << inputFileName << std::endl;
		return 0;
	}


	
}
