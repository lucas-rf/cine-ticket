#pragma once

namespace ct::model
{
    struct Seat
    {
        enum State
        {
            FREE,
            SELECTED_BY_CURRENT_USER,
            SELECTED_BY_OTHER_USER,
            ORDERED,
            INVALID,
        };

        int id;
        int movieSessionId;
        int row;
        int column;
        State state;
        int cartId;
        int orderId;
    };
}
