#include "../Skeleton/third_party/img_reader/reader.h"
#include "../Skeleton/third_party/vision/vision_object.h"
#include "../Skeleton/third_party/wincap/wincap.h"

#include <gtest/gtest.h>
#include <fstream>

void ReadNameAndCheckPrecise(
    Reader& r, 
    const std::filesystem::directory_entry& file) 
{
  cv::Mat img_mat = cv::imread(file.path(), cv::COLOR_RGB2BGR);

  // img_mat = img_mat(cv::Range(0, img_mat.rows), cv::Range(20, img_mat.cols));
  std::string text = r.ReadImageFromCvMat(img_mat, true);
  std::cout << text << std::endl;

  size_t result = std::string(file.path()).find(text);

  cv::imshow("test", img_mat);  
  cv::waitKey(0);

  EXPECT_NE(result, std::string::npos);
}

// TEST(ReadShipNames, HandlesPathToFolder) {
//   std::string path{"/Users/georgiyonischenko/Desktop/Programming/Skeleton/tests/data/ship_names"};

//   Reader r;

//   for (const auto& file : std::filesystem::directory_iterator(path)) {
//     ReadNameAndCheckPrecise(r, file);
//   }
// }

TEST(ReadNumbers, HandlesPathToFolder) {
   std::string path{"/Users/georgiyonischenko/Desktop/Programming/Skeleton/tests/data/numbers"};

   Reader r;
   for (const auto& file : std::filesystem ::directory_iterator(path)) {
    ReadNameAndCheckPrecise(r, file);
   }
}