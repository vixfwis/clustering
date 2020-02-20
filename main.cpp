#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "clustering.hpp"

//#define DEBUG
#include "debug.h"

int main() {
    srand(0);
    auto m = imread("fox.jpg", IMREAD_COLOR);
    Mat_<Vec3b> img(m);
    auto clusters = getClusters(img, 3);
    for(auto& c : clusters) {
        d("Created cluster:", c.getCenter());
    }
    d("");
    for(int i = 0; i < 100; ++i){
        d("Assigning points");
        assignPoints(img, clusters);
        for(auto& c : clusters) {
            d("Cluster has", c.getPoints().size(), "points");
            c.moveCenter();
            d("New center is", c.getCenter());
        }
        d("");
    }
    d("Applying center colors to image");
    for(auto& c : clusters)
        c.applyCenter();
    imwrite("fox2.jpg", img);
    return 0;
}
