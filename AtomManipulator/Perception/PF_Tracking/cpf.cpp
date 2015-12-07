#include "cpf.h"
//#include "../Global_constant.h"
//#include "imageIter.h"
#include <set>
//constructor
color_based_PF::color_based_PF()
{
	target_img = NULL;
	for(int i =0; i < BIN_NUM*BIN_NUM*BIN_NUM; i ++)
		target_model[i] = 0;
	particle_set = new sample_cpf[PARTICLE_NUM];
	buffer_particle_set = new sample_cpf[PARTICLE_NUM];
	estimate_available = false;
	rd.seed();
}
//deconstructor
color_based_PF::~color_based_PF()
{
	delete[] particle_set;
	delete[] buffer_particle_set;
}

/*
 output the bhattacharyya coefficient
*/
float color_based_PF::Bhattacharyya_coefficient(const float model1[], const float model2[])
{
	int i =0;
	float coefficient = 0;
	for(i =0; i< BIN_LENGTH; i++){
		coefficient += sqrt( model1[i]*model2[i]);
	}

	if(coefficient > 1){ //debug use
		printf(" theres error abound bh coefficient.. %f ", coefficient);
		system("pause");
		return -1;
	}
	else
		return coefficient;
}

/*
 build the histogram with the current image
*/
void color_based_PF::build_color_histogram(const sample_cpf &sample, float *histogram, IplImage *pImg)
{
	CvPoint bbox[2]; // the upper left and lower right point in image
	int bin_num, i , j;
	float dis = 0, total_weight = 0;
	const float scale_dis = sqrt( sample.hx*sample.hx + sample.hy*sample.hy);
	RgbImage img(pImg);

	bbox[0].x = sample.x - sample.hx;
	bbox[0].y = sample.y - sample.hy;
	bbox[1].x = sample.x + sample.hx;
	bbox[1].y = sample.y + sample.hy;

	bbox[0].x = bbox[0].x <0 ? 0 : bbox[0].x>= Image_width? Image_width -1: bbox[0].x;
	bbox[1].x = bbox[1].x <0 ? 0 : bbox[1].x>= Image_width? Image_width -1: bbox[1].x;
	bbox[0].y = bbox[0].y <0 ? 0 : bbox[0].y>= Image_height? Image_height -1: bbox[0].y;
	bbox[1].y = bbox[0].y <0 ? 0 : bbox[1].y>= Image_height? Image_height -1: bbox[1].y;

	for( i = 0; i < BIN_LENGTH; i++){
		histogram[i] = 0;
	}
	for( i = bbox[0].y; i <= bbox[1].y; i++){
		for(j = bbox[0].x; j <= bbox[1].x; j++){
			bin_num = (img[i][j].r)/BIN_WIDTH;
			bin_num += ((img[i][j].g)/BIN_WIDTH)*BIN_NUM;
			bin_num += ((img[i][j].b)/BIN_WIDTH)*BIN_NUM*BIN_NUM;
			dis = sqrt( (j - sample.x)*(j - sample.x) + (i - sample.y)*(i - sample.y))/ scale_dis;
			if(bin_num< BIN_LENGTH){
				histogram[bin_num] += kernel_weight(dis);
				total_weight += kernel_weight(dis);
			}
			else{
				printf(" bin num %d error! \n", bin_num);
				system("pause");
			}
		}
	}
	if(total_weight ==0){
		printf(" total_weight 0 error! \n");
		//system("pause");
	}
	else{
		for( i = 0; i < BIN_LENGTH; i++){
			histogram[i] = histogram[i] / total_weight;
		}
	}

}

