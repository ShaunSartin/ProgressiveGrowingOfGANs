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
float vector_avg(std::vector<int> &vals)
{
	int sum = 0;
	for(int i = 0; i < vals.size(); i++)
	{
		sum = sum + vals.at(i);
	}
	return sum / vals.size();
}

/*
*/
float vector_median(std::vector<int> &vals)
{
	std::sort(vals.begin(), vals.end());
	float result;

	if(vals.size() % 2 == 0)
	{
		int median_low_index = (vals.size() / 2) - 1;
		int median_high_index = vals.size() / 2;
		float median_low = vals.at(median_low_index);
		float median_high = vals.at(median_high_index);
		result = (median_low + median_high) / 2;
	}
	else
	{
		int median_index = (int) (vals.size() / 2);
		result = vals.at(median_index);
	}

	return result;
}


/*
*/
cv::Mat nonoverlap_compress_img(cv::Mat img_in, int window_length)
{
	int row_count = img_in.rows;
	int col_count = img_in.cols;
	cv::Mat img_out(row_count / window_length, col_count /  window_length, CV_8UC3);

	if((row_count <= window_length) || (col_count <= window_length))
	{
		std::cout << "No work to do. Returning input image." << std::endl;
		return img_in;
	}
	
	int output_row = 0;
	for(int row = 0; row < row_count - (window_length - 1); row = row + window_length)
	{
		int output_col = 0;
		for(int col = 0; col < col_count - (window_length - 1); col = col + window_length)
		{
			std::vector<int> red_vals;
			std::vector<int> green_vals;
			std::vector<int> blue_vals;
			
			for(int r_add = 0; r_add < window_length; r_add++)
			{
				for(int c_add = 0; c_add < window_length; c_add++)				
				{
					// NOTE: OpenCV follows a BGR color scheme
					cv::Vec3b color = img_in.at<cv::Vec3b>(cv::Point(row + r_add, col + c_add));
					red_vals.push_back(color.val[2]);
					green_vals.push_back(color.val[1]);
					blue_vals.push_back(color.val[0]);
				}
			}
			float avg_red = vector_avg(red_vals);
			float avg_green = vector_avg(green_vals);
			float avg_blue = vector_avg(blue_vals);

			img_out.at<cv::Vec3b>(cv::Point(output_row,output_col)) = cv::Vec3b(avg_blue, avg_green, avg_red);	
			
			red_vals.clear();
			green_vals.clear();
			blue_vals.clear();
			output_col++;
		}

		if(row % 100  == 0)
		{
			std::cout << row << "/" << row_count - (window_length - 1) << std::endl;
		}
		output_row++;
	}
	
	return img_out;

}

/*
*/
cv::Mat overlap_compress_img(cv::Mat img_in, int window_length)
{
	int row_count = img_in.rows;
	int col_count = img_in.cols;
	cv::Mat img_out(row_count - (window_length - 1), col_count - (window_length - 1), CV_8UC3);

	if((row_count <= window_length) || (col_count <= window_length))
	{
		std::cout << "No work to do. Returning input image." << std::endl;
		return img_in;
	}
	
	for(int row = 0; row < row_count - (window_length - 1); row++)
	{
		for(int col = 0; col < col_count - (window_length - 1); col++)
		{
			std::vector<int> red_vals;
			std::vector<int> green_vals;
			std::vector<int> blue_vals;
			
			for(int r_add = 0; r_add < window_length; r_add++)
			{
				for(int c_add = 0; c_add < window_length; c_add++)				
				{
					// NOTE: OpenCV follows a BGR color scheme
					cv::Vec3b color = img_in.at<cv::Vec3b>(cv::Point(row + r_add, col + c_add));
					red_vals.push_back(color.val[2]);
					green_vals.push_back(color.val[1]);
					blue_vals.push_back(color.val[0]);
				}
			}
			float avg_red = vector_median(red_vals);
			float avg_green = vector_median(green_vals);
			float avg_blue = vector_median(blue_vals);

			img_out.at<cv::Vec3b>(cv::Point(row,col)) = cv::Vec3b(avg_blue, avg_green, avg_red);	
			
			red_vals.clear();
			green_vals.clear();
			blue_vals.clear();
		}

		if(row % 1  == 0)
		{
			float completion = row;
			std::cout << completion << std::endl;
		}
	}

	return img_out;

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
	int windowSize;


	if(argc > 2)
	{
        	inputFileName = argv[1];
		windowSize = atoi(argv[2]);
    	}
    	else
    	{
        	std::cout << "Usage: ./LossyImageCompression <PATH_TO_IMAGE> <WINDOW_SIZE>" << std::endl;
        	return 0;
    	}

	cv::Mat image;
	image = cv::imread(inputFileName, CV_LOAD_IMAGE_COLOR);
	cv::Mat output_image((image.rows / windowSize), (image.cols / windowSize), CV_8UC3);    	


	// check for file error
    	if(!image.data)
	{
		std::cout << "Error while opening file " << inputFileName << std::endl;
		return 0;
	}

	std::cout << "Now Compressing Image: " << inputFileName << std::endl;
	while (output_image.rows >= 4)
	{
		output_image = nonoverlap_compress_img(image, windowSize);
		std::string outputFileName = "img" + std::to_string(output_image.rows) + "x" + std::to_string(output_image.cols) + ".jpg";
		cv::imwrite(outputFileName, output_image);

		//Reassign image to be the current output and output_image to be the next output
		cv::resize(image, image, cv::Size(output_image.rows, output_image.cols));
		output_image.create((output_image.rows / 2), (output_image.cols / 2), image.depth());	
	}	
}
