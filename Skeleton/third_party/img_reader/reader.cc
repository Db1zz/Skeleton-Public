#include "reader.h"

#include <opencv2/dnn_superres.hpp>
#include <opencv2/highgui.hpp>

Reader::Reader()
    : Reader("eng") {}

Reader::~Reader() {
  ocr_.End();
}

Reader::Reader(std::string&& language) {
  ocr_.Init("", language.c_str(), tesseract::OEM_LSTM_ONLY);
  ocr_.SetPageSegMode(tesseract::PSM_AUTO);
}

void Reader::UpscaleImage(cv::Mat &result_img)
{ 
  // cv::threshold(result_img, result_img, 120, 255, cv::THRESH_BINARY_INV);
     
  // best results was achieved on *2 scale.
  cv::dnn_superres::DnnSuperResImpl sr;
  sr.readModel("/Users/georgiyonischenko/Downloads/FSRCNN_x2.pb");
  sr.setModel("fsrcnn", 2);
  sr.upsample(result_img, result_img);

  // const float scale{ 1 };

  // cv::Size size(result_img.cols * scale, result_img.rows * scale);
  // cv::resize(result_img, result_img, size,
  //            0, // size.height / result_img.cols,
  //            0, // size.width / result_img.rows, 
  //            cv::INTER_LANCZOS4);
  //            // cv::INTER_NEAREST_EXACT seems accurate
}

std::string Reader::ReadImageFromFile(const std::string&& path, bool upscale) {
  cv::Mat img = cv::imread(path, cv::IMREAD_UNCHANGED);
  if (img.empty()) {
    READER_PRINT_ERROR("cv::imread error invalid path or image format was given")
    exit(-1);
  }
  return ReadImageFromCvMat(img, upscale);
}

std::string Reader::ReadImageFromCvMat(cv::Mat &img, bool upscale) {
  std::string result = "";

  if (img.empty()) {
    READER_PRINT_ERROR("Empty cv::Mat was passed to the function")
    return "";
  }

  if (upscale) {
    UpscaleImage(img);
  }

  ocr_.SetImage(img.data, img.cols, img.rows, 3, (int)img.step);
  result = ocr_.GetUTF8Text();

  // pop back ' '(space) character from the string.
  if (!result.empty()) {
    result.pop_back(); 
  }

  return result;
}