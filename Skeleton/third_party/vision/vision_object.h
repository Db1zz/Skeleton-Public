#ifndef VISION_OBJECT_H_
#define VISION_OBJECT_H_

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

using std::vector;
using std::pair;

class VisionObject {
  public:
    VisionObject(std::string needle_img_path, 
                 cv::TemplateMatchModes match_method);

    VisionObject(std::string& needle_img_path);
    
    vector<cv::Rect> FindObjectOnImage(cv::Mat haystack_img, 
                                       double threshold = 0.85);

    vector<pair<int, int>> HighlightRectangles(cv::Mat haystack_img, 
                                               vector<cv::Rect> rectangles);

    vector<cv::Rect> FindObjectOnImageWithinRoi(cv::Mat haystack_img,
                                                vector<cv::Rect> roi,
                                                double threshold = 0.70);

  private:
    cv::TemplateMatchModes method_;
    cv::Mat needle_img_;
    int needle_w_;
    int needle_h_;
};

#endif // VISION_OBJECT_H_