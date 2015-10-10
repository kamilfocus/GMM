#include "mixture_of_gaussians.hpp"
#include "tools.hpp"
#include "Initializator.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

MixtureOfGaussians::MixtureOfGaussians(int k, double alpha, double bg_classifier, int learning_frames_num)
{
    this->width = NONE;
    this->height = NONE;
    this->clusters_num = k;
    this->alpha = alpha;
    this->bg_classifier = bg_classifier;
    is_initialized = false;
    pixels = NULL;
    this->learning_frames_num = learning_frames_num;
    Initializator::set_learning_frames_num(learning_frames_num);
    Initializator::set_gaussian_num(k);
}


MixtureOfGaussians::~MixtureOfGaussians()
{
	for(int i=0; i < height; i++)
		delete [] pixels[i];
	delete [] pixels;

}

uchar ** MixtureOfGaussians::generate_inital_means(bool deterministic)
{
	uchar** new_gaussian_means = new uchar* [clusters_num];
	for(int i=0; i<clusters_num; i++)
		new_gaussian_means[i] = new uchar [RGB_COMPONENTS_NUM];

	if(deterministic)
    {
		uchar mean_value = 30;
		uchar step = (255 - 30)/clusters_num;
		for(int i=0; i<clusters_num; i++)
			for(int j=0; j<RGB_COMPONENTS_NUM; j++)
				new_gaussian_means[i][j] = mean_value + i*step;
    }
    else
    {
    	srand(time(NULL));
		for(int i=0; i<clusters_num; i++)
			for(int j=0; j<RGB_COMPONENTS_NUM; j++)
				new_gaussian_means[i][j] = rand()%0xff + 1;		// 1 - 255 range

    }

	/*
	cout << "init means:" << endl;
	for(int i=0; i<clusters_num; i++){
		cout << "gaussian: " << i << " ";
		for(int j=0; j<RGB_COMPONENTS_NUM; j++){
			cout << (int)new_gaussian_means[i][j] << " ";
		}
		cout << endl;
	}
	*/
    return new_gaussian_means;
}

void MixtureOfGaussians::update(const Mat & input_frame, Mat & output_frame)
{

    if(!is_initialized)
    {
        height = input_frame.rows;
        width = input_frame.cols;
        is_initialized = initialize_gaussians(input_frame);
    	output_frame = input_frame;
        return;
    }

	double rgb[RGB_COMPONENTS_NUM];
	const uchar * input_pixel_ptr;
	uchar * result_pixel_ptr;
	uchar mask;
	for(int row = 0; row < height; ++row)
	{
		input_pixel_ptr = input_frame.ptr(row);
	    result_pixel_ptr = output_frame.ptr(row);
	    for(int col = 0; col < width; ++col)
	    {
	    	//RGB reverted order
	        rgb[2] = (double) *input_pixel_ptr++;
	        rgb[1] = (double) *input_pixel_ptr++;
	        rgb[0] = (double) *input_pixel_ptr++;
	        mask = (pixels[row][col].is_foreground(rgb)) ? WHITE : BLACK;

	        //cout << row << "x" << col << ": " << (int)mask << endl;

	        for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
	        	*result_pixel_ptr++ = mask;
	    }

	}
/*
//    cout << "init complete" << endl;
    for(int row = 0; row < height; ++row)
    {
        const uchar *input_pixel_ptr = input_frame.ptr(row);
        uchar *result_pixel_ptr = output_frame.ptr(row);
        for(int col = 0; col < width; ++col)
        {
            //points to each pixel B,G,R value in turn assuming a CV_8UC3 color image
            paint_foreground(input_pixel_ptr, result_pixel_ptr, row, col);
        }
    }
    return;
*/
}

void MixtureOfGaussians::paint_foreground(const uchar * input_pixel_ptr,
                                          uchar * result_pixel_ptr,
                                          int row, int col)
{
	static int cnt = 0;
    double rgb[RGB_COMPONENTS_NUM];
    rgb[2] = (double) *input_pixel_ptr++;
    rgb[1] = (double) *input_pixel_ptr++;
    rgb[0] = (double) *input_pixel_ptr++;

    if(pixels[row][col].is_foreground(rgb))    //Foreground
    {
        for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
           *result_pixel_ptr++ = WHITE;
        cnt++;
    }
    else
    {
        for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
            *result_pixel_ptr++ = BLACK;
    }

    // temp
    if(row==0 && col==0)
    {
    	cout << double(cnt)/(height*width) << endl;
    	cnt = 0;
    }

}

