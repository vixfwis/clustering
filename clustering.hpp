#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <random>
using namespace cv;
using namespace std;

template<typename T, int cn>
class Cluster{
    Mat_<Vec<T, cn>> *img;
    Vec<T, cn> center;
    vector<Vec<T, 2>> points;

public:
    Cluster(Mat_<Vec<T, cn>>& img, int x, int y){
        this->img = &img;
        this->center = img(x, y);
    }
    virtual ~Cluster(){};

    void addPoint(int x, int y){
        points.emplace_back(Vec<T, 2>(x, y));
    }

    void resetPoints(){
        points.clear();
    }

    void moveCenter(){
        Vec<uint64_t , cn> buf;
        for(auto& pt : points)
            for(int i = 0; i < cn; ++i)
                buf[i] += (*img)(pt[0], pt[1])[i];
        buf /= static_cast<int>(points.size());
        for(int i = 0; i < cn; ++i)
            center[i] = saturate_cast<T>(buf[i]);
    }

    void applyCenter(){
        for(auto pt : points)
            (*img)(pt[0], pt[1]) = center;
    }

    double getDistance(int x, int y){
        return norm(center, (*img)(x, y));
    }



    const Vec<T, cn>& getCenter() {
        return center;
    }

    void setCenter(Vec<T, cn>& pt){
        center = pt;
    }

    const vector<Vec<T, 2>> getPoints(){
        return points;
    }
};

template <typename T, int cn>
vector<Cluster<T, cn>> getClusters(Mat_<Vec<T, cn>>& img, int count){
    vector<Cluster<T, cn>> out;
    for(int i = 0; i < count; ++i)
        out.emplace_back(Cluster<T, cn>(img, random() % img.cols, random() % img.rows));
    return out;
}

template <typename T, int cn>
void assignPoints(Mat_<Vec<T, cn>>& img, vector<Cluster<T, cn>>& clusters){
    for(auto& c : clusters)
        c.resetPoints();
    for(int col = 0; col < img.cols; ++col) {
        for(int row = 0; row < img.rows; ++row) {
            auto pt = img(row, col);
            vector<double> distance(clusters.size());
            for(int c = 0; c < distance.size(); ++c)
                distance[c] = clusters[c].getDistance(row, col);
            int min_id[] = {0, 0};
            minMaxIdx(distance, nullptr, nullptr, min_id);
            clusters[min_id[1]].addPoint(row, col);
        }
    }
}
