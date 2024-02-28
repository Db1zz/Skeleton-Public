#include "wincap.h"
#include <cstdint>
#include <iostream>
#include <vector>

cv::Mat Wincap::GetDesktopScreenshot(const WinRect& rect) {
  return GetDesktopScreenshot(rect.offset_x, rect.offset_y, rect.w, rect.h);
}

#if defined(_WIN32) || defined(_WIN64)

cv::Mat Wincap::GetDesktopScreenshot(int cropped_x, int cropped_y, 
                              int w, int h)
{
  HDC hScreenDC = GetDC(NULL);
  if(!hScreenDC) {
    PRINT_ERROR("Handle is nullptr")
    exit(-1);
  }

  HDC hMemoryDC = CreateCompatibleDC(hScreenDC); 
  if(!hMemoryDC) {
    PRINT_ERROR("Handle is nullptr")
    exit(-1);
  }

  HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, w, h);
  if(!hBitmap) {
    PRINT_ERROR("Handle is nullptr")
    exit(-1);
  }

  HBITMAP hOldBitMap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
  if(!hOldBitMap) {
    PRINT_ERROR("Handle is nullptr")
    exit(-1);
  }

  if(!BitBlt(hMemoryDC, 0, 0, w, h, hScreenDC, cropped_x, cropped_y, SRCCOPY)) {
    PRINT_ERROR(GetLastError())
    exit(-1);
  }

  hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitMap);
  BITMAP bitmap;
  GetObject(hBitmap, (int)sizeof(bitmap), &bitmap);

  BITMAPINFOHEADER bmi = {0};
  bmi.biSize = sizeof(BITMAPINFOHEADER);
  bmi.biPlanes = 1;
  bmi.biBitCount = 32;
  bmi.biWidth = w;
  bmi.biHeight = -h;

  cv::Mat img(h, w, CV_8UC4);
  GetDIBits(hMemoryDC, hBitmap, 0, h, img.data, 
            (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
  if(img.empty()) {
    PRINT_ERROR("Img array is empty")
    exit(-1);
  }
  cv::cvtColor(img, img, cv::COLOR_BGRA2BGR);

  DeleteDC(hScreenDC);
  DeleteDC(hMemoryDC);
  DeleteObject(hBitmap);
  DeleteObject(hOldBitMap);

  return img;
}

#elif defined(__APPLE__)

cv::Mat Wincap::GetDesktopScreenshot(int cropped_x, int cropped_y, 
                                     int w, int h) 
{
  if (cropped_x >= w || cropped_y >= h) {
    std::cout << "FATAL ERROR: " << __FILE__ 
              << " in function " << __func__ 
              << " at line " << __LINE__ << '\n';
    printf("[x: %d >= w: %d || y: %d >= h: %d] \n", cropped_x, cropped_y, w, h);
    exit(-1);
  }

  cv::Mat img(cv::Size (w, h), CV_8UC4);
  CGColorSpaceRef color_space = CGColorSpaceCreateDeviceRGB();
  CGContextRef context_ref = CGBitmapContextCreate(
    img.data, img.cols, img.rows,
    8, img.step[0],
    color_space, kCGImageAlphaNoneSkipLast);

  CGRect rect {
    .size.width = (double)w,
    .size.height = (double)h,
    .origin.x = 0,
    .origin.y = 0
  };

  // Debug
  // std::cout << "Rect1: (width x" << rect.size.width 
  //           << ", height x" << rect.size.height << ")\n";

  CGImageRef screenshot
      = CGDisplayCreateImageForRect(kCGDirectMainDisplay, rect);

  CGContextDrawImage(context_ref, rect, screenshot);
  cvtColor(img, img, cv::COLOR_RGB2BGR);
  
  CGContextRelease(context_ref);
  CGColorSpaceRelease(color_space);
  CGImageRelease(screenshot);

  if (cropped_x > 0 || cropped_y > 0) {
    img = img(cv::Range(cropped_y, h), cv::Range(cropped_x, w));
  }

  return img;
}

#endif 

std::pair<int, int> Wincap::GetRelativePosition(std::pair<int, int> point, 
                                                Wincap::WinRect &screen_pos)
{
    return std::pair<int, int>(point.first  + screen_pos.offset_x,
                               point.second + screen_pos.offset_y);
}