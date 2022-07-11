#pragma once

#include "opencv2/opencv.hpp"
#include "ceres/ceres.h"
#include "glog/logging.h"
#include "Eigen/Dense"
#include "projection.h"

class Pose2dErrorTerm
{
    private:
        std::shared_ptr<Data::KeyFrame> mPrev, mCurr;

    public:
        Pose2dErrorTerm(std::shared_ptr<Data::KeyFrame> prevKeyFrame, std::shared_ptr<Data::KeyFrame> currKeyFrame)
        : mPrev(prevKeyFrame), mCurr(currKeyFrame){}

        template <typename T>
        bool operator()(const T* const R, const T* const t, T* residual) const
        {

            double k[] = {718.856, 0, 607.1928, 0, 718.856, 185.2157, 0, 0, 1};

            auto origin3d = mPrev->getFramePoint3d();
            auto origin2d = mCurr->getGoodMatches();

            T sumOfError;

            for(int i=0; i < origin3d.size(); ++i)
            {
                auto origin_p = origin3d[i];

                auto origin_x = origin_p.x;
                auto origin_y = origin_p.y;
                auto origin_z = origin_p.z;

                T camera_x = R[0]*origin_x + R[1]*origin_y + R[2]*origin_z + t[0];
                T camera_y = R[3]*origin_x + R[4]*origin_y + R[5]*origin_z + t[1];
                T camera_z = R[6]*origin_x + R[7]*origin_y + R[8]*origin_z + t[2];

                T projected_x = (camera_x*k[0] + camera_z*k[2]) / camera_z;
                T projected_y = (camera_y*k[4] + camera_y*k[5]) / camera_z;

                T diff_x = T(origin2d[i].x) - projected_x;
                T diff_y = T(origin2d[i].y) - projected_y;

                T error = diff_x*diff_x + diff_y*diff_y;

                sumOfError += error;
            }

            residual[0] = sumOfError;

            return true;
        }
};

template <typename T>
void optimization(T prevKeyFrame, T currKeyFrame)
{
    using ceres::AutoDiffCostFunction;
    using ceres::CostFunction;
    using ceres::Problem;
    using ceres::Solver;
    using ceres::Solve;

    Problem problem;

    cv::Mat rotationMat = prevKeyFrame->getRotationMat();
    cv::Mat translationMat = prevKeyFrame->getTranslationMat();

    double* R = new double[9];
    double* t = new double[3];


    /*
    ceres의 문제점 : double* 타입만 매개변수로 넘길 수 있다. 포인터에 값들을 넘기는 과정이 필요...
    */
    int n = 0;
    for(int y = 0; y < rotationMat.rows; ++y)
    {
        for(int x = 0; x < rotationMat.cols; ++x)
        {
            R[n] = rotationMat.ptr<double>(y)[x];
            n++;
        }
    }

    n = 0;
    for(int y = 0; y < translationMat.rows; ++y)
    {
        for(int x = 0; x < translationMat.cols; ++x)
        {
            t[n] = translationMat.ptr<double>(y)[x];
            n++;
        }
    }

    CostFunction* cost_function =
        new AutoDiffCostFunction<Pose2dErrorTerm, 1, 9, 3>(new Pose2dErrorTerm(prevKeyFrame, currKeyFrame));

    problem.AddResidualBlock(cost_function, nullptr, R, t);

    Solver::Options options;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    Solve(options, &problem, &summary);


    /*
    최적화된 값을 다시 R,t에 넣어주는 과정
    */
    n = 0;
    for(int y=0; y<rotationMat.rows; ++y)
    {
        for(int x=0; x<rotationMat.cols; ++x)
        {
            rotationMat.at<double>(y,x) = R[n];
            ++n;
        }
    }

    n = 0;
    for(int y=0; y<translationMat.rows; ++y)
    {
        for(int x=0; x<translationMat.cols; ++x)
        {
            translationMat.at<double>(y,x) = t[n];
            ++n;
        }
    }

    prevKeyFrame->setRotationMat(rotationMat);
    prevKeyFrame->setTranslationMat(translationMat);


    std::cout << summary.BriefReport() << "\n";

}
