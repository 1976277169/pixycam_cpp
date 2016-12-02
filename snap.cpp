#include "includes/pixy_cam.hpp"

int main(int argc, char * argv[])
{
    pixy_cam pixy;
    bool result = pixy.get_encoded_img(400, 640);
    return 0;
}