/*
estimate the new pos of the target
*/
void color_based_PF::estimate(const IplImage *img)
{
	int i =0;
	//IplImage* debug_img = cvCloneImage(img);
	estimation.x = 0;
	estimation.y = 0;
	estimation.hx = 0;
	estimation.hy = 0;
	for( i = 0; i < PARTICLE_NUM; i++){
		estimation.x += particle_set[i].weight*particle_set[i].x;	
		estimation.y += particle_set[i].weight*particle_set[i].y;	
		estimation.hx += particle_set[i].weight*particle_set[i].hx;	
		estimation.hy += particle_set[i].weight*particle_set[i].hy;	
	}
	//cvEllipse( debug_img, cvPoint(estimation.x,estimation.y), cvSize( estimation.hx, estimation.hy),
	//		0, 0, 360,cvScalar(0,255, 0));
	//cvNamedWindow("estimation");
	//cvMoveWindow("estimation", 660,0);
	//cvShowImage("estimation", debug_img);
	//cvWaitKey(1);
	////system("pause");
	//cvReleaseImage( &debug_img);
}
/*
evaluate the sample by color histogram
*/
void color_based_PF::evaluate(IplImage* img)
{
	int i =0;
	float bh_dis = 0, total_prob = 0;
	const float constant = 1/(sqrt(2*3.1415926*BH_sigma));

	for(i =0; i <PARTICLE_NUM; i++){
		build_color_histogram( particle_set[i], local_model, img);
		bh_dis = Bhattacharyya_coefficient( local_model, target_model);
		particle_set[i].weight = exp( - (1- bh_dis)/(2*BH_sigma*BH_sigma));
		total_prob += particle_set[i].weight; 
		//printf("bh dis for %d is %f \n", i, bh_dis);
	}


	//reweight
	if(total_prob == 0){
		for(i =0; i <PARTICLE_NUM; i++){
			particle_set[i].weight = 0;
		}
		estimate_available = false;
		//printf("the total prob error %f \n", total_prob);
		//system("pause");
	}
	else{
		estimate_available = true;
		for(i =0; i <PARTICLE_NUM; i++){
			//printf("before %f after ", particle_set[i].weight);
			particle_set[i].weight =particle_set[i].weight/total_prob;
			//printf(" the weight is %f for %d , total_prob %f \n",particle_set[i].weight, i, total_prob);
		}
	}
	printf("evaluate done \n");
	//system("pause");
}

/*
get the estimation color model
*/
float* color_based_PF::getEstimation_model(IplImage *img)
{
	if(this->estimate_available){
		build_color_histogram(estimation, local_model, img); 
		return local_model;
	}
	else
		return target_model;
}

/*
initilaization for the cpf, we calculated the color histogram
*/
void color_based_PF::initial_target(const sample_cpf &initial_guess, IplImage *pImg)
{
	CvPoint bbox[2]; // the upper left and lower right point in image
	int bin_num, i , j;
	float dis = 0, total_weight = 0;
	const float scale_dis = sqrt( initial_guess.hx*initial_guess.hx + initial_guess.hy*initial_guess.hy);
	//IplImage* debug_img = cvCreateImage( cvSize( Image_width, Image_height), IPL_DEPTH_8U, 3);

	//cvZero(debug_img);

	build_color_histogram( initial_guess, target_model, pImg);

	//initial the sample to all the same
	for( i = 0 ;i < PARTICLE_NUM; i++){
		particle_set[i] = initial_guess;
		particle_set[i].vx += rd.randNorm(0,Vx_var);
		particle_set[i].vy += rd.randNorm(0,Vy_var);
		particle_set[i].a += rd.randNorm(0,A_var);
	}

	//cvNamedWindow("debug");
	//cvShowImage("debug", debug_img);
	//cvWaitKey(1);
	////system("pause");
	//cvReleaseImage( &debug_img);

}

/*
modify the sample with prediction,
in this version we only shift the sample
*/
void color_based_PF::modify_sample( const sample_cpf& p_sample, const sample_cpf&e_sample)
{
	float x_shift = p_sample.x - e_sample.x;
	float y_shift = p_sample.y - e_sample.y;
	float hy_shift = p_sample.hy - e_sample.hy;
	float hx_shift = p_sample.hx - e_sample.hx;

	for(int i =0; i< PARTICLE_NUM; i++){
		particle_set[i].x += x_shift;
		particle_set[i].y += y_shift;
		particle_set[i].hy += hy_shift;
		particle_set[i].hx += hx_shift;
	}

}

