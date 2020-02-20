#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "clustering.hpp"

int main() {
    srand(0);
    auto m = imread("fox.jpg");
    Mat_<Vec3i> img(m);
    //m.convertTo(img, CV_8UC3);
    auto clusters = getClusters(img, 3);
    for(auto& c : clusters)
        cout << "Created cluster: " << c.getCenter() << endl;
    cout << endl;
    for(int i = 0; i < 3; ++i){
        cout << "Assigning points" << endl;
        assignPoints(img, clusters);
        for(auto& c : clusters) {
            cout << "Cluster has " << c.getPoints().size() << " points" << endl;
            c.moveCenter();
            cout << "New center is " << c.getCenter() << endl;
        }
        cout << endl;
    }
    for(auto& c : clusters)
        c.applyCenter();
    imwrite("fox2.jpg", img);
    return 0;
}
