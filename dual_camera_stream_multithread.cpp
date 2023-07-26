#include <opencv2opencv.hpp>
#include <thread>
#include <mutex>
#include <stdio.h>
#include <atomic>
std::mutex mtx;
cv::Mat frame1, frame2;
std::atomic<bool> stopFlag(false);

    int sensor_id = 0;
    int stread_width = 1280;
    int stream_height = 720;
    int frame_rate = 30;
    int flip_method = 2;
    int display_width = 960;
    int display_height = 540;


void readVideoFeed1()
{
    // GStreamer pipeline for IMX219 camera 1
    std::string pipeline = "nvarguscamerasrc sensor-id=" + std::to_string(sensor_id) + " ! video/x-raw(memory:NVMM), width=" + std::to_string(stread_width) + ", height=" + std::to_string(stream_height) + ", format=NV12, framerate=" + std::to_string(frame_rate) + "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=" + std::to_string(display_width) + ", height=" + std::to_string(display_height) + ", format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened())
    {
        std::cout << "Error: Unable to open camera 1." << std::endl;
        return;
    }

    while (true && !stopFlag.load())
    {
        cv::Mat temp_frame;
        cap >> temp_frame;
        
        if (temp_frame.empty())
        {
            break;
        }
       
        mtx.lock();
       

        if(cv::waitKey(1)=='a')
        {
           cv::imwrite("Captured_Image1.jpg",temp_frame);
           std::cout << "Captured Image 1 " << std::endl;
           cv::imshow("Captured Image 1",temp_frame);
        }
       
        frame1 = temp_frame.clone();
        mtx.unlock();
    }
}

void readVideoFeed2()
{
   std::string pipeline = "nvarguscamerasrc sensor-id=" + std::to_string(sensor_id+1) + " ! video/x-raw(memory:NVMM), width=" + std::to_string(stread_width) + ", height=" + std::to_string(stream_height) + ", format=NV12, framerate=" + std::to_string(frame_rate) + "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=" + std::to_string(display_width) + ", height=" + std::to_string(display_height) + ", format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened())
    {
        std::cout << "Error: Unable to open camera 2." << std::endl;
        return;
    }
    
    while (true && !stopFlag.load())
    {
        cv::Mat temp_frame;
        cap >> temp_frame;
   
        if (temp_frame.empty())
        {
            break;
        }
        mtx.lock();

//        if(cv::waitKey(1)=='a')
//        {
//           cv::imwrite("Captured_Image1.jpg",temp_frame);
//           std::cout << "Captured Image 1 " << std::endl;
//           cv::imshow("Captured Image 1",temp_frame);
//        }

        frame2 = temp_frame.clone();
        mtx.unlock();
    }
}

int main()
{
    // Create a window to display the combined video feeds
    cv::namedWindow("Two Video Feeds", cv::WINDOW_AUTOSIZE);

    // Start two separate threads to read video feeds
    std::thread thread1(readVideoFeed1);
    std::thread thread2(readVideoFeed2);

    while (true)
    {
        mtx.lock();
        // Check if frames have valid dimensions
        if (!frame1.empty() && !frame2.empty() && frame1.cols > 0 && frame1.rows > 0 && frame2.cols > 0 && frame2.rows > 0)
        {
            cv::Mat side_by_side;
            cv::hconcat(frame1, frame2, side_by_side);

            // Display the combined video feed
            cv::imshow("Two Video Feeds", side_by_side);
        }
        mtx.unlock();

        char key = cv::waitKey(1);

        if(key == 'q')
        {
            cv::destroyAllWindows();
            stopFlag.store(true);
            //exit(0);
            break;
        }
        if(key == 'a')
        {
            cv::imwrite("captured_image1.jpg",frame1);
            cv::imwrite("captured_image2.jpg",frame2);
            std::cout <<"Images captured" << std::endl;
            cv::imshow("Caputred image 1 ",frame1);
            cv::imshow("Captured image 2 ",frame2);
        }

        // Check for the 'q' key press to exit the loop
        //if (cv::waitKey(1) == 'q')
        //{
        //    break;
        //}


    }

    // Wait for both threads to finish
    thread1.join();
    thread2.join();
//    camera.release();
//    cv::destroyAllWindows();
    printf("Program complete....\r\n"); 

    return 0;
}

