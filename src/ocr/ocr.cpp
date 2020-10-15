#include "ocr.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <opencv2/core/mat.hpp>
#include <mutex>

namespace fs = boost::filesystem;

std::mutex lock;

enum Depth {
    DEPTH_MASK_32F = 1 << 5
};

OCR* OCR::instance = NULL;

OCR* OCR::getInstance() {
    if (OCR::instance == NULL) {
        lock.lock();
        if (OCR::instance == NULL) {
            OCR* volatile tmp = new OCR();
            OCR::instance = tmp;
        }
        lock.unlock();
    }

    return OCR::instance;
}

OCR* OCR::set_template(std::string p) {
    this->template_path = p;
    this->load();
    return this;
}

OCR::OCR() {
}

void OCR::load() {
    const fs::path dirpath = this->template_path;

    if (!exists(dirpath) || !is_directory(dirpath)) {
        return;
    }

    fs::directory_iterator dirit(dirpath), end;

    for (const fs::directory_entry &entry : dirit) {
        if (is_regular_file(entry.path())) {
            std::string file_path = entry.path().string();
            std::string file_name = entry.path().filename().string();

            std::vector<std::string> names;
            boost::iter_split(names, file_name, boost::first_finder(".jpg"));
            if (names.size() != 2) {
                std::cout << "file name not std: " << names.size() << " "  << file_name << std::endl;        
            } else {
                std::string d_name = names[0];

                cv::Mat temp = cv::imread(file_path, cv::COLOR_BGR2GRAY);
                if (temp.empty()) {
                    std::cout << "invalid img: " << file_path << std::endl;
                } else {
                    this->lib[d_name] = temp;
                }
            }
        }
    }
    /*
    std::for_each(dirit, end, [](const fs::directory_entry& entry) {
        if (is_regular_file(entry.path())) {
            std::string file_path = entry.path().string();
            std::string file_name = entry.path().filename().string();

            std::vector<std::string> names;
            boost::iter_split(names, file_name, boost::first_finder(".jpg"));
            if (names.size() != 1) {
                std::cout << "file name not std: " << file_name << std::endl;        
            } else {
                std::string d_name = names[0];

                cv::Mat temp = cv::imread(file_path, cv::COLOR_BGR2GRAY);
                if (temp.empty()) {
                    std::cout << "invalid img: " << file_path << std::endl;
                } else {
                    this->lib[d_name] = temp;
                }
            }
        }
    });
    */
}

std::vector<std::vector<cv::Point> > OCR::clean_mess(std::vector<std::vector<cv::Point> > contours, double areaThresh) {
    std::vector< std::vector<cv::Point> > c;

    for(int i=0; i<contours.size(); i++) {
        double area = (double)cv::contourArea(cv::InputArray(contours[i]), false);
        if(area <= areaThresh)
            continue;

        c.push_back(contours[i]);
    }

    return c;
}

void OCR::display_img(cv::Mat m, std::string name) {
    return;
    cv::imshow(name, m);
    cv::waitKey();
}

int OCR::recognise(std::string file) {
    cv::Mat img_mat = cv::imread(file);
    return this->recognise(img_mat);
}

int OCR::recognise(std::vector<uchar> v) {
    cv::Mat img_mat = cv::imdecode(v, cv::IMREAD_COLOR);
    return this->recognise(img_mat);
}


