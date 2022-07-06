#include "keyframe.h"

namespace Data
{

    KeyFrame::KeyFrame(std::shared_ptr<Data::Frame> frame)
    :mpFrame(frame)
    {
        this->setDescriptors(frame->getDescriptors());
        this->setEssentialMatrix(frame->getEssentialMatrix());
        this->setFrame(frame->getFrame());
        this->setFramePoint2d(frame->getFramePoint2d());
        this->setFramePoint3d(frame->getFramePoint3d());
        this->setFundamentalMatrix(frame->getFundamentalMatrix());
        this->setGoodMatches(frame->getGoodMatches());
        this->setRotationMatrix(frame->getRotationMatrix());
        this->setScale(frame->getScale());
        this->setTranslationMatrix(frame->getTranslationMatrix());
    };



    // void KeyFrame::setKeyFrame(std::shared_ptr<Data::Frame> frame)
    // {
    //     mpFrame = frame;
    // }

    std::shared_ptr<Data::Frame> KeyFrame::getKeyFrame()
    {
        return mpFrame;
    }


    void KeyFrame::setReprojPoints(std::vector<cv::KeyPoint> reprojPoints)
    {
        mvReprojPoints = reprojPoints;
    }

    std::vector<cv::KeyPoint> KeyFrame::getReprojPoints()
    {
        return mvReprojPoints;
    }

    void KeyFrame::setPose_cw(const cv::Mat& T_cw)
    {
        mmT_cw = T_cw;
    }

    cv::Mat KeyFrame::getPose_cw()
    {
        return mmT_cw;
    }


    void KeyFrame::setPose_wc(const cv::Mat& T_wc)
    {
        mmT_wc = T_wc;
    }

    cv::Mat KeyFrame::getPose_wc()
    {
        return mmT_wc;
    }

    void KeyFrame::setPose_lr(const cv::Mat& T_lr)
    {
        mmT_lr = T_lr;
    }
    cv::Mat KeyFrame::getPose_lr()
    {
        return mmT_lr;
    }
}
