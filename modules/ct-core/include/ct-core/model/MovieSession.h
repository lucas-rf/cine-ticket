#pragma once

namespace ct::model
{
    struct MovieSession
    {
        int id;
        int roomId;
        int movieId;
        int day;
        int time;
        float price;
    };
}
