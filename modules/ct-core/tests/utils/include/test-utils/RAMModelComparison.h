#pragma once

#include <ct-core/impl/ramdb/RAMMovie.h>
#include <ct-core/impl/ramdb/RAMMovieSession.h>
#include <ct-core/impl/ramdb/RAMCart.h>
#include <ct-core/impl/ramdb/RAMOrder.h>

namespace ct::impl
{
    inline bool operator==(const RAMMovie& left, const RAMMovie& right)
    {
        return
            left.id == right.id &&
            left.title == right.title &&
            left.synopsis == right.synopsis &&
            left.classification == right.classification &&
            left.genre == right.genre &&
            left.coverImage == right.coverImage &&
            left.backgroundImage == right.backgroundImage &&
            left.runtime == right.runtime &&
            left.idxDaysToSessions == right.idxDaysToSessions;
    }
}
