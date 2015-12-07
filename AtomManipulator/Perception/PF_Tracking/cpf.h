//opencv library
#ifndef _CPF_TRACKER
#define _CPF_TRACKER
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
//stdio
#include <stdio.h>
#include <stdlib.h>
//random number generator
#include "MersenneTwister.h"

#include "imageIter.h"

#define BIN_NUM 8
#define PARTICLE_NUM 50

#define Image_width 640
#define Image_height 480
#define Delta_image_time 30

const int BIN_WIDTH = 256/BIN_NUM;
const int BIN_LENGTH = BIN_NUM*BIN_NUM*BIN_NUM;

//define variance
#if _SCENE == 0 || _SCENE == 1
const float X_var = 3;
const float Y_var = 3;
const float Vx_var = 1;
const float Vy_var = 1;
const float Hx_var = 0.1;
const float Hy_var = 0.1;
const float A_var = 0.01;
const float BH_sigma = 0.05;
const float Alpha_memory = 0.1;
const float Mean_state_threshold = 0.1;
#elif _SCENE == 2
// out side
const float X_var = 3;
const float Y_var = 3;
const float Vx_var = 10;
const float Vy_var = 10;
const float Hx_var = 0.1;
const float Hy_var = 0.1;
const float A_var = 0.05;
const float BH_sigma = 0.05;
const float Alpha_memory = 0.1;
const float Mean_state_threshold = 0.01;
#endif
typedef struct _sample_cpf
{
	float x;      // the x coord in image
	float y;      // the y coord in img
	float vx;     // estimated x spd
	float vy;     // estimated y spd
	float hx;     // estimated particle half width
	float hy;     // estimated particle half height
	float a;      // the scale change rate
	float weight; // the weight of this sample

	_sample_cpf()
	{
		x = 0;
		y = 0;
		vx = 0; 
		vy = 0;
		hx = 0;
		hy = 0;
		a = 0;
		weight = 1;
	}
	void replace_sample(const _sample_cpf& t_sample){
		x = t_sample.x;
		y = t_sample.y;
		vx = t_sample.vx;
		vy = t_sample.vy;
		hx = t_sample.hx;
		hy = t_sample.hy;
		a = t_sample.a;
		weight = t_sample.weight;
	}
}sample_cpf;


/*
 the color based particle filter
 with input the initial guess of the
 image position , and bounding rectangle .
*/
class color_based_PF
{
public:
//function
	//contructor
	color_based_PF();
	~color_based_PF();
	//global function
	bool isEstimate_available()const{return estimate_available;};
	const sample_cpf& tracking(IplImage* img);
	float* getEstimation_model(IplImage* img);
	float* getTarget_model(){return target_model;};
	void initial_target(const sample_cpf & initial_guess, IplImage* img);
	void modify_sample(const sample_cpf & p_sample, const sample_cpf & e_sample);
	void sample();
	// local functions
	static float Bhattacharyya_coefficient(const float model1[], const float model2[]);
	inline float kernel_weight(const float& dis)const{ return dis>1? 0 : 1- dis*dis;};
	void build_color_histogram(const sample_cpf& sample, float* histgram, IplImage* img);
private:
//variables
	bool estimate_available;
	IplImage* target_img;
	MTRand rd;
	sample_cpf estimation;

	float target_model[BIN_NUM*BIN_NUM*BIN_NUM];
	float local_model[BIN_NUM*BIN_NUM*BIN_NUM];
	sample_cpf* particle_set;
	sample_cpf* buffer_particle_set;
//function
	// pf functions
	void estimate(const IplImage* img);
	void evaluate(IplImage* img);
	void predict(const IplImage* img);
	// target model
	void build_target_model();
	void update_target_model(IplImage* img);

};
#endif