int OCR::recognise(cv::Mat& img_mat) {
    if (img_mat.empty()) {
        return -1;
    }
    this->display_img(img_mat, "src");

    cv::Mat bin_mat = this->binary(img_mat);
    this->display_img(bin_mat, "bin");

    for (int i=0; i<5; i++) {
        for (int j=0; j<bin_mat.cols; j++) {
            bin_mat.at<uchar>(i, j) = 255;
        }
    }

    for (int i=bin_mat.rows-5; i<bin_mat.rows; i++) {
        for (int j=0; j<bin_mat.cols; j++) {
            bin_mat.at<uchar>(i, j) = 255;
        }
    }

    for (int i=0; i<bin_mat.rows; i++) {
        for (int j=0; j<10; j++) {
            bin_mat.at<uchar>(i, j) = 255;
        }
    }

    for (int i=0; i<bin_mat.rows; i++) {
        for (int j=bin_mat.cols-10; j<bin_mat.cols; j++) {
            bin_mat.at<uchar>(i, j) = 255;
        }
    }
    this->display_img(bin_mat, "bin-edage");

    cv::Mat normalize_mat = cv::Mat::zeros(bin_mat.size(), bin_mat.type());
    for (int i=0; i<bin_mat.rows; i++) {
        for (int j=0; j<bin_mat.cols; j++) {
            if (bin_mat.at<uchar>(i,j) > 200) {
                normalize_mat.at<uchar>(i,j) = 0;
            } else {
                normalize_mat.at<uchar>(i,j) = 255;
            }
        }
    }
    this->display_img(normalize_mat, "normalize");

    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(normalize_mat, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    contours = this->clean_mess(contours, 100.);
    //std::cout << "contours size: " << contours.size() << std::endl;

    if (contours.size() != 2) {
        return -1;
    }

    cv::Rect rect1 = boundingRect(cv::InputArray(contours[0]));
    cv::Rect rect2 = boundingRect(cv::InputArray(contours[1]));

    cv::Mat l_mat = cv::Mat::zeros(40, 40, normalize_mat.type());
    cv::Mat r_mat = cv::Mat::zeros(40, 40, normalize_mat.type());

    // std::cout << "rect1: (" << rect1.x << ", " << rect1.y << ", " << rect1.width << ", " << rect1.height << ")" << std::endl;
    // std::cout << "rect2: (" << rect2.x << ", " << rect2.y << ", " << rect2.width << ", " << rect2.height << ")" << std::endl;

    if (rect1.x < rect2.x) {
        cv::Mat l_mat_roi(l_mat, cv::Rect(0, 0, rect1.width, rect1.height));
        cv::Mat r_mat_roi(r_mat, cv::Rect(0, 0, rect2.width, rect2.height));

        cv::Mat l_roi(normalize_mat, rect1);
        cv::Mat r_roi(normalize_mat, rect2);

        cv::bitwise_or(l_mat_roi, l_roi, l_mat_roi);
        cv::bitwise_or(r_mat_roi, r_roi, r_mat_roi);
    } else {
        cv::Mat l_mat_roi(l_mat, cv::Rect(0, 0, rect2.width, rect2.height));
        cv::Mat r_mat_roi(r_mat, cv::Rect(0, 0, rect1.width, rect1.height));

        cv::Mat l_roi(normalize_mat, rect2);
        cv::Mat r_roi(normalize_mat, rect1);
        
        cv::bitwise_or(l_mat_roi, l_roi, l_mat_roi);
        cv::bitwise_or(r_mat_roi, r_roi, r_mat_roi);
    }
    this->display_img(l_mat, "left");
    this->display_img(r_mat, "right");

    std::vector<cv::Mat> mats;
    mats.push_back(l_mat);
    mats.push_back(r_mat);

    std::vector<float> score;
    score.push_back(0);
    score.push_back(0);

    std::vector<std::string> name;
    name.push_back("");
    name.push_back("");

    for (int i=0; i<2; i++) {
        for (std::map<std::string, cv::Mat>::iterator it = this->lib.begin(); it != this->lib.end(); ++it) {
            double minv, maxv;
            cv::Point minloc,maxloc;
            int w = mats[i].cols - it->second.cols + 1;
            int h = mats[i].rows - it->second.rows + 1;
            cv::Mat result(h, w, DEPTH_MASK_32F);
            cv::matchTemplate(cv::InputArray(mats[i]), cv::InputArray(it->second), cv::OutputArray(result), cv::TM_SQDIFF_NORMED);
            cv::minMaxLoc(result, &minv, &maxv, &minloc, &maxloc, cv::Mat());

            // std::cout << it->first << " minv " << minv << std::endl;
            //cv::imshow(it->first, it->second);
            //cv::waitKey();
            if (score[0 + i] == 0 || score[0+i] > minv) {
                score[0+i] = minv;
                name[0+i] = it->first;
            }
        }
    }

    int t = atoi(name[0].c_str());
    int g = atoi(name[1].c_str());

    return t * 10 + g;
}

cv::Mat OCR::dilate_img(cv::Mat img_mat) {
    int dilation_size = 1;
    cv::Mat element = getStructuringElement(cv::MORPH_RECT,
            cv::Size(2*dilation_size + 1, 2*dilation_size+1),
            cv::Point(dilation_size, dilation_size));

    cv::Mat dilation_dst;
    cv::dilate(img_mat, dilation_dst, element);
    return dilation_dst;
}

cv::Mat OCR::erode_img(cv::Mat img_mat) {
    int erosion_size = 1;

    cv::Mat kernel = getStructuringElement(cv::MORPH_RECT,
            cv::Size(2*erosion_size + 1, 2*erosion_size+1),
            cv::Point(erosion_size , erosion_size ));


    cv::Mat erode_dst;
    cv::erode(img_mat, erode_dst, kernel);
    return erode_dst;
}

cv::Mat OCR::binary(cv::Mat img_mat) {
    cv::Mat dilate_mat = this->dilate_img(img_mat);
    cv::Mat erode_mat = this->erode_img(dilate_mat);

    cv::Mat gray_mat;
    cv::cvtColor(erode_mat, gray_mat, cv::COLOR_BGR2GRAY);

    cv::Mat adaptive_mat;
    cv::adaptiveThreshold(gray_mat, adaptive_mat, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 25, 10);
    return adaptive_mat;
}

void OCR::test(std::string test_dir) {
    const fs::path dirpath = test_dir;
    if (!exists(dirpath) || !is_directory(dirpath)) {
        return;
    }

    fs::directory_iterator dirit(dirpath), end;

    for (const fs::directory_entry &entry : dirit) {
        if (is_regular_file(entry.path())) {
            std::string file_path = entry.path().string();
            std::string file_name = entry.path().filename().string();

            std::vector<std::string> names;
            boost::iter_split(names, file_name, boost::first_finder(".jpg"));
            if (names.size() != 2) {
                std::cout << "file name not std: " << names.size() << " "  << file_name << std::endl;        
            } else {
                std::string d_name = names[0];

                std::vector<std::string> ns;
                boost::iter_split(ns, d_name, boost::first_finder("-"));
                
                int nv = atoi(ns[0].c_str());
                int res = this->recognise(file_path);
                std::cout << names[0] << " actual --> " << res << std::endl;
                if (nv != res) {
                    std::cout << "--------------f------------" << file_name << std::endl;
                }
            }
        }
    }
}

int OCR::read_file(std::string file, std::string &s) {

    FILE *fp = fopen(file.c_str(), "rb");
    if(!fp)
        return -1;

    fseek(fp, 0, SEEK_END);
    if(ferror(fp)) {
        fclose(fp);
        return -2;
    }

    long flen = ftell(fp);
    if(flen == -1) {
        fclose(fp);
        return -3;
    }

    fseek(fp, 0, SEEK_SET);
    if(ferror(fp)) {
        fclose(fp);
        return -4;
    }

    char *buff = (char*)calloc(flen+1, sizeof(char));
    if(buff == NULL) {
        fclose(fp);
        return -5;
    }

    long size = fread(buff, sizeof(char), flen, fp);
    if(ferror(fp)) {
        fclose(fp);
        return -6;
    }

    fclose(fp);

    s = std::string(buff);
    return 0;
}
