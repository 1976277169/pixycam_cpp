#include "pixy_cam.hpp"

pixy_cam::pixy_cam()
{
    pixy_init_status = pixy_init();
    if(!pixy_init_status == 0)
        pixy_error(pixy_init_status);
    return_value = pixy_command("stop", END_OUT_ARGS, &response, END_IN_ARGS); 
    if (return_value != 0)    
        throw std::runtime_error("pixy_command(\"stop\") error");
    return_value = pixy_rcs_set_position(1, 900);
    if (return_value != 0)    
        throw std::runtime_error("pixy_rcs_set_position 1st call error");
    return_value = pixy_rcs_set_position(0, 500);
    if (return_value != 0)    
        throw std::runtime_error("pixy_rcs_set_position 2nd call error");
}

bool pixy_cam::save_encoded_img()
{
    // currently these height and width values seem to be the only ones that make pixy
    // cam return a snapshot, not because it doesn't support higher resolution modes
    // but because of memory limitations with max memsize being 73,728
    // 320x200 = 64,000 is the only image size that fits this limitation
    uint16_t height = 200;
    uint16_t width = 320;
    cv::Mat image = get_img(height, width);
    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    cv::String img_name("snapshot.png");
    try {
        imwrite(img_name, image, compression_params);
    }
    catch (std::exception & e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
    return true;
}

cv::Mat pixy_cam::get_img(uint16_t height, uint16_t width)
{
    unsigned char *pixels;
    int32_t response, fourcc;
    int8_t render_flags;
    int return_value;
    uint16_t rwidth, rheight;
    uint32_t  num_pixels;

    return_value = pixy_command("run", END_OUT_ARGS, &response, END_IN_ARGS);   
    return_value = pixy_command("stop", END_OUT_ARGS, &response, END_IN_ARGS);
    return_value = pixy_command("cam_getFrame",  // std::String id for remote procedure
                                 0x01, 0x21,      // mode
                                 0x02,   0,        // xoffset
                                 0x02,   0,         // yoffset
                                 0x02, width,       // width
                                 0x02, height,       // height
                                 0,            // separator
                                 &response, &fourcc, &render_flags, &rwidth, &rheight, &num_pixels, &pixels, 0);
    if (response != 0)
        throw std::runtime_error("cam_getFrame command didn't return succesfully");
    if (rheight != height)
        throw std::runtime_error("cam_getFrame command didn't return the expected image height");
    if (rwidth!= width)
        throw std::runtime_error("cam_getFrame command didn't return the expected image width");
    return render_BA81(rwidth, rheight, pixels);
}

cv::Mat pixy_cam::render_BA81(uint16_t width, uint16_t height, uint8_t *frame)
{
    uint16_t x, y;
    uint8_t r, g, b;
    cv::Mat imageRGB;

    frame += width;
    uchar data[3*((height-2)*(width-2))];

    uint m = 0;
    for (y=1; y<height-1; y++)
    {
        frame++;
        for (x=1; x<width-1; x++, frame++)
        {
            interpolate_bayer(width, x, y, frame, &r, &g, &b);
            data[m++] = b;
            data[m++] = g;
            data[m++] = r;
        }
        frame++;
    }

    imageRGB =  cv::Mat(height - 2,width -2, CV_8UC3, data);
    return imageRGB;
}

void pixy_cam::interpolate_bayer(uint16_t width, uint16_t x, uint16_t y, uint8_t *pixel, uint8_t* r, uint8_t* g, uint8_t* b)
{
    if (y&1)
    {
        if (x&1)
        {
            *r = *pixel;
            *g = (*(pixel-1)+*(pixel+1)+*(pixel+width)+*(pixel-width))>>2;
            *b = (*(pixel-width-1)+*(pixel-width+1)+*(pixel+width-1)+*(pixel+width+1))>>2;
        }
        else
        {
            *r = (*(pixel-1)+*(pixel+1))>>1;
            *g = *pixel;
            *b = (*(pixel-width)+*(pixel+width))>>1;
        }
    }
    else
    {
        if (x&1)
        {
            *r = (*(pixel-width)+*(pixel+width))>>1;
            *g = *pixel;
            *b = (*(pixel-1)+*(pixel+1))>>1;
        }
        else
        {
            *r = (*(pixel-width-1)+*(pixel-width+1)+*(pixel+width-1)+*(pixel+width+1))>>2;
            *g = (*(pixel-1)+*(pixel+1)+*(pixel+width)+*(pixel-width))>>2;
            *b = *pixel;
        }
    }
}