void MixtureOfGaussians::initialize_gaussians(const Mat & input_frame, Mat & result_frame)
{
    height = input_frame.rows;
    width = input_frame.cols;
    cout << height << "x" << width << endl;

    uchar ** new_gaussian_means;
    new_gaussian_means = generate_inital_means(false);

    pixels = new Pixel*[height];
    for(int i=0; i < height; ++i)
    {
        pixels[i] = new Pixel[width];
    }

    for(int i=0; i < height; ++i)
    {
        for(int j=0; j < width; ++j)
            pixels[i][j].init(clusters_num, alpha, new_gaussian_means);
    }

    for(int i=0; i<clusters_num; i++)
    {
    	delete [] new_gaussian_means[i];
    }
    delete [] new_gaussian_means;

    //Allocate memory
    double **gaussian_means;
    double *weight, *standard_deviation;
    int * cluster_elements;
    int ** pixel_cluster_group;

    gaussian_means = new double*[clusters_num];
    weight = new double[clusters_num];
    standard_deviation = new double[clusters_num];
    cluster_elements = new int[clusters_num];
    pixel_cluster_group = new int*[height];
    for(int i = 0; i < clusters_num; ++i)
    {
        cluster_elements[i] = 0;
        weight[i] = 0.0;
        standard_deviation[i] = 0.0;
        gaussian_means[i] = new double[RGB_COMPONENTS_NUM];
        for(int j=0; j < RGB_COMPONENTS_NUM; ++j)
            gaussian_means[i][j] = 0.0;
    }
    //Precount means and weights
    int total_pixels_num = 0;
    double rgb[RGB_COMPONENTS_NUM];
    double temp_gaussian_means[RGB_COMPONENTS_NUM];
    for(int row = 0; row < height; ++row)
    {
        pixel_cluster_group[row] = new int[width];
        const uchar *input_pixel_ptr = input_frame.ptr(row);
        uchar *result_pixel_ptr = result_frame.ptr(row);

        for(int col = 0; col < width; ++col)
        {
            total_pixels_num++;

            //RGB reverted order
            rgb[2] = (double) *input_pixel_ptr++;
            rgb[1] = (double) *input_pixel_ptr++;
            rgb[0] = (double) *input_pixel_ptr++;
            pixels[row][col].get_rgb_mean(0, temp_gaussian_means);
            double cluster_value;
            int best_cluster_num = 0;
            double best_cluster_value = malahidan_distance(rgb, temp_gaussian_means, RGB_COMPONENTS_NUM);
            for(int gaussian_num = 1; gaussian_num < clusters_num; ++gaussian_num)
            {
                pixels[row][col].get_rgb_mean(gaussian_num, temp_gaussian_means);
                cluster_value = malahidan_distance(rgb, temp_gaussian_means, RGB_COMPONENTS_NUM);
                if(cluster_value < best_cluster_value)
                {
                    best_cluster_value = cluster_value;
                    best_cluster_num = gaussian_num;
                }
            }

            pixel_cluster_group[row][col] = best_cluster_num;
            cluster_elements[best_cluster_num]++;
            for(int rgb_num = 0; rgb_num < RGB_COMPONENTS_NUM; ++rgb_num)
            {
                gaussian_means[best_cluster_num][rgb_num] += rgb[rgb_num];
            }

            //Paint Black first result frame
            for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
                *result_pixel_ptr++ = BLACK;

        }
    }

    //Count final weights and means
    for(int i = 0; i < clusters_num; ++i)
    {
        weight[i] = cluster_elements[i]/((double) total_pixels_num);
        cout << "weight " << i << ": " << weight[i] << endl;
        cout << "Gaussian means " << i << ":";
        for(int rgb_num = 0; rgb_num < RGB_COMPONENTS_NUM; ++rgb_num)
        {
            gaussian_means[i][rgb_num] /= cluster_elements[i];
            cout << gaussian_means[i][rgb_num] << " ";
        }
        cout << endl;
    }

    //Precount standard deviation
    for(int row = 0; row < height; ++row)
    {
        const uchar *input_pixel_ptr = input_frame.ptr(row);
        for(int col = 0; col < width; ++col)
        {
            rgb[2] = (double) *input_pixel_ptr++;
            rgb[1] = (double) *input_pixel_ptr++;
            rgb[0] = (double) *input_pixel_ptr++;

            int pixel_cluster = pixel_cluster_group[row][col];
            double rgb_mean_diff = malahidan_distance(rgb, gaussian_means[pixel_cluster], RGB_COMPONENTS_NUM);
            standard_deviation[pixel_cluster] += rgb_mean_diff;
        }
    }

    double initial_variance = 0;
    //Count final standard deviation
    for(int i = 0; i < clusters_num; ++i)
    {
        standard_deviation[i] /= (double) cluster_elements[i];
        standard_deviation[i] /= (double) RGB_COMPONENTS_NUM;

        //temp
        //standard_deviation[i] /= 10;

        if(i == 0 || initial_variance < standard_deviation[i])
        	initial_variance = standard_deviation[i];

        standard_deviation[i] = sqrt(standard_deviation[i]);
        standard_deviation[i] /= 10;
        cout << "standard deviation " << i << ": " << standard_deviation[i] << " ";
    }

    //Gaussian::set_initial_variance(initial_variance);
    Gaussian::get_initial_variance() = initial_variance;

    cout << endl;

    Pixel::set_T(bg_classifier);

    pixels[0][0].frame_init(weight, gaussian_means, standard_deviation);
    pixels[0][0].sort(bg_classifier);

    //Initialize gaussians with generated value and free memory
    for(int row = 0; row < height; ++row)
    {
        delete [] pixel_cluster_group[row];
        for(int col = 0; col < width; ++col)
        {
        	if(row == 0 && col == 0)
        		continue;
            pixels[row][col] = pixels[0][0];
        }
    }

    delete [] pixel_cluster_group;
    delete [] cluster_elements;
    delete [] weight;
    delete [] standard_deviation;
    for(int i = 0; i < clusters_num; ++i)
    {
        delete [] gaussian_means[i];
    }
    delete [] gaussian_means;
}


