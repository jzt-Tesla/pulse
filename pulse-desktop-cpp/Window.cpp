#include "Window.hpp"
#include "EvmGdownIIR.hpp"
#include "ext_opencv.hpp"

Window::Window(EvmGdownIIR& evm) :
evm(evm),
WINDOW_NAME("EVM"),
TRACKBAR_BLUR_NAME("Blur level"),
TRACKBAR_F_HIGH_NAME("High cut-off"),
TRACKBAR_F_LOW_NAME("Low cut-off"),
TRACKBAR_ALPHA_NAME("Amplification"),
TRACKBAR_BLUR(evm.blurLevel),
TRACKBAR_F_HIGH(evm.fHigh * 600),
TRACKBAR_F_LOW(evm.fLow * 600),
TRACKBAR_ALPHA(evm.alpha) {
    namedWindow(WINDOW_NAME);
    createTrackbar(TRACKBAR_BLUR_NAME, WINDOW_NAME, &TRACKBAR_BLUR, 8);
    createTrackbar(TRACKBAR_F_HIGH_NAME, WINDOW_NAME, &TRACKBAR_F_HIGH, 240);
    createTrackbar(TRACKBAR_F_LOW_NAME, WINDOW_NAME, &TRACKBAR_F_LOW, 240);
    createTrackbar(TRACKBAR_ALPHA_NAME, WINDOW_NAME, &TRACKBAR_ALPHA, 1000);
}

Window::~Window() {
}

void Window::update(Mat& frame) {
    if (evm.blurLevel != TRACKBAR_BLUR) {
        evm.start(frame.cols, frame.rows);
    }
    evm.blurLevel = TRACKBAR_BLUR;
    evm.fHigh = TRACKBAR_F_HIGH / 600.;
    evm.fLow = TRACKBAR_F_LOW / 600.;
    evm.alpha = TRACKBAR_ALPHA;

    evm.onFrame(frame, frame);

    showTrackbarValues(frame);
    showFPS(frame);

    imshow(WINDOW_NAME, frame);
}

void Window::showTrackbarValues(Mat& frame) {
    stringstream ss;

    ss << TRACKBAR_BLUR_NAME << ": " << TRACKBAR_BLUR;
    putText(frame, ss.str(), Point(10, 30), FONT_HERSHEY_PLAIN, 1, BLUE);

    ss.str("");

    ss << TRACKBAR_F_HIGH_NAME << ": " << TRACKBAR_F_HIGH;
    putText(frame, ss.str(), Point(10, 45), FONT_HERSHEY_PLAIN, 1, BLUE);

    ss.str("");

    ss << TRACKBAR_F_LOW_NAME << ": " << TRACKBAR_F_LOW;
    putText(frame, ss.str(), Point(10, 60), FONT_HERSHEY_PLAIN, 1, BLUE);

    ss.str("");

    ss << TRACKBAR_ALPHA_NAME << ": " << TRACKBAR_ALPHA;
    putText(frame, ss.str(), Point(10, 75), FONT_HERSHEY_PLAIN, 1, BLUE);
}

void Window::showFPS(Mat& frame) {
    static int frameCounter = 0;
    static int lastFrameCounter = 0;
    static double lastFpsTime = (double)getTickCount();
    static string fpsStr;
    static Point point(10, 15);

    frameCounter++;

    if ((frameCounter % 30) != 0) {
        putText(frame, fpsStr, point, FONT_HERSHEY_PLAIN, 1, BLUE);
    } else {
        double now = (double)getTickCount();
        double diff = (now - lastFpsTime) * 1000. / getTickFrequency();

        if (diff > 0) {
            double fps = (frameCounter - lastFrameCounter) * 1000 / diff;
            lastFrameCounter = frameCounter;
            lastFpsTime = now;

            stringstream ss;
            ss.precision(3);
            ss << "FPS: " << fps;
            fpsStr = ss.str();

            putText(frame, fpsStr, point, FONT_HERSHEY_PLAIN, 1, BLUE);
        }
    }
}
