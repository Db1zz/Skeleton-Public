#include "vision_object.h"

VisionObject::VisionObject(std::string needle_img_path, 
                          cv::TemplateMatchModes match_method)
    : method_{match_method} 
{
  needle_img_ = cv::imread(needle_img_path, method_);
  if (needle_img_.empty()) {
    std::cout << "Invalid image path or image format: " << needle_img_path << std::endl;
    exit(-1);
  }
  needle_w_ = needle_img_.size().width;
  needle_h_ = needle_img_.size().height;
}

VisionObject::VisionObject(std::string& needle_img_path)
    : VisionObject(needle_img_path, cv::TemplateMatchModes::TM_CCOEFF_NORMED) {}

std::vector<cv::Rect> VisionObject::FindObjectOnImage(
    cv::Mat haystack_img,
    double threshold) 
{
  if (haystack_img.empty()) {
      std::cout << "Invalid haystack image path or format" << std::endl;
      exit(-1);
  }
  cv::Mat result;
  cv::matchTemplate(needle_img_, haystack_img, result, method_);
  cv::threshold(result, result, threshold, 1.0, cv::THRESH_BINARY);

  std::vector<cv::Point> locations;
  cv::findNonZero(result, locations);

  std::vector<cv::Rect> rectangles;
  for (const auto &loc : locations) {
    cv::Rect rect(loc.x, loc.y, needle_w_, needle_h_);
    rectangles.push_back(rect);
    rectangles.push_back(rect);
  }

  std::vector<int> weights;
  cv::groupRectangles(rectangles, weights, 1, 0.1);

  return rectangles;
}

std::vector<cv::Rect> VisionObject::FindObjectOnImageWithinRoi(
    cv::Mat haystack_img,
    std::vector<cv::Rect> roi,
    double threshold)
{
  if (haystack_img.empty()) {
    std::cout << "Invalid haystack image path or format" << std::endl;
    exit(-1);
  }

  std::vector<cv::Rect> rectangles;
  if (!roi.empty()) {
    for (int i = 0; i < roi.size(); i++) {
      cv::Mat img_roi(haystack_img, roi[i]);
      std::vector<cv::Rect> res = FindObjectOnImage(img_roi);

      for (int j = 0; j < res.size(); j++) {
        res[j].x += roi[i].x;
        res[j].y += roi[i].y;
        rectangles.push_back(res[j]);
      }
    }
  }

  return rectangles;
}

std::vector<std::pair<int, int>> VisionObject::HighlightRectangles(
    cv::Mat haystack_img, 
    std::vector<cv::Rect> rectangles)
{
  std::vector<std::pair<int, int>> points;
  if (!rectangles.empty()) {
    cv::Scalar line_color = cv::Scalar(0, 255, 0);
    int line_thickness{2};
    int line_type{cv::LINE_4};
    int shift{0};

    for (int i = 0; i < rectangles.size(); i++) {
      int center_x = rectangles[i].x + (rectangles[i].width / 2);
      int center_y = rectangles[i].y + (rectangles[i].width / 2);
      points.push_back(std::pair<int, int>(center_x, center_y));

      cv::Point top_left{rectangles[i].x, rectangles[i].y};
      cv::Point bottom_right{rectangles[i].x + rectangles[i].width,
                             rectangles[i].y + rectangles[i].height};

      cv::rectangle(haystack_img, top_left, bottom_right, line_color,
                    line_thickness, line_type, shift);
    }

    cv::imshow("Matches", haystack_img);
  }

  return points;
}