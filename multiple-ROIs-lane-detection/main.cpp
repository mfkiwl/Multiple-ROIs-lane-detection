#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

#include <include/def.hpp>
#include <include/ids.h>
#include <include/kurokesu.hpp>
#include <include/lanedetector.hpp>
#include <include/sharedmemory.hpp>

#ifdef FUNCTION_TIMING
#define INIT_TIMER auto start1 = std::chrono::high_resolution_clock::now();
#define START_TIMER  start1 = std::chrono::high_resolution_clock::now();
#define STOP_TIMER(name)  std::cout << "RUNTIME of " << name << ": " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start1).count() << " us " << std::endl;
#else
#define INIT_TIMER
#define START_TIMER
#define STOP_TIMER(name)
#endif

#ifdef FPS_COUNT
#define INIT_TIMER2 auto start2 = std::chrono::high_resolution_clock::now();
#define START_TIMER2  start2 = std::chrono::high_resolution_clock::now();
#define TIMER2_DIFF ((double)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-start2).count()))
#endif

// Custom handlers
Kurokesu kurokesu;
LaneDetector lanedetector;
SharedMemory shm_lane_points(50002, 5000);
SharedMemory shm_usb_to_send(50003, 32);
SharedMemory shm_watchdog(50004, 32);

<<<<<<< HEAD
pthread_cond_t algorithm_signal = PTHREAD_COND_INITIALIZER;
pthread_mutex_t algorithm_signal_mutex = PTHREAD_MUTEX_INITIALIZER;

=======
>>>>>>> IDS
// Frame for cv::Size info
cv::Mat frame_ref(1.8*CAM_RES_Y, CAM_RES_X, CV_8UC1);
cv::Mat frame_ref_inv(CAM_RES_Y, CAM_RES_X, CV_8UC1);

// Functions declarations
void update_bird_eye(int, void*);
void update_cam(int, void*);

