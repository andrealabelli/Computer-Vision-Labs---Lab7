#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

    const int num_images = 5;

    // Check if path to image is provided correctly as command line argument
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return -1;
    }

    /*Mat img = imread(argv[1], IMREAD_GRAYSCALE);

    namedWindow("img0", WINDOW_KEEPRATIO);
    imshow("img0", img);

    Size patternSize(6, 5);    // Chessboard pattern

    vector<Point2f> corners;

    bool found = findChessboardCorners(img, patternSize, corners);

    if (found) {
    drawChessboardCorners(img, patternSize, Mat(corners), found);
    imshow("Detected Corners", img);
    }*/

    Size patternSize(6, 5);

    float squareSize = 10.0;


    // Prepare object points
    vector<Point3f> objectPoints;
    for (int i = 0; i < patternSize.height; i++) {
        for (int j = 0; j < patternSize.width; j++) {
            objectPoints.push_back(Point3f(j * squareSize, i * squareSize, 0.0f));
        }
    }

    string folderPath = argv[1];

    Mat images[num_images];
    
    for (int i = 0; i < num_images; i++) {
        string filename = "img" + to_string(i) + ".jpg"; // Genera il nome del file dell'immagine
        images[i] = imread(folderPath + "/" + filename, IMREAD_COLOR); // Legge l'immagine da file

        if (images[i].empty()) {
            cout << "Errore durante la lettura dell'immagine " << filename << endl;
            return 1;
        }
    }
    
    vector<vector<Point2f>> imgPoints;
    Mat cameraMatrix, distCoeffs;

    for (int i = 0; i < num_images; i++) {

        vector<Point2f> corners;

        bool found = findChessboardCorners(images[i], patternSize, corners);
        
        //imshow("img" + to_string(i), images[i]); // Mostra l'immagine corrente
        
        if (found) {
        imgPoints.push_back(corners);
        drawChessboardCorners(images[i], patternSize, Mat(corners), found);
        imshow("Detected Intersections", images[i]);
        waitKey(0);
        }


    }


        // Calibrate the camera
        vector<Mat> rvecs, tvecs;
        double imageReprojectionError = 0, sumReprojectionError = 0;
        double cal = calibrateCamera(objectPoints, imgPoints, images[5].size(), cameraMatrix, distCoeffs, rvecs, tvecs);

        // Print the camera matrix and distortion coefficients
        cout << "Camera Matrix:\n" << cameraMatrix << endl;
        cout << "Calibration value" << cal << endl;



        /*for(int j = 0; j < num_images; j++)
        {

        // Compute the reprojection errors
        vector<Point2f> projectedPoints;
        projectPoints(objectPoints, rvecs[j], tvecs[j], cameraMatrix, distCoeffs, projectedPoints);

        // Accumulate the mean reprojection errors
        sumReprojectionError = imageReprojectionError++;

        // Compute the mean reprojection error for the current image
        imageReprojectionError += norm(imgPoints[j], projectedPoints, NORM_L2) / imgPoints[j].size();

        }*/

        
        cout << "Distortion Coefficients:\n" << distCoeffs << endl;


        // Compute the overall mean reprojection error
        double meanReprojectionError = sumReprojectionError/num_images;

        cout << "Mean reprojection error: " << + meanReprojectionError << endl;


    //waitKey(0);

    return 0;

}