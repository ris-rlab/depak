

import pyrealsense2 as rs
import numpy as np
import cv2

# Configure depth and color streams
pipeline = rs.pipeline()
config = rs.config()



config.enable_stream(rs.stream.infrared, 1)
config.enable_stream(rs.stream.infrared, 2)
config.enable_stream(rs.stream.depth)

# config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

# Start streaming
selection = pipeline.start(config)
selected_device = selection.get_device()
depth_sensor = selected_device.first_depth_sensor();

print(depth_sensor.get_depth_scale())


#Disable Laser Sener
if(depth_sensor.supports(rs.option.emitter_enabled)):
    depth_sensor.set_option(rs.option.emitter_enabled, 0)
    
if (depth_sensor.supports(rs.option.laser_power)):

    depth_sensor.set_option(rs.option.laser_power, 0);



def feature_surf(imgN):
    img = imgN
    #gray= cv2.cvtColor(imgN,cv2.COLOR_BGR2GRAY)
    sift = cv2.xfeatures2d.SURF_create()
    (kps,descs) = sift.detectAndCompute(imgN,None)
    img=cv2.drawKeypoints(imgN,kps,img)
    # cv2.imwrite('sift_keypoints.jpg',img)
#     print("# kps: {}, descriptors: {}".format(len(kps), descs.shape))
    return img

try:
    while True:

        # Wait for a coherent pair of frames: depth and color
        frames = pipeline.wait_for_frames()
        depth_frame = frames.get_depth_frame()
        ir_frame1 = frames.get_infrared_frame(index=1)
        ir_frame2 = frames.get_infrared_frame(index=2)
#         color_frame = frames.get_color_frame()
        if not ir_frame2 or not ir_frame1:
            continue

        # Convert images to numpy arrays
        depth_image = np.asanyarray(depth_frame.get_data())
        ir_frame1 = np.asanyarray(ir_frame1.get_data())
        ir_frame2 = np.asanyarray(ir_frame2.get_data())
#         color_image = np.asanyarray(color_frame.get_data())

#         # Apply colormap on depth image (image must be converted to 8-bit per pixel first)
        #depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.05), cv2.COLORMAP_JET)

        # Stack both images horizontally
#       images = np.hstack(( ir_frame1, depth_colormap))

        # Show images
        feature = feature_surf(ir_frame1)
        cv2.namedWindow('RealSense', cv2.WINDOW_AUTOSIZE)
        cv2.imshow('RealSense', depth_colormap)
        cv2.imshow('IR stream', ir_frame1)
        cv2.imshow('SURF', feature)
        cv2.waitKey(1)

finally:

    # Stop streaming
    pipeline.stop()
