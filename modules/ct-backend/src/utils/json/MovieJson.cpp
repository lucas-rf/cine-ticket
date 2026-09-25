#include <ct-backend/utils/json/MovieJson.h>
#include <ct-backend/utils/json/TheaterJson.h>

namespace ct::model
{
    void to_json(nlohmann::ordered_json& j, const Movie& movie)
    {
        j = nlohmann::ordered_json{
            {"id", movie.id},
            {"title", movie.title},
            {"synopsis", movie.synopsis},
            {"classification", movie.classification},
            {"genre", movie.genre},
            {"coverImage", movie.coverImage},
            {"backgroundImage", movie.backgroundImage},
            {"runtime", movie.runtime},
            {"theaters", movie.theaters}
        };
    }

    void from_json(const nlohmann::ordered_json& j, Movie& movie)
    {
        j.at("id").get_to(movie.id);
        j.at("title").get_to(movie.title);
        j.at("synopsis").get_to(movie.synopsis);
        j.at("classification").get_to(movie.classification);
        j.at("genre").get_to(movie.genre);
        j.at("coverImage").get_to(movie.coverImage);
        j.at("backgroundImage").get_to(movie.backgroundImage);
        j.at("runtime").get_to(movie.runtime);
        j.at("theaters").get_to(movie.theaters);
    }
}
