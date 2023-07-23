#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>

std::mutex mtx;
cv::Mat frame1, frame2;

void readVideoFeed1()
{
    // GStreamer pipeline for IMX219 camera 1
    std::string pipeline = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=1280, height=720, format=NV12, framerate=30/1 ! nvvidconv flip-method=2 ! video/x-raw, width=960, height=540, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";

    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened())
    {
        std::cout << "Error: Unable to open camera 1." << std::endl;
        return;
    }

    while (true)
    {
        cv::Mat temp_frame;
        cap >> temp_frame;
        if (temp_frame.empty())
        {
            break;
        }
        mtx.lock();
        frame1 = temp_frame.clone();
        mtx.unlock();
    }
}

void readVideoFeed2()
{
    // GStreamer pipeline for IMX219 camera 2
    std::string pipeline = "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=1280, height=720, format=NV12, framerate=30/1 ! nvvidconv flip-method=2 ! video/x-raw, width=960, height=540, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

    if (!cap.isOpened())
    {
        std::cout << "Error: Unable to open camera 2." << std::endl;
        return;
    }

    while (true)
    {
        cv::Mat temp_frame;
        cap >> temp_frame;
        if (temp_frame.empty())
        {
            break;
        }
        mtx.lock();
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

        // Check for the 'q' key press to exit the loop
        if (cv::waitKey(1) == 'q')
        {
            break;
        }
    }

    // Wait for both threads to finish
    thread1.join();
    thread2.join();

    cv::destroyAllWindows();

    return 0;
}