int main()
{
<<<<<<< HEAD
#ifdef FUNCTION_TIMING
    INIT_TIMER
#endif
#ifdef FPS_COUNT
    INIT_TIMER2
#endif
    // Threshold variables
    int thresh_h_low = 30;
    int thresh_h_high = 36;
    int thresh_l_low = 0;
    int thresh_l_high = 220;
    int thresh_s_low = 200;
    int thresh_s_high = 255;
    int thresh_hist = 40;
    int thresh_stop = 50;

=======
>>>>>>> IDS
#ifndef VID_MODE
#ifndef IDS_MODE
    // Camera init
    if(!kurokesu.init())
        return 0;
#else
<<<<<<< HEAD
    ids.create_trackbars();
    pthread_mutex_init(&algorithm_signal_mutex,NULL);
    ids.initialize_camera();
    //ids.setting_auto_params();
    //ids.change_params();

    int x = 0;
    pthread_t frame_thread;
    if (pthread_create(&frame_thread, NULL, camera_thread, &x))
    {
        std::cout<<"Error creating thread"<<std::endl;
    }
    else
    {
         std::cout << "Thread started" << std::endl;
    }
=======
    ids.init();
>>>>>>> IDS
#endif
#else
    // Video read init
    cv::VideoCapture video("/home/mateusz/Videos/sample_selfie/out_1.avi");
    // Check
    if(!video.isOpened())
    {
        std::cout << "Error opening video stream or file!"
                  << "Closing app!" << std::endl;
        return 0;
    }
#endif


    // Init shm
    shm_lane_points.init();
    shm_usb_to_send.init();
    shm_watchdog.init();

    // Declarations of cv::Mat
    cv::Mat frame(CAM_RES_Y, CAM_RES_X, CV_8UC3);
    cv::Mat frame_hls(CAM_RES_Y, CAM_RES_X, CV_8UC3);
    cv::Mat frame_bird(1000, 1000, CV_8UC1);
    std::vector<cv::Mat> frame_split_vec(3);

    cv::Mat frame_h(CAM_RES_Y, CAM_RES_X, CV_8UC1);
    cv::Mat frame_l(CAM_RES_Y, CAM_RES_X, CV_8UC1);
    cv::Mat frame_s(CAM_RES_Y, CAM_RES_X, CV_8UC1);

    cv::Mat frame_binary_h(CAM_RES_Y, CAM_RES_X, CV_8UC1);
    cv::Mat frame_binary_l(CAM_RES_Y, CAM_RES_X, CV_8UC1);
    cv::Mat frame_binary_s(CAM_RES_Y, CAM_RES_X, CV_8UC1);

    cv::Mat frame_morph_h(CAM_RES_Y, CAM_RES_X, CV_8UC1);
    cv::Mat frame_morph_l(CAM_RES_Y, CAM_RES_X, CV_8UC1);
    cv::Mat frame_morph_s(CAM_RES_Y, CAM_RES_X, CV_8UC1);

    cv::Mat frame_data(CAM_RES_Y, CAM_RES_X, CV_8UC3);
    cv::Mat frame_bird_inverse(CAM_RES_Y, CAM_RES_X, CV_8UC1);
    cv::Mat frame_settings(1, 580, CV_8UC1);

#ifdef TEST_SHM
    cv::Mat frame_test_shm(CAM_RES_Y, CAM_RES_X, CV_8UC3);
#endif

// Creations of windows
#ifdef DEBUG_MODE
    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Camera", 0, 0);
    cv::namedWindow("Bird Eye", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Bird Eye", 700, 0);

    cv::namedWindow("Binary H", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Binary H", 0, 385);
    cv::namedWindow("Binary L", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Binary L", 0, 385);
    cv::namedWindow("Binary S", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Binary S", 0, 385);

    cv::namedWindow("Data inverse", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Data inverse", 0, 0);
    cv::namedWindow("Settings", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Settings", 600, 385);

#ifndef VID_MODE
#ifndef IDS_MODE
    cv::namedWindow("CAM Settings", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("CAM Settings", 750, 385);
#endif
#endif
#endif

#ifdef VERBOSE_MODE
    cv::namedWindow("H", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("H", 1300, 200);
    cv::namedWindow("L", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("L", 1300, 200);
    cv::namedWindow("S", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("S", 1300, 300);
    cv::namedWindow("Data", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Data", 1300, 700);
#endif

#ifdef TEST_SHM
    cv::namedWindow("SHM TEST", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("SHM TEST", 1300, 700);
#endif

    // Threshold variables
    int thresh_h_low = 24;
    int thresh_h_high = 28;
    int thresh_l_low = 0;
    int thresh_l_high = 180;
    int thresh_s_low = 50;
    int thresh_s_high = 255;
    int thresh_hist = 100;

    uint32_t h_l_s = 1; // 0 -H, 1 - L, 2 - S

    // Bird Eye first calculation
    lanedetector.calculate_bird_var(frame_ref, frame_ref_inv);

    // Detection areas, count from the bottom to the top of the cv::Mat
    DetectionArea area_line[DETECTION_NUMBER];

    //Init def values
    for(int i = 0; i < DETECTION_NUMBER; i++)
    {
        area_line[i].init(cv::Point(320, 320 - 40*i));
    }

    // Detected points
    std::vector<cv::Point> left_points;
    std::vector<cv::Point> right_points;
    std::vector<cv::Point> cones_points;

    // Scene variables
    bool reset_stm = false;
    bool red_light_visible = false;
    bool green_light_visible = false;

#ifdef DEBUG_MODE
    // UI variables
    char keypressed;
#endif

    // Trackbars
<<<<<<< HEAD
    cv::createTrackbar("f", "Settings", &lanedetector.f_i, 1000, update_trackbar);
    cv::createTrackbar("dst", "Settings", &lanedetector.dist_i, 1000, update_trackbar);
    cv::createTrackbar("alpha", "Settings", &lanedetector.alpha_i, 200, update_trackbar);
    cv::createTrackbar("l low", "Settings", &thresh_l_low, 255, NULL);
    cv::createTrackbar("l high", "Settings", &thresh_l_high, 255, NULL);
    cv::createTrackbar("s low", "Settings", &thresh_s_low, 255, NULL);
    cv::createTrackbar("s high", "Settings", &thresh_s_high, 255, NULL);
=======
#ifdef DEBUG_MODE
#ifndef IDS_MODE
#ifndef VID_MODE
    kurokesu.trackbars();
#endif
#endif
    cv::createTrackbar("f", "Settings", &lanedetector.f_i, 1000, update_bird_eye);
    cv::createTrackbar("dst", "Settings", &lanedetector.dist_i, 500, update_bird_eye);
    cv::createTrackbar("dst_inv", "Settings", &lanedetector.dist_inv_i, 500, update_bird_eye);
    cv::createTrackbar("alpha", "Settings", &lanedetector.alpha_i, 200, update_bird_eye);
    cv::createTrackbar("CUT Y", "Settings", &lanedetector.cut_y, 50, NULL);
    cv::createTrackbar("H low", "Settings", &thresh_h_low, 255, NULL);
    cv::createTrackbar("H high", "Settings", &thresh_h_high, 255, NULL);
    cv::createTrackbar("L low", "Settings", &thresh_l_low, 255, NULL);
    cv::createTrackbar("L high", "Settings", &thresh_l_high, 255, NULL);
    cv::createTrackbar("S low", "Settings", &thresh_s_low, 255, NULL);
    cv::createTrackbar("S high", "Settings", &thresh_s_high, 255, NULL);
>>>>>>> IDS
    cv::createTrackbar("t hist", "Settings", &thresh_hist, 255, NULL);
#endif

    ids.update_params();

#ifdef FPS_COUNT
    // FPS
    struct timespec start, end;
    unsigned int frame_count = 0;
    float seconds = 0;
    float fps = 0;
#endif

    // Pre-scan
    // Get new frame
#ifndef VID_MODE
#ifndef IDS_MODE
<<<<<<< HEAD
    camera >> frame;
=======
    for(int i = 0; i < 30; i++)
        kurokesu.camera >> frame;
>>>>>>> IDS
#else
    for(int i = 0; i < 40; i++)
        ids.get_frame_to(frame);
#endif
#else
<<<<<<< HEAD
//video >> frame;
=======
    video >> frame;
>>>>>>> IDS
#endif
/*...*/ STOP_TIMER("Get new frame")

    // Blur image
    //cv::GaussianBlur(frame, frame_blur, cv::Size(3,3), 0, 0);

/*...*/ START_TIMER
    // Change perspective
    lanedetector.bird_eye(frame, frame_bird);
/*...*/ STOP_TIMER("Bird eye")

/*...*/ START_TIMER
    // Obtain HLS color space image
    cv::cvtColor(frame_bird, frame_hls, cv::COLOR_BGR2HSV);
/*...*/ STOP_TIMER("Color space change")

/*...*/ START_TIMER
    // Split channels
    cv::split(frame_hls, frame_split_vec);
    //frame_h = frame_split_vec[0];
    frame_l = frame_split_vec[1];
    frame_s = frame_split_vec[2];
/*...*/ STOP_TIMER("Channels split")

<<<<<<< HEAD
#ifdef ADAPTIVE_MODE
/*...*/ START_TIMER
//        cv::adaptiveThreshold(frame_l, frame_l_adapt, 255, metoda, typ, blok, c);
    cv::adaptiveThreshold(frame_l, frame_s_adapt, 255, metoda, typ, blok, c);
/*...*/ STOP_TIMER("Adaptive threshold")

/*...*/ START_TIMER
//        lanedetector.opening(frame_l_adapt, frame_l_morph);
    lanedetector.opening(frame_s_adapt, frame_s_morph);
/*...*/ STOP_TIMER("Morph opening")

/*...*/ START_TIMER
    // Calculate new ROI's positions
    for(int i = 0; i < DETECTION_NUMBER; i++)
    {
        // Take new ROI based on previous frame
        area_left_line[i].update(frame_s_morph);
=======
    switch(h_l_s)
    {
    case 0:
    {
        // Binarize channels
        lanedetector.binarize(frame_h, frame_binary_h, thresh_h_low, thresh_h_high);

        // Opening operation
        lanedetector.opening(frame_binary_h, frame_morph_h);

        // Calculate new ROI's positions
        for(int i = 0; i < DETECTION_NUMBER; i++)
        {
            // Take new ROI based on previous frame
            area_line[i].update(frame_morph_h);

            // Detect position of the line relative to the ROI
            area_line[i].find_center(thresh_hist);
>>>>>>> IDS

            // Set standard ROI size
            area_line[i].roi_x = ROI_X;

            // Corect position of ROI if outside cv::Mat
            area_line[i].check_boundaries();
        }

        // Pull back lost ROI's
//        lanedetector.correct_ROIs(area_line, frame_morph_h);
        break;
    }
<<<<<<< HEAD
/*...*/ STOP_TIMER("ROI's positioning")

/*...*/ START_TIMER
    // Pull back lost ROI's
    lanedetector.correct_ROIs(area_left_line, frame_s_morph);
/*...*/ STOP_TIMER("Check for lost ROI's")

/*...*/ START_TIMER
    // Check for stop line
    lanedetector.search_stop_line(area_left_line, frame_s_morph, thresh_stop);
/*...*/ STOP_TIMER("Search STOP line")
#else
    // Binarize channels
    //lanedetector.binarize(frame_h, frame_binary_h, thresh_h_low, thresh_h_high);
    lanedetector.binarize(frame_l, frame_binary_l, thresh_l_low, thresh_l_high);
    cv::bitwise_not(frame_binary_l, frame_binary_l);
    lanedetector.binarize(frame_s, frame_binary_s, thresh_s_low, thresh_s_high);

    // Logical conjunction of H and S channel
    //cv::bitwise_and(frame_binary_h, frame_binary_s, frame_and);

    // Opening operation
    //lanedetector.opening(frame_and, frame_morph);

    // Obtain gradient points
    //lanedetector.apply_sobel(frame_morph, frame_sobel);
=======
    case 1:
    {
        // Binarize channels
        lanedetector.binarize(frame_l, frame_binary_l, thresh_l_low, thresh_l_high);

        // NOT
        cv::bitwise_not(frame_binary_l, frame_binary_l);

        // Opening operation
        lanedetector.opening(frame_binary_l, frame_morph_l);

        // Calculate new ROI's positions
        for(int i = 0; i < DETECTION_NUMBER; i++)
        {
            // Take new ROI based on previous frame
            area_line[i].update(frame_morph_l);

            // Detect position of the line relative to the ROI
            area_line[i].find_center(thresh_hist);
>>>>>>> IDS

            // Set standard ROI size
            area_line[i].roi_x = ROI_X;

            // Corect position of ROI if outside cv::Mat
            area_line[i].check_boundaries();
        }

        // Pull back lost ROI's
//        lanedetector.correct_ROIs(area_line, frame_morph_l);
        break;

    }
    case 2:
    {
<<<<<<< HEAD
        // Take new ROI based on previous frame
        area_left_line[i].update(frame_binary_s);
=======
        // Binarize channels
        lanedetector.binarize(frame_s, frame_binary_s, thresh_s_low, thresh_s_high);

        // Opening operation
        lanedetector.opening(frame_binary_s, frame_morph_s);

        // Calculate new ROI's positions
        for(int i = 0; i < DETECTION_NUMBER; i++)
        {
            // Take new ROI based on previous frame
            area_line[i].update(frame_morph_s);
>>>>>>> IDS

            // Detect position of the line relative to the ROI
            area_line[i].find_center(thresh_hist);

            // Set standard ROI size
            area_line[i].roi_x = ROI_X;

<<<<<<< HEAD
        // Corect position of ROI if outside cv::Mat
        area_left_line[i].check_boundaries();
    }

    // Pull back lost ROI's
    //lanedetector.correct_ROIs(area_left_line, frame_morph);
#endif
#ifdef DEBUG_MODE
    // Draw detection
    lanedetector.draw_data(frame_bird, area_left_line, NULL);
=======
            // Corect position of ROI if outside cv::Mat
            area_line[i].check_boundaries();
        }

        // Pull back lost ROI's
//        lanedetector.correct_ROIs(area_line, frame_morph_s);
        break;
    }
    }

#ifdef DEBUG_MODE
    // Draw detection
    lanedetector.draw_data(frame_bird, area_line);
>>>>>>> IDS

    // Inverse data to original camera perspective
    lanedetector.bird_eye_inverse(frame_bird, frame_bird_inverse);
#endif


    // Pack data for SHM
    lanedetector.pack_data(area_line, left_points);

    // Push output data to SHM
    shm_lane_points.push_lane_data(left_points, right_points, cones_points);
    shm_usb_to_send.push_scene_data(reset_stm, red_light_visible, green_light_visible, lanedetector.stop_detected, lanedetector.stop_distance);
    // End of first scan

     // ========================Main loop ==================================
#ifndef VID_MODE
    while(true)
#else
    for(int i = 1; i < video.get(cv::CAP_PROP_FRAME_COUNT); )
#endif
    {
#ifdef FPS_COUNT
        // FPS
        if(frame_count == 0)
        {
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
#endif
/*...*/ START_TIMER
        // Get new frame
#ifndef VID_MODE
#ifndef IDS_MODE
        kurokesu.camera >> frame;
#else
        ids.get_frame_to(frame);
#endif
#else
<<<<<<< HEAD
    //video >> frame;
=======
        //video >> frame;
>>>>>>> IDS
#endif
/*...*/ STOP_TIMER("Get new frame")

        // Blur image
        //cv::GaussianBlur(frame, frame_blur, cv::Size(3,3), 0, 0);

/*...*/ START_TIMER
        // Change perspective
        lanedetector.bird_eye(frame, frame_bird);
/*...*/ STOP_TIMER("Bird eye")

/*...*/ START_TIMER
        // Obtain HLS color space image
        cv::cvtColor(frame_bird, frame_hls, cv::COLOR_BGR2HSV);
/*...*/ STOP_TIMER("Color space change")

/*...*/ START_TIMER
        // Split channels
        cv::split(frame_hls, frame_split_vec);
        //frame_h = frame_split_vec[0];
        frame_l = frame_split_vec[1];
        frame_s = frame_split_vec[2];
/*...*/ STOP_TIMER("Channels split")

<<<<<<< HEAD
#ifdef ADAPTIVE_MODE
/*...*/ START_TIMER
//        cv::adaptiveThreshold(frame_l, frame_l_adapt, 255, metoda, typ, blok, c);
        cv::adaptiveThreshold(frame_l, frame_s_adapt, 255, metoda, typ, blok, c);
/*...*/ STOP_TIMER("Adaptive threshold")

/*...*/ START_TIMER
//        lanedetector.opening(frame_l_adapt, frame_l_morph);
        lanedetector.opening(frame_s_adapt, frame_s_morph);
/*...*/ STOP_TIMER("Morph opening")

/*...*/ START_TIMER
        // Calculate new ROI's positions
        for(int i = 0; i < DETECTION_NUMBER; i++)
        {
            // Take new ROI based on previous frame
            area_left_line[i].update(frame_s_morph);
=======
        switch(h_l_s)
        {
        case 0:
        {
            // Binarize channels
            lanedetector.binarize(frame_h, frame_binary_h, thresh_h_low, thresh_h_high);

            // Opening operation
            lanedetector.opening(frame_binary_h, frame_morph_h);

            // Calculate new ROI's positions
            for(int i = 0; i < DETECTION_NUMBER; i++)
            {
                // Take new ROI based on previous frame
                area_line[i].update(frame_morph_h);
>>>>>>> IDS

                // Detect position of the line relative to the ROI
                area_line[i].find_center(thresh_hist);

<<<<<<< HEAD
            // Corect position of ROI if outside cv::Mat
            area_left_line[i].check_boundaries();
        }
/*...*/ STOP_TIMER("ROI's positioning")

/*...*/ START_TIMER
        // Pull back lost ROI's
        lanedetector.correct_ROIs(area_left_line, frame_s_morph);
/*...*/ STOP_TIMER("Check for lost ROI's")

/*...*/ START_TIMER
        // Check for stop line
        lanedetector.search_stop_line(area_left_line, frame_s_morph, thresh_stop);
/*...*/ STOP_TIMER("Search STOP line")
#else
        // Binarize channels
        //lanedetector.binarize(frame_h, frame_binary_h, thresh_h_low, thresh_h_high);
        lanedetector.binarize(frame_l, frame_binary_l, thresh_l_low, thresh_l_high);
        cv::bitwise_not(frame_binary_l, frame_binary_l);
        lanedetector.binarize(frame_s, frame_binary_s, thresh_s_low, thresh_s_high);

        // Logical conjunction of H and S channel
        //cv::bitwise_and(frame_binary_h, frame_binary_s, frame_and);

        // Opening operation
        //lanedetector.opening(frame_and, frame_morph);

        // Obtain gradient points
        //lanedetector.apply_sobel(frame_morph, frame_sobel);
=======
                // Set standard ROI size
                area_line[i].roi_x = ROI_X;

                // Corect position of ROI if outside cv::Mat
                area_line[i].check_boundaries();
            }

            // Pull back lost ROI's
            lanedetector.correct_ROIs(area_line, frame_morph_h);
            break;
        }
        case 1:
        {
#ifdef AUTO_THRESH
            // Calculate thresh
            lanedetector.calculate_thresh(frame_l, area_line);
#else
            // Binarize channels
            lanedetector.binarize(frame_l, frame_binary_l, thresh_l_low, thresh_l_high);
#endif
            // NOT
            cv::bitwise_not(frame_binary_l, frame_binary_l);

            // Opening operation
            lanedetector.opening(frame_binary_l, frame_morph_l);

            // Calculate new ROI's positions
            for(int i = 0; i < DETECTION_NUMBER; i++)
            {
                // Take new ROI based on previous frame
                area_line[i].update(frame_morph_l);

                // Detect position of the line relative to the ROI
                area_line[i].find_center(thresh_hist);
>>>>>>> IDS

                // Set standard ROI size
                area_line[i].roi_x = ROI_X;

                // Corect position of ROI if outside cv::Mat
                area_line[i].check_boundaries();
            }

            // Pull back lost ROI's
            lanedetector.correct_ROIs(area_line, frame_morph_l);
            break;

        }
        case 2:
        {
<<<<<<< HEAD
            // Take new ROI based on previous frame
            area_left_line[i].update(frame_binary_s);
=======
            // Binarize channels
            lanedetector.binarize(frame_s, frame_binary_s, thresh_s_low, thresh_s_high);
>>>>>>> IDS

            // Opening operation
            lanedetector.opening(frame_binary_s, frame_morph_s);

            // Calculate new ROI's positions
            for(int i = 0; i < DETECTION_NUMBER; i++)
            {
                // Take new ROI based on previous frame
                area_line[i].update(frame_morph_s);

                // Detect position of the line relative to the ROI
                area_line[i].find_center(thresh_hist);

                // Set standard ROI size
                area_line[i].roi_x = ROI_X;

                // Corect position of ROI if outside cv::Mat
                area_line[i].check_boundaries();
            }

            // Pull back lost ROI's
            lanedetector.correct_ROIs(area_line, frame_morph_s);
            break;
        }
        }

<<<<<<< HEAD
        // Pull back lost ROI's
        //lanedetector.correct_ROIs(area_left_line, frame_morph);
#endif
=======
>>>>>>> IDS
#ifdef DEBUG_MODE
        // Draw detection
        lanedetector.draw_data(frame_bird, area_line);

        // Inverse data to original camera perspective
        lanedetector.bird_eye_inverse(frame_bird, frame_bird_inverse);
#endif

/*...*/ START_TIMER
        // Pack data for SHM
<<<<<<< HEAD
        lanedetector.pack_data(area_left_line, NULL, left_points, right_points);
/*...*/ STOP_TIMER("Pack data for SHM")
=======
        lanedetector.pack_data(area_line, left_points);
>>>>>>> IDS

/*...*/ START_TIMER
        // Push output data to SHM
        shm_lane_points.push_lane_data(left_points, right_points, cones_points);
<<<<<<< HEAD
        shm_usb_to_send.push_scene_data(reset, red_light_visible, green_light_visible, lanedetector.stop_detected, lanedetector.stop_distance);
/*...*/ STOP_TIMER("Push data to SHM")
=======
        shm_usb_to_send.push_scene_data(reset_stm, red_light_visible, green_light_visible, lanedetector.stop_detected, lanedetector.stop_distance);
>>>>>>> IDS

#ifdef TEST_SHM
        shm_lane_points.pull_lane_data(frame_test_shm);
        shm_usb_to_send.pull_scene_data();
#endif

#ifdef DEBUG_MODE
        // Show frames
<<<<<<< HEAD
#ifdef IDS_MODE
        if (++denom>5)
        {
            denom = 0;
#endif
            cv::imshow("Camera", frame);
            cv::imshow("Bird Eye", frame_bird);

#ifdef ADAPTIVE_MODE
            cv::imshow("L morph", frame_l_morph);
            cv::imshow("S morph", frame_s_morph);
#else
        //cv::imshow("Morph and", frame_morph);
        //cv::imshow("Sobel", frame_sobel);
#endif
            cv::imshow("Data inverse", frame_bird_inverse);
            cv::imshow("Settings", frame_settings);

            // Get input from user
            keypressed = (char)cv::waitKey(FRAME_TIME);

#ifdef IDS_MODE
        }
#endif

#ifdef VERBOSE_MODE
#ifdef ADAPTIVE_MODE
        cv::imshow("Blur", frame_blur);
        cv::imshow("H", frame_h);
        cv::imshow("S", frame_s);
#else
        //cv::imshow("H", frame_h);
        //cv::imshow("L", frame_l);
        cv::imshow("Binary l", frame_binary_l);
        cv::imshow("Binary s", frame_binary_s);
        //cv::imshow("AND", frame_and);
#endif
        //cv::imshow("Data", frame_data);
=======
        cv::imshow("Camera", frame);
        cv::imshow("Bird Eye", frame_bird);

        cv::imshow("Binary H", frame_morph_h);
        cv::imshow("Binary L", frame_morph_l);
        cv::imshow("Binary S", frame_morph_s);

        cv::imshow("Data inverse", frame_bird_inverse);
        cv::imshow("Settings", frame_settings);

#ifndef VID_MODE
#ifndef IDS_MODE
        cv::imshow("CAM Settings", frame_settings);
#endif
#endif

#ifdef VERBOSE_MODE
        cv::imshow("H", frame_h);
        cv::imshow("L", frame_l);
        cv::imshow("S", frame_s);
        cv::imshow("Data", frame_data);
        frame_data = cv::Mat::zeros(CAM_RES_Y, CAM_RES_X, CV_8UC3);
>>>>>>> IDS
#endif

#ifdef TEST_SHM
        cv::imshow("SHM TEST", frame_test_shm);
        frame_test_shm = cv::Mat::zeros(CAM_RES_Y, CAM_RES_X, CV_8UC3);
#endif

<<<<<<< HEAD
        // Reset from Futaba
        //std::cout << "RESET: " << shm_usb_to_send.shared_variable[1] << std::endl; //++++++++++++++++++++++++++++++
        if(shm_usb_to_send.shared_variable[1] > 0)
=======
        // Get input from user
        keypressed = (char)cv::waitKey(FRAME_TIME);

        // Process given input
        if( keypressed == 27 )
            break;

        switch(keypressed)
        {
        // Reset ROI positions
        case 'r':
>>>>>>> IDS
        {
            // Reset
            for(int i = 0; i < DETECTION_NUMBER; i++)
            {
                area_line[i].reset();
            }

            // Pre-scan
            // Get new frame
#ifndef VID_MODE
#ifndef IDS_MODE
            kurokesu.camera >> frame;
#else
            ids.get_frame_to(frame);
#endif
#else
            video >> frame;
#endif
            // Change perspective
            //cv::GaussianBlur(frame, frame_blur, cv::Size(3,3), 0, 0);
            lanedetector.bird_eye(frame, frame_bird);

            // Obtain HLS color space image
            cv::cvtColor(frame_bird, frame_hls, cv::COLOR_BGR2HLS);

            // Split channels
            cv::split(frame_hls, frame_split_vec);
            frame_h = frame_split_vec[0];
            frame_l = frame_split_vec[1];
            frame_s = frame_split_vec[2];

            switch(h_l_s)
            {
            case 0:
            {
                // Binarize channels
                lanedetector.binarize(frame_h, frame_binary_h, thresh_h_low, thresh_h_high);

                // Opening operation
                lanedetector.opening(frame_binary_h, frame_morph_h);

<<<<<<< HEAD
                // Corect position of ROI if outside cv::Mat
                area_left_line[i].check_boundaries();
            }

            // Pull back lost ROI's
            //lanedetector.correct_ROIs(area_left_line, frame_morph);
#endif
#ifdef DEBUG_MODE
            // Draw detection
            //lanedetector.draw_data(frame_bird, area_left_line, NULL);
=======
                // Calculate new ROI's positions
                for(int i = 0; i < DETECTION_NUMBER; i++)
                {
                    // Take new ROI based on previous frame
                    area_line[i].update(frame_morph_h);

                    // Detect position of the line relative to the ROI
                    area_line[i].find_center(thresh_hist);
>>>>>>> IDS

                    // Set standard ROI size
                    area_line[i].roi_x = ROI_X;

                    // Corect position of ROI if outside cv::Mat
                    area_line[i].check_boundaries();
                }

                // Pull back lost ROI's
        //        lanedetector.correct_ROIs(area_line, frame_morph_h);
                break;
            }
            case 1:
            {
                // Binarize channels
                lanedetector.binarize(frame_l, frame_binary_l, thresh_l_low, thresh_l_high);

                // NOT
                cv::bitwise_not(frame_binary_l, frame_binary_l);

                // Opening operation
                lanedetector.opening(frame_binary_l, frame_morph_l);

                // Calculate new ROI's positions
                for(int i = 0; i < DETECTION_NUMBER; i++)
                {
                    // Take new ROI based on previous frame
                    area_line[i].update(frame_morph_l);

                    // Detect position of the line relative to the ROI
                    area_line[i].find_center(thresh_hist);

                    // Set standard ROI size
                    area_line[i].roi_x = ROI_X;

                    // Corect position of ROI if outside cv::Mat
                    area_line[i].check_boundaries();
                }

                // Pull back lost ROI's
        //        lanedetector.correct_ROIs(area_line, frame_morph_l);
                break;
            }
            case 2:
            {
                // Binarize channels
                lanedetector.binarize(frame_s, frame_binary_s, thresh_s_low, thresh_s_high);

                // Opening operation
                lanedetector.opening(frame_binary_s, frame_morph_s);

                // Calculate new ROI's positions
                for(int i = 0; i < DETECTION_NUMBER; i++)
                {
                    // Take new ROI based on previous frame
                    area_line[i].update(frame_morph_s);

                    // Detect position of the line relative to the ROI
                    area_line[i].find_center(thresh_hist);

                    // Set standard ROI size
                    area_line[i].roi_x = ROI_X;

                    // Corect position of ROI if outside cv::Mat
                    area_line[i].check_boundaries();
                }

<<<<<<< HEAD
                // Corect position of ROI if outside cv::Mat
                area_left_line[i].check_boundaries();
            }

            // Pull back lost ROI's
            //lanedetector.correct_ROIs(area_left_line, frame_morph);
        #endif
        #ifdef DEBUG_MODE
=======
                // Pull back lost ROI's
        //        lanedetector.correct_ROIs(area_line, frame_morph_s);
                break;
            }
            }

#ifdef DEBUG_MODE
>>>>>>> IDS
            // Draw detection
            lanedetector.draw_data(frame_bird, area_line);

            // Inverse data to original camera perspective
            lanedetector.bird_eye_inverse(frame_bird, frame_bird_inverse);
#endif

            // Pack data for SHM
            lanedetector.pack_data(area_line, left_points);

            // Push output data to SHM
            shm_lane_points.push_lane_data(left_points, right_points, cones_points);
            shm_usb_to_send.push_scene_data(reset_stm, red_light_visible, green_light_visible, lanedetector.stop_detected, lanedetector.stop_distance);
            // End of first scan

            break;
        }
        // Change side on which line is searched
        case 'c':
        {
            h_l_s++;

            if(h_l_s >=3)
            {
                h_l_s = 0;
            }
            break;
        }

        case'q':
        {
            system("v4l2-ctl -d /dev/video1 --set-ctrl=gamma=500");
            break;
        }
        case 'u':
        {
            kurokesu.update();
        }
        }
#endif

#ifdef FPS_COUNT
        // FPS
        if(frame_count > FPS_AMOUNT)
        {
            frame_count = 0;
            clock_gettime(CLOCK_MONOTONIC, &end);
            seconds = (end.tv_sec - start.tv_sec);
            fps  =  1 / (seconds / FPS_AMOUNT);

            std::cout <<"FPS: " << fps << std::endl;
        }
        else
        {
            frame_count++;
        }
#endif
    }

    // Clean up
#ifndef VID_MODE
    kurokesu.camera.release();
#else
    video.release();
#endif

    shm_lane_points.close();
    shm_usb_to_send.close();
    return 0;
}

void update_bird_eye(int, void*)
{
    lanedetector.calculate_bird_var(frame_ref, frame_ref_inv);
}
