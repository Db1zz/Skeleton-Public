#ifndef WINCAP_H_
#define WINCAP_H_

#if defined(_WIN32) || defined(_WIN64)
  #include <windows.h>
#elif defined(__APPLE__)
  #include <ApplicationServices/ApplicationServices.h>
#endif

#include <string>

#include <opencv2/opencv.hpp>

// Messages
#define PRINT_ERROR(msg) std::cerr << "[Windowcapture]: " << msg << " { File: " << __FILE__ << " Function: " << __func__ << " line: " << __LINE__ << " }" << std::endl; 

class Wincap  {
  public:
    struct WinRect {
      int offset_x;
      int offset_y;
      int w;
      int h;
        
      WinRect() : offset_x(0), offset_y(0), w(0), h(0) {}

      WinRect(int offset_x, int offset_y, int w, int h)
          : offset_x(offset_x), offset_y(offset_y), w(w), h(h) {}
    };

  public:
    cv::Mat GetScreenshot(int offset_x, int offset_y, 
                          int w, int h);

    cv::Mat GetScreenshot(const WinRect& rect);

    std::pair<int, int> GetPosition(std::pair<int, int> point, 
                                    Wincap::WinRect& screen_pos);
};

#endif // WINCAP_H_
