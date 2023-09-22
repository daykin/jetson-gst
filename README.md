## GStreamer Appsink to VPI

This utility (in theory) will set up a GStreamer pipeline to either your mp4 file, or TIS camera serial number.

The method vpi_callback is a very bare-bones adapter. It will, on a best-effort basis, with essentially no error checking,

grab the color type, height and width of received frames, and package them into an NVidia VPIImage object. 

At the moment, it does this by wrapping a pointer to a generic GstMapInfo.data array, as opposed to copying the array more than strictly necessary.

### Requirements

TISCamera SDK - https://github.com/TheImagingSource/tiscamera.git (follow the install instructions)

NVidia VPI v1.1 - sudo apt install vpi1-dev

OpenCV 4 - sudo apt install libopencv-dev

### Building

mkdir build

cd build

cmake ..

make