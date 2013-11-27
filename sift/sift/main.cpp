#include <tchar.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <iostream>

using namespace std;

float dist(const cv::Point2f& p1, const cv::Point2f& p2);

int main(int args, TCHAR* argv[])
{
	using namespace cv;
	initModule_nonfree();

    const Mat input = imread(argv[1], 0); //Load as grayscale

	Mat transposed;
	transpose(input, transposed);
	
	SiftFeatureDetector detector;
    
	std::vector<KeyPoint> keypoints;
    detector.detect(input, keypoints);

    std::vector<KeyPoint> transposedKeypoints;
    detector.detect(transposed, transposedKeypoints);

	// get key points
	SurfDescriptorExtractor extractor;
	Mat descriptors1, descriptors2;
	extractor.compute(input,      keypoints,           descriptors1);
	extractor.compute(transposed, transposedKeypoints, descriptors2);

	// create matches
    BFMatcher matcher(NORM_L2);
    vector<DMatch> matches;

	vector<vector<DMatch> > knnMatches;
	vector<DMatch> goodMatches;
	// knn match, 3 points
	// choose best from distance
	matcher.knnMatch(descriptors1, descriptors2, knnMatches, 3);
	for(size_t i = 0; i < knnMatches.size(); ++i)
	{
		KeyPoint from  = keypoints[knnMatches[i][0].queryIdx];
		Point2f  ideal(from.pt.y, from.pt.x);

		float minD      = dist(ideal, transposedKeypoints[knnMatches[i][0].trainIdx].pt);
		size_t idealIdx = 0;
		for(size_t j = 1; j < knnMatches[i].size(); ++j)
		{
			KeyPoint kp = transposedKeypoints[knnMatches[i][j].trainIdx];
			float tmpD = dist(kp.pt, ideal);
			if (tmpD < minD)
			{
				minD = tmpD;
				idealIdx = j;
			}
		}
		goodMatches.push_back(knnMatches[i][idealIdx]);
		goodMatches.back().distance = -minD;
	}

	matches = goodMatches;

	sort(matches.begin(), matches.end());

	vector<DMatch>        visibleMatches;
	std::vector<KeyPoint> visibleFromKeypoints;
	std::vector<KeyPoint> visibleToKeypoints;

	for (int i = matches.size() - 1; i >= 0 && visibleMatches.size() < 25; --i)
	{
		visibleFromKeypoints.push_back(keypoints[matches[i].queryIdx]);
		visibleToKeypoints.push_back(transposedKeypoints[matches[i].trainIdx]);

		matches[i].queryIdx = visibleFromKeypoints.size() - 1;
		matches[i].trainIdx = visibleToKeypoints.size() - 1;

		visibleMatches.push_back(matches[i]);
	}

	cout << "TOTAL MATCHES: " << matches.size() << endl;

	int count = 0;
	for (count = matches.size() - 1; count >= 0 && abs(matches[count].distance) < 1.0f; --count);
	count = matches.size() - count;

	cout << "Matches with distance < 1.0f: " << count << endl;

    namedWindow("matches", 1);
    Mat img_matches;
	drawMatches(input, visibleFromKeypoints, transposed, visibleToKeypoints, visibleMatches, img_matches);
    imshow("matches", img_matches);
    waitKey(0);

	imwrite("out" + std::string(argv[1]), img_matches);

	return 0;
}

float dist(const cv::Point2f& p1, const cv::Point2f& p2)
{
	cv::Point2f diff = p1 - p2;
	return sqrt(diff.x * diff.x + diff.y * diff.y);
}