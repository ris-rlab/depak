#Display gryo and accel raw data from RealSense D435i

import pyrealsense2 as rs
import numpy as np
import cv2
from pprint import pprint
import os

def pose_cb(frame):
    
    motion = frame.as_motion_frame()
    
    if (motion and (motion.get_profile().stream_type() == rs.stream.gyro) and (motion.get_profile().format() == rs.format.motion_xyz32f)):
        print('gyro', flush=true)
        ts = motion.get_timestamp()
        gyro_data = motion.get_motion_data()        
        print(gyro_data.x)
    
    if (motion and (motion.get_profile().stream_type() == rs.stream.accel) and (motion.get_profile().format() == rs.format.motion_xyz32f)):
        print('accel')
        accel_data = motion.get_motion_data()
        print(accel_data.x)
    
# Configure depth and color streams
pipeline = rs.pipeline()
config = rs.config()

print('Available streams:')
# pprint(dir(rs.stream))

# config.enable_stream(rs.stream.infrared, 1)
# config.enable_stream(rs.stream.infrared, 2)
# config.enable_stream(rs.stream.depth)
config.enable_stream(rs.stream.gyro) # Positional data (translation, rotation, velocity etc)
config.enable_stream(rs.stream.accel) # Positional data (translation, rotation, velocity etc)


# config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

# Start streaming
selection = pipeline.start(config, pose_cb)



# while(1): i
