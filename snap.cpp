#include "includes/pixy_cam.hpp"

int main(int argc, char * argv[])
{
    pixy_cam pixy;
    bool result = pixy.save_encoded_img();
    if (result)
        std::cout << "image saved succesfully" << std::endl;
    return 0;
}