/*
predict the sample with the current particle set
*/
void color_based_PF::predict(const IplImage* img)
{
	int i =0;
	//IplImage* debug_img = cvCloneImage(img);
	for(i = 0; i < PARTICLE_NUM; i++){
		particle_set[i].x = particle_set[i].x + particle_set[i].vx*Delta_image_time + rd.randNorm(0,X_var);
		particle_set[i].y = particle_set[i].y + particle_set[i].vy*Delta_image_time + rd.randNorm(0,Y_var);
		particle_set[i].hx += particle_set[i].hx*particle_set[i].a + rd.randNorm(0,Hx_var);;
		particle_set[i].hy += particle_set[i].hy*particle_set[i].a + rd.randNorm(0,Hy_var);;
		particle_set[i].vx += rd.randNorm(0,Vx_var);
		particle_set[i].vy += rd.randNorm(0,Vy_var);
		particle_set[i].a  = rd.randNorm(0,A_var);

		particle_set[i].hx = particle_set[i].hx>=0? particle_set[i].hx: 0;
		particle_set[i].hy = particle_set[i].hy>=0? particle_set[i].hy: 0;
		//cvEllipse( debug_img, cvPoint(particle_set[i].x,particle_set[i].y), cvSize( particle_set[i].hx, particle_set[i].hy),
		//	0, 0, 360,cvScalar(0,0,255));
	}

	
	//cvNamedWindow("predict");
	//cvMoveWindow("predict", 330,0);
	//cvShowImage("predict", debug_img);
	//cvWaitKey(1);
	////system("pause");
	//cvReleaseImage( &debug_img);
}

/*
resample the particle
*/
void color_based_PF::sample()
{
	int i =0, j =0;
	float cum_prob[PARTICLE_NUM] = {0}, prob;
	std::multiset<int> resample_list;
	sample_cpf* temp_ptr = NULL;
	for(i = 0; i <PARTICLE_NUM; i ++){
		if( i == 0)
			cum_prob[i] = particle_set[i].weight;
		else
			cum_prob[i] = cum_prob[i -1] + particle_set[i].weight;
	}
	//debug use
	if(fabs(cum_prob[PARTICLE_NUM - 1] - 1) >0.0001){
		printf(" cum prob error... %f \n",cum_prob[PARTICLE_NUM - 1] );
		system("pause");
	}
	for( i = 0 ;i < PARTICLE_NUM ; i++){
		prob = rd.rand();
		for( j =0; j<PARTICLE_NUM; j++){
			if(prob < cum_prob[j]){
				resample_list.insert(j);
				j = PARTICLE_NUM;
			}
		}
	}

	i = 0;
	for(std::multiset<int>::const_iterator iiter = resample_list.begin(); iiter != resample_list.end(); iiter++){
		//printf(" the list is %d , current i is %d\n", *iiter, i);
		buffer_particle_set[i].replace_sample(particle_set[*iiter]);
		buffer_particle_set[i].weight = 1; // initialthe weight
		i++;
	}

	temp_ptr = particle_set;
	particle_set = buffer_particle_set;
	buffer_particle_set = temp_ptr;
	
	resample_list.clear();
}

/*
the whole tracking procedures
*/
const sample_cpf& color_based_PF::tracking(IplImage *img)
{
	double t = (double)cvGetTickCount();
	IplImage* copy_img = cvCloneImage(img);
	if(estimate_available)
		sample();
	predict(copy_img);
	evaluate(copy_img);
	if(estimate_available){
		estimate(img);
		update_target_model(copy_img);
	}
	if(copy_img)
		cvReleaseImage( &copy_img);
	t = (double)cvGetTickCount() - t;

	printf( "tracking exec time = %gms\n", t/(cvGetTickFrequency()*1000.) );
	return estimation;
}

/*
update the target model id the estimation is good enough
*/
void color_based_PF::update_target_model(IplImage* img)
{
	int i =0;
	float bh_dis = 0, total_weight = 0;
	const float constant = 1/(sqrt(2*3.1415926*BH_sigma*BH_sigma));
	//evaluate the estimation

	build_color_histogram( estimation, local_model, img);
	bh_dis = Bhattacharyya_coefficient( local_model, target_model);
	estimation.weight = constant*exp( - (1- bh_dis)/(2*BH_sigma*BH_sigma));

	printf(" the estimation prob is %f \n", estimation.weight);
	if(estimation.weight > Mean_state_threshold){
		for(i = 0; i< BIN_LENGTH; i ++){
			target_model[i] = target_model[i]*(1 - Alpha_memory) + Alpha_memory*local_model[i];
			total_weight += target_model[i];
		}

		//debug
		if(total_weight == 0){
			printf(" error of total weight in update target model! \n");
			system("pause");
		}
		for(i = 0; i< BIN_LENGTH; i ++){
			target_model[i] = target_model[i]/ total_weight;
		}
	}
}