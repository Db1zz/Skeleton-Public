#ifndef READER_H 
#define READER_H

#include <filesystem>
#include <string>

// #if defined(_WIN32) || defined(_WIN64)
//     #include <Windows.h>
// #endif

#include <opencv2/opencv.hpp>
#include <allheaders.h>
#include <tesseract/baseapi.h>

#define READER_PRINT_ERROR(msg) std::cerr << "[Reader]: " << msg << " { File: " << __FILE__ << " Function: " << __func__ << " line: " << __LINE__ << " }" << std::endl; 

class Reader {
  public:
    Reader();
    Reader(std::string&& language);
    ~Reader();
    
    std::string ReadImageFromFile(const std::string&& path, bool upscale);
    std::string ReadImageFromCvMat(cv::Mat &img, bool upscale);

  protected:
    void UpscaleImage(cv::Mat& img);

  private:
    tesseract::TessBaseAPI ocr_;
};

#endif // READER_H