#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../clustering.hpp"

TEST_CASE("Cluster class check", "[clustering]"){
    srand(0);
    Mat_<Vec3b> img(3, 3, Vec3b(10, 10, 10));
    for(int i = 0; i < 3; ++i)
        img(2, i) = Vec3b(100, 100, 100);

    SECTION("getClusters returns vector of clusters"){
        auto v = getClusters(img, 3);
        REQUIRE(v.size() == 3);
    }

    SECTION("getDistance returns distance between center and point"){
        Cluster<uint8_t, 3> c(img, 0, 0);
        c.setCenter(img(1, 1));
        REQUIRE(c.getDistance(2, 0) == Approx(155.88).epsilon(0.01));
    }

    SECTION("moveCenter changes center to average of added points values"){
        Cluster<uint8_t, 3> c(img, 0, 0);
        c.addPoint(0, 0); // 10
        c.addPoint(2, 0); // 100
        c.addPoint(2, 1); // 100
        c.addPoint(2, 2); // 100
        c.moveCenter();
        Vec<uint8_t, 3> ref(77, 77, 77);
        REQUIRE(c.getCenter() == ref);
    }

    SECTION("assignPoints distributes image points between clusters based on distance (closest)"){
        vector<Cluster<uint8_t, 3>> v = {
            Cluster<uint8_t, 3>(img, 0, 0),
            Cluster<uint8_t, 3>(img, 2, 2)
        };
        assignPoints(img, v);

        auto c10pts = v[0].getPoints();
        REQUIRE(std::count(c10pts.begin(), c10pts.end(), Vec2i(0, 0)));
        REQUIRE(std::count(c10pts.begin(), c10pts.end(), Vec2i(0, 1)));
        REQUIRE(std::count(c10pts.begin(), c10pts.end(), Vec2i(0, 2)));
        REQUIRE(std::count(c10pts.begin(), c10pts.end(), Vec2i(1, 0)));
        REQUIRE(std::count(c10pts.begin(), c10pts.end(), Vec2i(1, 1)));
        REQUIRE(std::count(c10pts.begin(), c10pts.end(), Vec2i(1, 2)));

        auto c100pts = v[1].getPoints();
        REQUIRE(std::count(c100pts.begin(), c100pts.end(), Vec2i(2, 0)));
        REQUIRE(std::count(c100pts.begin(), c100pts.end(), Vec2i(2, 1)));
        REQUIRE(std::count(c100pts.begin(), c100pts.end(), Vec2i(2, 2)));
    }

    SECTION("applyCenter changes every assigned point to value of the center"){
        Cluster<uint8_t, 3> c1(img, 0, 0);
        Cluster<uint8_t, 3> c2(img, 2, 2);
        c1.addPoint(2, 1);
        c1.addPoint(1, 1);
        c2.addPoint(0, 1);
        c2.addPoint(0, 2);
        c1.applyCenter();
        c2.applyCenter();

        REQUIRE(img(2, 1) == Vec<uint8_t, 3>(10, 10, 10));
        REQUIRE(img(1, 1) == Vec<uint8_t, 3>(10, 10, 10));
        REQUIRE(img(0, 1) == Vec<uint8_t, 3>(100, 100, 100));
        REQUIRE(img(0, 2) == Vec<uint8_t, 3>(100, 100, 100));
    }
}