void MixtureOfGaussians::sort()
{
    for(int row = 0; row < height; ++row)
    {
        for(int col = 0; col < width; ++col)
        {
            pixels[row][col].sort(bg_classifier);
        }
    }
}

void MixtureOfGaussians::print_parameters(int row, int col, int gaussian_num)
{
    if(row == -1 || col == -1)
    {
        for(int row = 0; row < height; ++row)
        {
            for(int col = 0; col < width; ++col)
            {
                cout<<"["<<row<<"]"<<"["<<col<<"]: ";
                pixels[row][col].print(gaussian_num);
                cout<<endl;
            }
        }
    }
    else
    {
        if(row < 0 && row >= height)
        {
            cout<<"Row out of bounds: "<<row<<endl;
            return;
        }
        if(col < 0 && col >= width)
        {
            cout<<"Col out of bounds: "<<col<<endl;
            return;
        }
        cout<<"["<<row<<"]"<<"["<<col<<"]: ";
        pixels[row][col].print(gaussian_num);
        cout<<endl;
    }
}

void MixtureOfGaussians::simple_inital(const Mat & input_frame)
{
    height = input_frame.rows;
    width = input_frame.cols;

	double** new_gaussian_means = new double* [clusters_num];
	double * new_weight = new double [clusters_num];
	double * new_deviation = new double[clusters_num];

	for(int i=0; i<clusters_num; i++)
		new_gaussian_means[i] = new double [RGB_COMPONENTS_NUM];

	double mean_value = 30;
	double step = (255 - 30)/clusters_num;
	for(int i=0; i<clusters_num; i++)
	{
		for(int j=0; j<RGB_COMPONENTS_NUM; j++)
			new_gaussian_means[i][j] = mean_value + i*step;
		new_weight[i] = 1.0/clusters_num;
		new_deviation[i] = INITIAL_DEVIATION;
	}
    Pixel::set_T(bg_classifier);
    pixels = new Pixel*[height];
    for(int i=0; i < height; ++i)
    {
        pixels[i] = new Pixel[width];
    }

    for(int i=0; i < height; ++i)
    {
        for(int j=0; j < width; ++j)
        {
            pixels[i][j].init(clusters_num, alpha);
            pixels[i][j].frame_init(new_weight, new_gaussian_means, new_deviation);
            pixels[i][j].sort(bg_classifier);
        }
    }

    delete [] new_weight;
}

