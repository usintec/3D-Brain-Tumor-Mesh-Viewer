#pragma once

#include <vector>
#include <cmath>
#include <numeric>
#include <iostream>

class Evaluation
{
public:

    //-----------------------------------
    // FPS Evaluation
    //-----------------------------------

    static float computeFPS(float deltaTime)
    {
        if (deltaTime <= 0.0f)
            return 0.0f;

        return 1.0f / deltaTime;
    }

    //-----------------------------------
    // Gradient Magnitude
    //-----------------------------------

    static float gradientMagnitude(
        float dx,
        float dy)
    {
        return std::sqrt(
            dx * dx +
            dy * dy
        );
    }

    //-----------------------------------
    // Fresnel Edge Visibility
    //-----------------------------------

    static float edgeVisibility(
        float dotVN)
    {
        return std::pow(
            1.0f -
            std::max(dotVN, 0.0f),
            2.0f
        );
    }

    //-----------------------------------
    // Silhouette Visibility
    //-----------------------------------

    static float silhouetteVisibility(
        const std::vector<float>& values)
    {
        if (values.empty())
            return 0.0f;

        float sum =
            std::accumulate(
                values.begin(),
                values.end(),
                0.0f);

        return sum /
            static_cast<float>(
                values.size());
    }
};