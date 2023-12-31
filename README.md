## GStreamer Appsink to VPI

This utility (in theory) will set up a GStreamer pipeline to either your mp4 file, or TIS camera serial number.
The method vpi_callback is a very bare-bones adapter. It will, on a best-effort basis, with essentially no error checking,
grab the color type, height and width of received frames, and package them into an NVidia VPIImage object. 
At the moment, it does this by wrapping a pointer to a generic GstMapInfo.data array, as opposed to copying the array more than strictly necessary.

In your real-life processing, you'll probably be creating an output image of some kind, submitting jobs to the `VPIStream` and doing some processing.
While this is happening, block the invalidation of an existing frame with `VPIStreamSync`. See [here](https://docs.nvidia.com/vpi/1.1/group__VPI__Stream.html#ga31f569f9da89eabc0249d42746f1c3b7). 

### Requirements

TISCamera SDK - https://github.com/TheImagingSource/tiscamera.git (follow the install instructions)

NVidia VPI v1.1 - `sudo apt install vpi1-dev`

OpenCV 4 - `sudo apt install libopencv-dev`

### Building

`mkdir build`

`cd build`

`cmake ..`

`make`

### Try a dumb example

go to `build/bin` and run `./demo -v ../../media/motor-sample/2rpm/2rpm.mp4`.

### Helpful Documentation

- [NVidia VPI 1.1](https://docs.nvidia.com/vpi/1.1/index.html)
- [General GStreamer Plugins](https://docs.nvidia.com/vpi/1.1/group__VPI__Stream.html#ga31f569f9da89eabc0249d42746f1c3b7)
- [TIS Camera SDK](https://github.com/TheImagingSource/tiscamera.git)
- [OpenCV](https://docs.opencv.org/4.1.1/)