void MixtureOfGaussians::foreground_detection(const Mat & input_frame, Mat & result_frame)
{
	double rgb[RGB_COMPONENTS_NUM];
	const uchar * input_pixel_ptr;
	uchar * result_pixel_ptr;
	uchar mask;
	for(int row = 0; row < height; ++row)
	{
		input_pixel_ptr = input_frame.ptr(row);
	    result_pixel_ptr = result_frame.ptr(row);
	    for(int col = 0; col < width; ++col)
	    {
	    	//RGB reverted order
	        rgb[2] = (double) *input_pixel_ptr++;
	        rgb[1] = (double) *input_pixel_ptr++;
	        rgb[0] = (double) *input_pixel_ptr++;
	        mask = (pixels[row][col].is_foreground(rgb)) ? WHITE : BLACK;

	        //cout << row << "x" << col << ": " << (int)mask << endl;

	        for(int i = 0; i < RGB_COMPONENTS_NUM; ++i)
	        	*result_pixel_ptr++ = mask;
	    }

	}
}

bool MixtureOfGaussians::initialize_gaussians(const Mat & input_frame)
{
	static int current_frame = 0;
	static double ** new_gaussian_means = NULL;
	static Initializator ** gaussian_initializator = NULL;

	double rgb[RGB_COMPONENTS_NUM];
	const uchar *input_pixel_ptr;

	if(current_frame == 0)
	{
	    height = input_frame.rows;
	    width = input_frame.cols;
//	    cout << height << "x" << width << endl;

	    uchar ** temp_gaussian_means = generate_inital_means(true);
	    new_gaussian_means = new double*[clusters_num];
	    for(int i=0; i<clusters_num; i++)
	    {
	    	new_gaussian_means[i] = new double[RGB_COMPONENTS_NUM];
	    	for(int j=0; j<RGB_COMPONENTS_NUM; j++)
	    		new_gaussian_means[i][j] = (double)temp_gaussian_means[i][j];
	    	delete [] temp_gaussian_means[i];
	    }
	    delete [] temp_gaussian_means;

	    Initializator::set_init_gaussian_means(new_gaussian_means);
	    gaussian_initializator = new Initializator*[height];
	    for(int i=0; i<height; i++)
	    	gaussian_initializator[i] = new Initializator[width];
	}

    for(int row = 0; row < height; ++row)
    {
        input_pixel_ptr = input_frame.ptr(row);
        for(int col = 0; col < width; ++col)
        {
            //RGB reverted order
            rgb[2] = (double) *input_pixel_ptr++;
            rgb[1] = (double) *input_pixel_ptr++;
            rgb[0] = (double) *input_pixel_ptr++;
//            if(current_frame == learning_frames_num-1)
//            	cout<<"["<<row<<"]"<<"["<<col<<"]: ";
            gaussian_initializator[row][col].init(rgb, current_frame);

        }
    }

    current_frame++;
    if(current_frame<learning_frames_num)
    	return false;

    Pixel::set_T(bg_classifier);

    double * temp_weight;
    double * temp_deviation;
    double ** temp_gaussian_means;

    pixels = new Pixel*[height];
    for(int i=0; i < height; ++i)
    {
        pixels[i] = new Pixel[width];
    }

    for(int i=0; i < height; ++i)
    {
        for(int j=0; j < width; ++j)
        {
        	gaussian_initializator[i][j].get_gaussian_param(&temp_weight, &temp_deviation, &temp_gaussian_means);
            pixels[i][j].init(clusters_num, alpha);
            pixels[i][j].frame_init(temp_weight, temp_gaussian_means, temp_deviation);
            pixels[i][j].sort(bg_classifier);
        }
        delete [] gaussian_initializator[i];
    }
    delete [] gaussian_initializator;



    for(int i=0; i<clusters_num; i++)
    {
    	delete [] new_gaussian_means[i];
    }
    delete [] new_gaussian_means;
    current_frame = 0;

	return true;
}


