/**
 * Copyright 2016 Ortelio Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * #http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "includes.ihh"

/**
 * \class pixy_cam
 * \brief connects with the pixy visual sensor (CMUcam5), captures a
 * snapshot from it and puts the snapshot in the memory encoded in one
 * of the popular image formats(png, jpg) 
 * \version 0.1.0
 * \date 28 November 2016
 * \author Lazaros Penteridis <lp@ortelio.co.uk>
 */
class pixy_cam
{
public:
    /**
    * \brief initialise pixy, make it ready to capture a snapshot
    */
    pixy_cam();

    /**
    * \brief encode the rgb image captured as an OpenCV matrix (cv::Mat)
    * in the memory to one of the popular image formats (png, jpg)
    * \param height is the height of the image. Valid values: 200, 400, 800.
    * \param width is the width of the image. Valid values: 320, 640, 1280.
    * \param ext is the extension of the encoding ("png", "jpg")
    * \param buf is the buffer that will receive the encoded image
    * \param params Format-specific parameters. See opencv's imwrite().
    */
    bool get_encoded_img(
                            uint16_t height,
                            uint16_t width //,
            //                const std::String& ext,
            //                std::vector<uchar>& buf,
            //                const std::vector<int>& params=std::vector<int>()
                        );
    
    /**
     * \brief capture a snapshot and call the renderBA81 method to
     * render it as RGB image
     * \param height is the height of the image. Valid values: 200, 400, 800.
     * \param width is the width of the image. Valid values: 320, 640, 1280.
     */
    cv::Mat get_img(
                       uint16_t height,
                       uint16_t width
                   );

    /**
     * \brief Render an RGB image from the captured pixy snapshot
     * through dealing with the bayer interpolation proccess
     * \param width of the image (matrix in memory)
     * \param height of the image (matrix in memory)
     * \param *frame pointer to the image location in memory
     */
    cv::Mat render_BA81(
                           uint16_t width,
                           uint16_t height,
                           uint8_t *frame
                       );

    /**
     * \brief bayer interpolation proccess
     * \param width is the width of the image
     * \param x is the horizontal position of the current pixel
     * \param y is the vertical position of the current pixel
     * \param *pixel is a pointer to the memory address where the
     * value of the current pixel is stored
     * \param &r is a reference to the memory address where the red
     * value of the current pixel will be stored
     * \param &g is a reference to the memory address where the green
     * value of the current pixel will be stored
     * \param &b is a reference to the memory address where the blue
     * value of the current pixel will be stored
     */
    void interpolate_bayer(
                                    uint16_t width,
                                    uint16_t x,
                                    uint16_t y,
                                    uint8_t *pixel,
                                    uint8_t* r,
                                    uint8_t* g,
                                    uint8_t* b
                                 );

private:
    int pixy_init_status, return_value;
    int32_t response;
};
