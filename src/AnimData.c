#include "AnimData.h"

AnimData UpdateAnimData(AnimData data, float dT, int maxFrame)
{
    data.runntingTime += dT;
    if (data.runntingTime >= data.updateTime)
    {
        data.runntingTime = 0.0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

void UpdateAnimDataArray(AnimData* data, int length, float dT, int maxFrame)
{
    for(int i = 0; i < length; i++)
    {
        data[i] = UpdateAnimData(data[i], dT, maxFrame);
    }
}