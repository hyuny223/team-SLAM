#include "framepoint.h"

namespace Data
{

    void FramePoint::setFramePoint3d(std::vector<cv::Point3d> framePoints)
    {
        mvFramePoints3d = framePoints;
    }


    std::vector<cv::Point3d> FramePoint::getFramePoint3d()
    {
        return mvFramePoints3d;
    }


    void FramePoint::addFramePoint2d(std::vector<cv::KeyPoint> framePoints)
    {
        mvFramePoints2d.insert(mvFramePoints2d.end(), framePoints.begin(), framePoints.end());
    }

    void FramePoint::setFramePoint2d(std::vector<cv::KeyPoint> framePoints)
    {
        mvFramePoints2d = framePoints;
    }

    std::vector<cv::KeyPoint> FramePoint::getFramePoint2d()
    {
        return mvFramePoints2d;
    }
}
