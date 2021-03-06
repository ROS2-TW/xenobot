#ifndef LANE_DETECTOR_H
#define LANE_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <ros/ros.h>
#include <yaml-cpp/yaml.h>
#include <cv_bridge/cv_bridge.h>

#define SEMI_IMAGE_WIDTH 320
#define SEMI_IMAGE_HEIGHT 240

using namespace std;
using namespace cv;

class LaneDetector {
	private:
	string yaml_path;

	int outer_threshold_h_min, outer_threshold_h_max;
	int outer_threshold_s_min, outer_threshold_s_max;
	int outer_threshold_v_min, outer_threshold_v_max;

	int inner_threshold_h_min, inner_threshold_h_max;
	int inner_threshold_s_min, inner_threshold_s_max;
	int inner_threshold_v_min, inner_threshold_v_max;

	cv::Mat outer_hsv_image, outer_threshold_image, outer_hough_image;
	cv::Mat inner_hsv_image, inner_threshold_image, inner_hough_image;
	cv::Mat lane_mark_image, canny_image;

	cv::Mat H; //Homography matrix

	bool calibrate_mode;

	ros::NodeHandle node;
        ros::Publisher outer_threshold_img_publisher, outter_hough_img_publisher;
        ros::Publisher inner_threshold_img_publisher, inner_hough_img_publisher;
	ros::Publisher canny_img_publisher;
	ros::Publisher marked_image_publisher;

	bool read_threshold_setting(string yaml_path);
	bool read_extrinsic_calibration(string yaml_path);
	void mark_lane(cv::Mat& lane_mark_image, vector<Vec4i>& lines, Scalar line_color, Scalar dot_color, Scalar text_color);
	Point3f point_transform_image_to_ground(int pixel_x, int pixel_y);
	void append_yaml_data(YAML::Emitter& yaml_handler, string key, int value);
	void calculate_best_fittedline(vector<Vec4i>& lines, Vec4f& best_fitted_line);
	void homography_transform(cv::Mat& raw_image, cv::Mat& homograhy_image);

	public:
	LaneDetector(string yaml_path, bool calibrate_mode);

	void lane_detect(cv::Mat& image);
	void lane_detect(cv::Mat& raw_image, vector<Vec4i>& outer_lines,
		vector<Vec4i>& inner_lines);	
	void publish_images();
	void set_hsv(
	       double outer_h_max, double outer_h_min, double outer_s_max,
	       double outer_s_min, double outer_v_max, double outer_v_min,
	       double inner_h_max, double inner_h_min, double inner_s_max,
	       double inner_s_min, double inner_v_max, double inner_v_min
	);

	bool pose_estimate(vector<Vec4i>& outer_lines, float& d, float& phi);
	bool generate_vote(Point2f p1, Point2f p2, uint8_t segment_color, float& d,
		float& phi, float& l);

	void save_thresholding_yaml();
	bool load_yaml_setting();
};

#endif
