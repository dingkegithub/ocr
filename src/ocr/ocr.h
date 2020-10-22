#ifndef _OCR_H_
#define _OCR_H_

#include "opencv2/opencv.hpp"

#include <string>
#include <map>
#include <vector>


class OCR {
    private:
        std::string template_path;
        std::map<std::string, cv::Mat > lib;
        static OCR* instance;;

    private:
        void load();
        int read_file(std::string file, std::string &s);
        int average_pix(cv::Mat &mat);
        cv::Mat binary(cv::Mat img_mat);
        cv::Mat erode_img(cv::Mat img_mat);
        cv::Mat dilate_img(cv::Mat img_mat);
        void display_img(cv::Mat m, std::string name);
        std::vector<std::vector<cv::Point> > clean_mess(std::vector<std::vector<cv::Point> > contours, double areaThresh);
        OCR();

    public:
        bool display_switch;
        static OCR* getInstance();
        OCR* set_template(std::string p);
        int recognise(std::string file);
        int recognise(cv::Mat& mat);
        int recognise(std::vector<uchar> v);
        void test(std::string test_dir);
};

#endif
