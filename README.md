# Cine Ticket

This is a demonstration application for booking movie tickets online.

## Back end

The back end of the application is an executable written in C++. It is divided in two projects:
- **`ct-core`**: a library that implements the core logic behind ticket bookings and the database.
- **`ct-backend`**: implements the HTTP server with the REST API, generates the backend executable.

### The ticket booking logic

#### Entities

The application abstracts the following entities:
##### Movie
Represents a movie currently in exhibition, abstracted by the `ct::model::Movie` struct.
##### Theater
Represents a theater, abstracted by the `ct::movel::Theater` struct.
##### Room
Represents a room in a theater, abstracted by the `ct::movel::Room` struct. A *Room* can be used to exhibit multiple *Movies* in a given day. A *Theater* can contain multiple *Rooms*.
##### Day
Represents a day in a week of exhibitions. It is not abstracted by a struct, but just represented by an integer in the code.
For simplicity, the booking system was designed to allow bookings in periods of one week, from Thursday (day 0) to Wednesday (day 6).
A theater *Room* can be used to exhibit *Movies* in multiple *Days*.
##### Movie Session
Represents a *Movie* exhibition session in a given *Room*, *Day* and time. It is abstracted by the `ct::model::MovieSession` struct.
In a given *Day*, a *Room* can have multiple *Movie Sessions* of different *Movies*. Multiple *Movie Sessions* of the same *Movie* can occur in the same *Room* and *Day* at different times.
##### Seat
I does **not** represent a physical seat in a *Room*. Instead, it represents a seat that can be booked in a *Movie Session*. This means that if a *Room* has for example 10 physical seats, and is used to exhibit 4 *Movies* per *Day* for 7 *Days* (28 *Movie Sessions* in total), it will have 10 x 4 x 7 = 280 *Seats* available for booking.
A *Seat* can be in one of the following states:
- **Free**: can be selected by any [User](#User) to be booked.
- **Selected**: is currently selected by an *User*, cannot be selected by a different *User*.
- **Booked**: has been booked and cannot be selected for booking by any *User*.

It can be noted that the `model` namespace also has the struct `ct::model::RoomSession`. It does not represent a specific abstraction, it is just used to generate a complete view of a given *Movie Session* (with the *Theater* and *Seats* data) to be served by the API.
##### Cart
Represents a shopping cart containing selected *Seats* to be booked.  It is abstracted by the `ct::model::Cart` struct.
A *Cart* can contain any number of *Seats* from an unique *Movie Session*. This means that a *Cart* cannot contain *Seats* from different *Movie Sessions*. If a customer wants to book seats from multiple *Movie Sessions*, he has to book them in separate operations, meaning finishing a booking for a *Movie Session* and then moving to the next.
The *Cart* has an expiration time. This means that once it is created, a timer starts running, and when it expires, the *Cart* is deleted. The *Cart* has to be checked out before the time runs out in order to complete the booking. This is to prevent someone from selecting *Seats* and blocking them indefinitely from selection by other people.
##### Booking
Represents a successful booking of a set of *Seats* from a given *Movie Session*. It is generated from a *Cart* in the check-out process, and is abstracted by the `ct::model::Booking` struct.
##### User
Represents an active user that is selecting *Seats* for booking. It is not abstracted by a struct, but just represented by an integer in the code. A *User* can have zero or one *Cart*.
For simplicity, the application does not support user sign up or login. A user key is generated when a new session is created, and remains tied to it. This means that if the app is accessed by the same person from two different browsers in the same device, two different sessions will be created, and consequently two different *Users*.

#### Operations

##### Select Seat
A *User* can select a *Seat* that is currently free from a *Movie Session*.
Once a *Seat* is selected, it is added to the *User's* *Cart*, and cannot be selected by a different *User*.
If the *User* does not have a *Cart*, a new *Cart* is created, and then the *Seat* is added to it. The *Cart* timer is started.
If the *Cart* currently has *Seats* from a different *Movie Session*, the *Cart* is cleared (all *Seats* are removed from it and consequently become free) and then the new *Seat* is added to it. The *Cart* timer is restarted. The *Cart* is effectively **recreated**, this terminology is used throughout the source code.
##### Deselect Seat
A *User* can deselect a *Seat* that is currently in it's *Cart*.
Once a *Seat* is deselected, it becomes free to be selected by any *User*.
If the deselected *Seat* is the only one in the *Cart*, the *Cart* is deleted.
##### Book Cart
A *Cart* can be checked out by its *User*, which effectively books all its *Seats* and generates a *Booking*.
Once a *Cart* is booked, all its *Seats* are removed from the *Cart* and added to the *Booking*. The *Seats'* states go from selected to booked. The *Cart* is deleted.
A unique booking key code composed of 6 symbols in the range `[0-9A-Z]` is generated and tied to the *Booking*. It can be used to view the *Booking* details.

### The REST API

#### Query endpoints
<details>
  <summary><span style="font-size: 1.4em;"><b>GET&ensp;</b><code>/movie/{movieId}</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Shows information about a specific <i>Movie</i></span></summary>

##### Request parameters

> | Name      | Type     | Data Type | Description                                |
> | --------- | -------- | --------- | ------------------------------------------ |
> | `movieId` | required | `int`     | The unique identifier of an existing movie |
##### Responses

> | HTTP Code | Content Type       | Response                                                   |
> | --------- | ------------------ | ---------------------------------------------------------- |
> | `200`     | `application/json` | [*Movie*](#movie-json) object, with `theaters` field empty |
> | `404`     | `text/plain`       | `Movie id #{movieId} not found`                            |

##### Example request
```
curl -X GET http://localhost:18080/movie/0
```
##### Example response
``` json
{
    "id": 0,
    "title": "Die Hard with a Vengeance",
    "synopsis": "John McClane is forced out of suspension to play a game of \"Simon Says\" by a terrorist who has planted bombs all around New York City and will detonate them if McClane doesn't do what he says.",
    "classification": "R",
    "genre": ["Action", "Adventure", "Thriller"],
    "coverImage": "die-hard-cover.png",
    "backgroundImage": "die-hard-background.png",
    "runtime": 128,
    "theaters": []
}
```
</details>

<details>
  <summary><span style="font-size: 1.4em;"><b>GET&ensp;</b><code>/movie/{movieId}/{day}</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Shows information about a specific <i>Movie</i>, including all sessions in a specific <i>Day</i></span></summary>

##### Request parameters

> | Name      | Type     | Data Type | Description                                     |
> | --------- | -------- | --------- | ----------------------------------------------- |
> | `movieId` | required | `int`     | The unique identifier of an existing movie      |
> | `day`     | required | `int`     | The day of the sessions that are to be included |
##### Responses

> | HTTP Code | Content Type       | Response                                                                                                                  |
> | --------- | ------------------ | ------------------------------------------------------------------------------------------------------------------------- |
> | `200`     | `application/json` | [*Movie*](#movie-json) object, with the `theaters` field populated with all theaters, rooms and sessions in the given day |
> | `404`     | `text/plain`       | `Movie id #{movieId} not found`                                                                                           |

##### Example request
```
curl -X GET http://localhost:18080/movie/1/0
```
##### Example response
``` json
{
    "id": 1,
    "title": "Seven",
    "synopsis": "Two detectives try to track down a serial killer who chooses his victims based on the Seven Deadly Sins.",
    "classification": "R",
    "genre": ["Drama", "Crime"],
    "coverImage": "seven-cover.png",
    "backgroundImage": "seven-background.png",
    "runtime": 127,
    "theaters": [
        {
            "id": 0,
            "name": "Cinemax North",
            "rooms": [
                {
                    "id": 0,
                    "theaterId": 0,
                    "number": 1,
                    "type": "2D",
                    "rows": 4,
                    "columns": 5,
                    "sessions": [
                        {
                            "id": 2,
                            "roomId": 0,
                            "movieId": 1,
                            "day": 0,
                            "time": 1400,
                            "price": 12.5
                        }
                    ]
                }
            ]
        },
        {
            "id": 1,
            "name": "iMAX Boulevard",
            "rooms": [
                {
                    "id": 3,
                    "theaterId": 1,
                    "number": 2,
                    "type": "2D iMAX",
                    "rows": 4,
                    "columns": 5,
                    "sessions": [
                        {
                            "id": 8,
                            "roomId": 3,
                            "movieId": 1,
                            "day": 0,
                            "time": 1170,
                            "price": 16.200000762939453
                        },
                        {
                            "id": 9,
                            "roomId": 3,
                            "movieId": 1,
                            "day": 0,
                            "time": 1360,
                            "price": 16.200000762939453
                        }
                    ]
                }
            ]
        }
    ]
}
```
</details>

<details>
  <summary><span style="font-size: 1.4em;"><b>GET&ensp;</b><code>/movies</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Lists all <i>Movies</i></span></summary>

##### Request parameters
*No request parameters needed.*
##### Responses

> | HTTP Code | Content Type       | Response                                                              |
> | --------- | ------------------ | --------------------------------------------------------------------- |
> | `200`     | `application/json` | Array of [*Movie*](#movie-json) objects, with `theaters` fields empty |

##### Example request
```
curl -X GET http://localhost:18080/movies
```
##### Example response
``` json
[
    {
        "id": 0,
        "title": "Die Hard with a Vengeance",
        "synopsis": "John McClane is forced out of suspension to play a game of \"Simon Says\" by a terrorist who has planted bombs all around New York City and will detonate them if McClane doesn't do what he says.",
        "classification": "R",
        "genre": ["Action", "Adventure", "Thriller"],
        "coverImage": "die-hard-cover.png",
        "backgroundImage": "die-hard-background.png",
        "runtime": 128,
        "theaters": []
    },
    {
        "id": 1,
        "title": "Seven",
        "synopsis": "Two detectives try to track down a serial killer who chooses his victims based on the Seven Deadly Sins.",
        "classification": "R",
        "genre": ["Drama", "Crime"],
        "coverImage": "seven-cover.png",
        "backgroundImage": "seven-background.png",
        "runtime": 127,
        "theaters": []
    },
    {
        "id": 2,
        "title": "Braveheart",
        "synopsis": "Scottish warrior William Wallace leads his countrymen in a rebellion to free his homeland from the tyranny of King Edward I of England.",
        "classification": "R",
        "genre": ["Drama", "War", "Epic"],
        "coverImage": "braveheart-cover.png",
        "backgroundImage": "braveheart-background.png",
        "runtime": 178,
        "theaters": []
    },
    {
        "id": 3,
        "title": "Jumanji",
        "synopsis": "A magic board game summons jungle perils and a missing boy.",
        "classification": "PG",
        "genre": ["Adventure", "Fantasy"],
        "coverImage": "jumanji-cover.png",
        "backgroundImage": "jumanji-background.png",
        "runtime": 104,
        "theaters": []
    }
]
```
</details>

<details>
  <summary><span style="font-size: 1.4em;"><b>GET&ensp;</b><code>/movies/{theaterId}/{day}</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Lists all <i>Movies</i> that have sessions in a specific <i>Theater</i> in a specific <i>Day</i></span></summary>

##### Request parameters

> | Name        | Type     | Data Type | Description                                     |
> | ----------- | -------- | --------- | ----------------------------------------------- |
> | `theaterId` | required | `int`     | The unique identifier of an existing theater    |
> | `day`       | required | `int`     | The day of the sessions that are to be included |
##### Responses

> | HTTP Code | Content Type       | Response                                                                                                                                                                   |
> | --------- | ------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
> | `200`     | `application/json` | Array of [*Movie*](#movie-json) objects, with the `theaters` fields populated with the theater specified by `theaterId`, with all it's rooms and sessions in the given day |
> | `404`     | `text/plain`       | `Theater id #{theaterId} not found`                                                                                                                                        |

##### Example request
```
curl -X GET http://localhost:18080/movies/1/0
```
##### Example response
``` json
[
    {
        "id": 0,
        "title": "Die Hard with a Vengeance",
        "synopsis": "John McClane is forced out of suspension to play a game of \"Simon Says\" by a terrorist who has planted bombs all around New York City and will detonate them if McClane doesn't do what he says.",
        "classification": "R",
        "genre": ["Action", "Adventure", "Thriller"],
        "coverImage": "die-hard-cover.png",
        "backgroundImage": "die-hard-background.png",
        "runtime": 128,
        "theaters": [
            {
                "id": 1,
                "name": "iMAX Boulevard",
                "rooms": [
                    {
                        "id": 2,
                        "theaterId": 1,
                        "number": 1,
                        "type": "2D iMAX",
                        "rows": 4,
                        "columns": 5,
                        "sessions": [
                            {
                                "id": 6,
                                "roomId": 2,
                                "movieId": 0,
                                "day": 0,
                                "time": 990,
                                "price": 16.200000762939453
                            }
                        ]
                    }
                ]
            }
        ]
    },
    {
        "id": 1,
        "title": "Seven",
        "synopsis": "Two detectives try to track down a serial killer who chooses his victims based on the Seven Deadly Sins.",
        "classification": "R",
        "genre": ["Drama", "Crime"],
        "coverImage": "seven-cover.png",
        "backgroundImage": "seven-background.png",
        "runtime": 127,
        "theaters": [
            {
                "id": 1,
                "name": "iMAX Boulevard",
                "rooms": [
                    {
                        "id": 3,
                        "theaterId": 1,
                        "number": 2,
                        "type": "2D iMAX",
                        "rows": 4,
                        "columns": 5,
                        "sessions": [
                            {
                                "id": 8,
                                "roomId": 3,
                                "movieId": 1,
                                "day": 0,
                                "time": 1170,
                                "price": 16.200000762939453
                            },
                            {
                                "id": 9,
                                "roomId": 3,
                                "movieId": 1,
                                "day": 0,
                                "time": 1360,
                                "price": 16.200000762939453
                            }
                        ]
                    }
                ]
            }
        ]
    },
    {
        "id": 2,
        "title": "Braveheart",
        "synopsis": "Scottish warrior William Wallace leads his countrymen in a rebellion to free his homeland from the tyranny of King Edward I of England.",
        "classification": "R",
        "genre": ["Drama", "War", "Epic"],
        "coverImage": "braveheart-cover.png",
        "backgroundImage": "braveheart-background.png",
        "runtime": 178,
        "theaters": [
            {
                "id": 1,
                "name": "iMAX Boulevard",
                "rooms": [
                    {
                        "id": 2,
                        "theaterId": 1,
                        "number": 1,
                        "type": "2D iMAX",
                        "rows": 4,
                        "columns": 5,
                        "sessions": [
                            {
                                "id": 7,
                                "roomId": 2,
                                "movieId": 2,
                                "day": 0,
                                "time": 1200,
                                "price": 16.200000762939453
                            }
                        ]
                    }
                ]
            }
        ]
    },
    {
        "id": 3,
        "title": "Jumanji",
        "synopsis": "A magic board game summons jungle perils and a missing boy.",
        "classification": "PG",
        "genre": ["Adventure", "Fantasy"],
        "coverImage": "jumanji-cover.png",
        "backgroundImage": "jumanji-background.png",
        "runtime": 104,
        "theaters": [
            {
                "id": 1,
                "name": "iMAX Boulevard",
                "rooms": [
                    {
                        "id": 4,
                        "theaterId": 1,
                        "number": 3,
                        "type": "3D iMAX",
                        "rows": 4,
                        "columns": 5,
                        "sessions": [
                            {
                                "id": 10,
                                "roomId": 4,
                                "movieId": 3,
                                "day": 0,
                                "time": 830,
                                "price": 19.899999618530273
                            },
                            {
                                "id": 11,
                                "roomId": 4,
                                "movieId": 3,
                                "day": 0,
                                "time": 990,
                                "price": 19.899999618530273
                            },
                            {
                                "id": 12,
                                "roomId": 4,
                                "movieId": 3,
                                "day": 0,
                                "time": 1150,
                                "price": 19.899999618530273
                            }
                        ]
                    }
                ]
            }
        ]
    }
]
```
</details>

<details>
  <summary><span style="font-size: 1.4em;"><b>GET&ensp;</b><code>/theaters</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Lists all <i>Theaters</i></span></summary>

##### Request parameters
*No request parameters needed.*
##### Responses

> | HTTP Code | Content Type       | Response                                                              |
> | --------- | ------------------ | --------------------------------------------------------------------- |
> | `200`     | `application/json` | Array of [*Theater*](#theater-json) objects, with `rooms` fields empty |

##### Example request
```
curl -X GET http://localhost:18080/theaters
```
##### Example response
``` json
[
    {
        "id": 0,
        "name": "Cinemax North",
        "rooms": []
    },
    {
        "id": 1,
        "name": "iMAX Boulevard",
        "rooms": []
    }
]
```
</details>

<details>
  <summary><span style="font-size: 1.4em;"><b>GET&ensp;</b><code>/roomSession/{movieSessionId}</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Shows information about a specific <i>Movie Session</i> with details</span></summary>

##### Request parameters

> | Name             | Type     | Data Type | Description                                        |
> | ---------------- | -------- | --------- | -------------------------------------------------- |
> | `movieSessionId` | required | `int`     | The unique identifier of an existing movie session |
##### Responses

> | HTTP Code | Content Type       | Response                                                                                                 |
> | --------- | ------------------ | -------------------------------------------------------------------------------------------------------- |
> | `200`     | `application/json` | [*RoomSession*](#room-session-json) object, with `seats` field containing all seats in the specific room |
> | `404`     | `text/plain`       | `Movie session id #{movieSessionId} not found`                                                           |

##### Example request
```
curl -X GET http://localhost:18080/roomSession/12
```
##### Example response
``` json
{
    "id": 12,
    "day": 0,
    "time": 1150,
    "price": 19.899999618530273,
    "theater": {
        "id": 1,
        "name": "iMAX Boulevard",
        "rooms": []
    },
    "room": {
        "id": 4,
        "theaterId": 1,
        "number": 3,
        "type": "3D iMAX",
        "rows": 4,
        "columns": 5,
        "sessions": []
    },
    "movie": {
        "id": 3,
        "title": "Jumanji",
        "synopsis": "A magic board game summons jungle perils and a missing boy.",
        "classification": "PG",
        "genre": ["Adventure", "Fantasy"],
        "coverImage": "jumanji-cover.png",
        "backgroundImage": "jumanji-background.png",
        "runtime": 104,
        "theaters": []
    },
    "seats": [
        {
            "id": 240,
            "movieSessionId": 12,
            "row": 0,
            "column": 0,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 241,
            "movieSessionId": 12,
            "row": 0,
            "column": 1,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 242,
            "movieSessionId": 12,
            "row": 0,
            "column": 2,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 243,
            "movieSessionId": 12,
            "row": 0,
            "column": 3,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 244,
            "movieSessionId": 12,
            "row": 0,
            "column": 4,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 245,
            "movieSessionId": 12,
            "row": 1,
            "column": 0,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 246,
            "movieSessionId": 12,
            "row": 1,
            "column": 1,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 247,
            "movieSessionId": 12,
            "row": 1,
            "column": 2,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 248,
            "movieSessionId": 12,
            "row": 1,
            "column": 3,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 249,
            "movieSessionId": 12,
            "row": 1,
            "column": 4,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 250,
            "movieSessionId": 12,
            "row": 2,
            "column": 0,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 251,
            "movieSessionId": 12,
            "row": 2,
            "column": 1,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 252,
            "movieSessionId": 12,
            "row": 2,
            "column": 2,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 253,
            "movieSessionId": 12,
            "row": 2,
            "column": 3,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 254,
            "movieSessionId": 12,
            "row": 2,
            "column": 4,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 255,
            "movieSessionId": 12,
            "row": 3,
            "column": 0,
            "state": "BOOKED",
            "cartId": -1,
            "bookingId": 12
        },
        {
            "id": 256,
            "movieSessionId": 12,
            "row": 3,
            "column": 1,
            "state": "BOOKED",
            "cartId": -1,
            "bookingId": 12
        },
        {
            "id": 257,
            "movieSessionId": 12,
            "row": 3,
            "column": 2,
            "state": "BOOKED",
            "cartId": -1,
            "bookingId": 12
        },
        {
            "id": 258,
            "movieSessionId": 12,
            "row": 3,
            "column": 3,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        },
        {
            "id": 259,
            "movieSessionId": 12,
            "row": 3,
            "column": 4,
            "state": "FREE",
            "cartId": -1,
            "bookingId": -1
        }
    ]
}
```
</details>

<details>
  <summary><span style="font-size: 1.4em;"><b>GET&ensp;</b><code>/cart</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Shows information about the <i>Cart</i></span></summary>

##### Request parameters
*No request parameters needed.*
##### Responses

> | HTTP Code | Content Type       | Response                                                                                      |
> | --------- | ------------------ | --------------------------------------------------------------------------------------------- |
> | `200`     | `application/json` | [*Cart*](#movie-json) object corresponding to the cart of the user that is making the request |
> | `404`     | `text/plain`       | `No cart found for the current session`                                                       |

##### Example request
```
curl -X GET http://localhost:18080/cart
```
##### Example response
``` json
{
    "id": 0,
    "movieSessionId": 10,
    "expirationTime": "2026-09-25 03:37:35",
    "seatCount": 3,
    "seats": [
        {
            "id": 206,
            "movieSessionId": 10,
            "row": 1,
            "column": 1,
            "state": "SELECTED_BY_CURRENT_USER",
            "cartId": 0,
            "bookingId": -1
        },
        {
            "id": 207,
            "movieSessionId": 10,
            "row": 1,
            "column": 2,
            "state": "SELECTED_BY_CURRENT_USER",
            "cartId": 0,
            "bookingId": -1
        },
        {
            "id": 208,
            "movieSessionId": 10,
            "row": 1,
            "column": 3,
            "state": "SELECTED_BY_CURRENT_USER",
            "cartId": 0,
            "bookingId": -1
        }
    ]
}
```
</details>

<details>
  <summary><span style="font-size: 1.4em;"><b>GET&ensp;</b><code>/seat/{seatId}</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Shows information about a specific <i>Seat</i></span></summary>

##### Request parameters

> | Name     | Type     | Data Type | Description                               |
> | -------- | -------- | --------- | ----------------------------------------- |
> | `seatId` | required | `int`     | The unique identifier of an existing seat |
##### Responses

> | HTTP Code | Content Type       | Response                      |
> | --------- | ------------------ | ----------------------------- |
> | `200`     | `application/json` | [*Seat*](#seat-json) object   |
> | `404`     | `text/plain`       | `Seat id #{seatId} not found` |

##### Example request
```
curl -X GET http://localhost:18080/seat/181
```
##### Example response
``` json
{
    "id": 181,
    "movieSessionId": 9,
    "row": 0,
    "column": 1,
    "state": "FREE",
    "cartId": -1,
    "bookingId": -1
}
```
</details>

<details>
  <summary><span style="font-size: 1.4em;"><b>GET&ensp;</b><code>/booking/{bookingKey}</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Shows information about a specific <i>Booking</i></span></summary>

##### Request parameters

> | Name         | Type     | Data Type | Description                                  |
> | ------------ | -------- | --------- | -------------------------------------------- |
> | `bookingKey` | required | `string`  | The unique identifier of an existing booking |
##### Responses

> | HTTP Code | Content Type       | Response                                   |
> | --------- | ------------------ | ------------------------------------------ |
> | `200`     | `application/json` | [*Booking*](#booking-json) object          |
> | `404`     | `text/plain`       | `Booking with key #{bookingKey} not found` |

##### Example request
```
curl -X GET http://localhost:18080/booking/W8E6L3
```
##### Example response
``` json
{
    "id": 14,
    "movieSessionId": 7,
    "userEmail": "robert.brown@email.com",
    "bookingKey": "W8E6L3",
    "bookingTime": "2026-09-25 03:44:47",
    "seatCount": 3,
    "seats": [
        {
            "id": 152,
            "movieSessionId": 7,
            "row": 2,
            "column": 2,
            "state": "BOOKED",
            "cartId": -1,
            "bookingId": 14
        },
        {
            "id": 153,
            "movieSessionId": 7,
            "row": 2,
            "column": 3,
            "state": "BOOKED",
            "cartId": -1,
            "bookingId": 14
        },
        {
            "id": 154,
            "movieSessionId": 7,
            "row": 2,
            "column": 4,
            "state": "BOOKED",
            "cartId": -1,
            "bookingId": 14
        }
    ]
}
```
</details>

#### Command endpoints

<details>
  <summary><span style="font-size: 1.4em;"><b>POST&ensp;</b><code>/seat/{seatId}/select</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Adds a specific <i>Seat</i> to the <i>Cart</i></span></summary>

##### Request parameters

> | Name     | Type     | Data Type | Description                               |
> | -------- | -------- | --------- | ----------------------------------------- |
> | `seatId` | required | `string`  | The unique identifier of an existing seat |
##### Responses

> | HTTP Code | Content Type       | Response                                                                          |
> | --------- | ------------------ | --------------------------------------------------------------------------------- |
> | `200`     | `application/json` | Boolean, `true` if the seat was successfully added to the cart, `false` otherwise |
> | `404`     | `text/plain`       | `Seat id #{seatId} not found`                                                     |

##### Example request
```
curl -X POST http://localhost:18080/seat/139/select
```
##### Example response
``` json
true
```
</details>

<details>
  <summary><span style="font-size: 1.4em;"><b>POST&ensp;</b><code>/seat/{seatId}/deselect</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Removes a specific <i>Seat</i> from the <i>Cart</i></span></summary>

##### Request parameters

> | Name     | Type     | Data Type | Description                               |
> | -------- | -------- | --------- | ----------------------------------------- |
> | `seatId` | required | `string`  | The unique identifier of an existing seat |
##### Responses

> | HTTP Code | Content Type       | Response                                                                              |
> | --------- | ------------------ | ------------------------------------------------------------------------------------- |
> | `200`     | `application/json` | Boolean, `true` if the seat was successfully removed from the cart, `false` otherwise |
> | `404`     | `text/plain`       | `Seat id #{seatId} not found`                                                         |

##### Example request
```
curl -X POST http://localhost:18080/seat/139/deselect
```
##### Example response
``` json
true
```
</details>

<details>
  <summary><span style="font-size: 1.4em;"><b>POST&ensp;</b><code>/book</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Books all the <i>Seats</i> in the current <i>Cart</i>, creates a <i>Booking</i> with the respective <i>Seats</i></span></summary>

##### Request parameters
*No request parameters needed.*
##### Responses

> | HTTP Code | Content Type       | Response                                                          |
> | --------- | ------------------ | ----------------------------------------------------------------- |
> | `200`     | `application/json` | [*Booking*](#booking-json) object, if the booking was successfull |
> | `200`     | `application/json` | `false`, if the booking was unsuccessfull                         |

##### Example request
```
curl -X POST http://localhost:18080/book
```
##### Example response
``` json
{
    "id": 14,
    "movieSessionId": 7,
    "userEmail": "robert.brown@email.com",
    "bookingKey": "W8E6L3",
    "bookingTime": "2026-09-25 03:44:47",
    "seatCount": 3,
    "seats": [
        {
            "id": 152,
            "movieSessionId": 7,
            "row": 2,
            "column": 2,
            "state": "BOOKED",
            "cartId": -1,
            "bookingId": 14
        },
        {
            "id": 153,
            "movieSessionId": 7,
            "row": 2,
            "column": 3,
            "state": "BOOKED",
            "cartId": -1,
            "bookingId": 14
        },
        {
            "id": 154,
            "movieSessionId": 7,
            "row": 2,
            "column": 4,
            "state": "BOOKED",
            "cartId": -1,
            "bookingId": 14
        }
    ]
}
```
</details>

#### Websockets

<details>
  <summary><span style="font-size: 1.4em;"><b>WEBSOCKET&ensp;</b><code>/movieSession/{movieSessionId}/events</code>&ensp;::&ensp;</span><span style="font-size: 1.2em;">Listens to events from a specific <i>Movie Session</i></span></summary>


A *User* can connect to this websocket to subscribe to events that happen to *Seats* in a given *Movie Session*.

##### Request parameters

> | Name             | Type     | Data Type | Description                                        |
> | ---------------- | -------- | --------- | -------------------------------------------------- |
> | `movieSessionId` | required | `string`  | The unique identifier of an existing movie session |
##### Messages

> | Content Type       | Content                                                       |
> | ------------------ | ------------------------------------------------------------- |
> | `application/json` | Array of [*Seat*](#seat-json) objects that have been modified |

##### Example message
``` json
[
    {
        "id": 17,
        "movieSessionId": 0,
        "row": 3,
        "column": 2,
        "state": "SELECTED_BY_OTHER_USER",
        "cartId": 4,
        "bookingId": -1
    }
]
```
</details>

#### Objects
##### Movie <a name="movie-json"></a>

> | Name              | Data Type   | Description                                                          |
> | ----------------- | ----------- | -------------------------------------------------------------------- |
> | `id`              | `int`       | Unique identifier                                                    |
> | `title`           | `string`    | Title                                                                |
> | `synopsis`        | `string`    | Synopsis                                                             |
> | `classification`  | `string`    | Classification                                                       |
> | `genre`           | `string[]`  | Genre (multiple)                                                     |
> | `coverImage`      | `string`    | Cover image file name                                                |
> | `backgroundImage` | `string`    | Background image file name                                           |
> | `runtime`         | `int`       | Runtime in minutes                                                   |
> | `theaters`        | `Theater[]` | Array of [*Theaters*](#theater-json) that are exhibiting the *Movie* |

##### Theater <a name="theater-json"></a>

> | Name    | Data Type | Description                                                 |
> | ------- | --------- | ----------------------------------------------------------- |
> | `id`    | `int`     | Unique identifier                                           |
> | `name`  | `string`  | Title                                                       |
> | `rooms` | `Room[]`  | Array of [*Rooms*](#room-json) that belong to the *Theater* |

##### Room <a name="room-json"></a>

> | Name        | Data Type        | Description                                                                          |
> | ----------- | ---------------- | ------------------------------------------------------------------------------------ |
> | `id`        | `int`            | Unique identifier                                                                    |
> | `theaterId` | `int`            | Unique identifier of the *Theater* to which the *Room* belongs                       |
> | `number`    | `int`            | *Room* number                                                                        |
> | `type`      | `string`         | Type of the *Room* (2D, 3D, 2D XD, ...)                                              |
> | `rows`      | `int`            | Number of rows of *Seats*                                                            |
> | `columns`   | `int`            | Number of *Seats* per row                                                            |
> | `sessions`  | `MovieSession[]` | Array of [*MovieSessions*](#movie-session-json) that are to take place in the *Room* |

##### MovieSession <a name="movie-session-json"></a>

> | Name       | Data Type        | Description                                                                                                                                                          |
> | ---------- | ---------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
> | `id`       | `int`            | Unique identifier                                                                                                                                                    |
> | `roomId`   | `int`            | Unique identifier of the *Room* in which the *Movie Session* takes place                                                                                             |
> | `movieId`  | `int`            | Unique identifier of the *Movie*                                                                                                                                     |
> | `day`      | `int`            | *Day* in which the *Movie Session* will take place                                                                                                                   |
> | `time`     | `int`            | Time of day in continuous minutes when the *Movie Session* will take place. To be converted to the format HH:MM, it has to be processed like "{time//60}:{time%60}". |
> | `price`    | `float`          | Ticket price                                                                                                                                                         |

##### Seat <a name="seat-json"></a>

> | Name             | Data Type | Description                                                                                                                                                                    |
> | ---------------- | --------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
> | `id`             | `int`     | Unique identifier                                                                                                                                                              |
> | `movieSessionId` | `int`     | Unique identifier of the *Movie Session* to which the *Seat* belongs                                                                                                           |
> | `row`            | `int`     | Row where the *Seat* is physically located inside the *Room*                                                                                                                   |
> | `column`         | `int`     | Column where the *Seat* is physically located inside the *Room*                                                                                                                |
> | `state`          | `string`  | The current state of the *Seat* from the perspective of the *User* that is making the request. Can be one of the values listed in the [*Seat::State*](#seat-state-json) table. |
> | `cartId`         | `int`     | Unique identifier of the *Cart* to which the *Seat* is associated. `-1` if it is not associated with any *Cart*.                                                               |
> | `bookingId`      | `int`     | Unique identifier of the *Booking* to which the *Seat* is associated. `-1` if it is not associated with any *Booking*.                                                         |
Note: a *Seat* cannot be associated to both a *Cart* and a *Booking* at the same time. This means that if `Seat.cartId` is different than `-1`, `Seat.bookingId` cannot be different than `-1`, and vice-versa.

##### Seat::State <a name="seat-state-json"></a>

> | Value                      | Description                                                                                                                                                                                                                                         |
> | -------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
> | `FREE`                     | *Seat* is free and available to be selected for the *Cart*. This state happens when `Seat.cartId` and `Seat.bookingId` are both `-1`.                                                                                                               |
> | `SELECTED_BY_CURRENT_USER` | *Seat* is currently in the *Cart* of the *User* that is making the request. This state happens when `Seat.cartId` corresponds to the *Cart* id of the current *User*; and `Seat.bookingId` is `-1`.                                                 |
> | `SELECTED_BY_OTHER_USER`   | *Seat* is currently in the *Cart* of a *User* different than the one making the request. This state happens when `Seat.cartId` is different than `-1` and does not correspond to the *Cart* id of the current *User*; and `Seat.bookingId` is `-1`. |
> | `BOOKED`                   | *Seat* is currently booked. This state happens when `Seat.cartId` is `-1` and `Seat.bookingId` is different than `-1`.                                                                                                                              |

##### RoomSession <a name="room-session-json"></a>

Represents a *Movie Session* but in greater detail than by [*MovieSession*](#movie-session-json).

> | Name      | Data Type | Description                                                                                                                                                          |
> | --------- | --------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
> | `id`      | `int`     | Unique identifier                                                                                                                                                    |
> | `day`     | `int`     | *Day* in which the *Movie Session* will take place                                                                                                                   |
> | `time`    | `int`     | Time of day in continuous minutes when the *Movie Session* will take place. To be converted to the format HH:MM, it has to be processed like "{time//60}:{time%60}". |
> | `price`   | `float`   | Ticket price                                                                                                                                                         |
> | `theater` | `Theater` | [*Theater*](#theater-json) in which the *Movie Session* will take place                                                                                              |
> | `room`    | `Room`    | [*Room*](#room-json) in which the *Movie Session* will take place                                                                                                    |
> | `movie`   | `Movie`   | [*Movie*](#movie-json) that will be played                                                                                                                           |
> | `seats`   | `Seat[]`  | Array of [*Seats*](#seat-json) that belong to the *Movie Session*                                                                                                    |

##### Cart <a name="cart-json"></a>

> | Name             | Data Type | Description                                                                        |
> | ---------------- | --------- | ---------------------------------------------------------------------------------- |
> | `id`             | `int`     | Unique identifier                                                                  |
> | `movieSessionId` | `int`     | Unique identifier of the *Movie Session* to which the *Seats* in the *Cart* belong |
> | `expirationTime` | `string`  | Time when the *Cart* is going to expire                                            |
> | `seatCount`      | `int`     | Number of *Seats* in the *Cart*                                                    |
> | `seats`          | `Seat[]`  | Array of [*Seats*](#seat-json) that are in the *Cart*                              |

##### Booking <a name="booking-json"></a>

> | Name             | Data Type | Description                                                                                      |
> | ---------------- | --------- | ------------------------------------------------------------------------------------------------ |
> | `id`             | `int`     | Unique identifier                                                                                |
> | `movieSessionId` | `int`     | Unique identifier of the *Movie Session* to which the *Seats* in the *Booking* belong            |
> | `userEmail`      | `string`  | Email address of the *User* that booked the tickets                                              |
> | `bookingKey`     | `string`  | Unique 6-digit key that identifies the *Booking*, composed of characters in the range `[0-9A-Z]` |
> | `bookingTime`    | `string`  | Time when the booking took place                                                                 |
> | `seatCount`      | `int`     | Number of *Seats* in the *Booking*                                                               |
> | `seats`          | `Seat[]`  | Array of [*Seats*](#seat-json) that are in the *Booking*                                         |

### Build and installation

To build **`ct-backend`**, take the following steps:

1. Make sure that CMake and a C++ compiler that supports C++20 are present in the system.

2. Clone the repository
```
git clone https://github.com/lucas-rf/cine-ticket.git
```
2. Check out the branch or tag that you want to build from
```
cd ./cine-ticket
git checkout v1.0
git submodule update --init --recursive
```
4. Clone the submodules
```
git submodule update --init --recursive
```
5. Build with CMake
```
cd ./modules/ct-backend
cmake -S . -B build
cmake --build build --config Release
```
6. Run the tests if you want
```
ctest --test-dir build
```
7. Install the executable to the destination you want. The executable will be located at `<destination-dir>/bin/ct-backend`
```
cmake --install ./build --prefix "/<install>/<directory>/"
```

### Running **ct-backend**

Assuming the executable is located at `/app/bin`, to get help about the command line parameters you can run:
```
cd /app
./bin/ct-backend --help
```

To which the response will be:
```
Usage: cine-ticket-backend [--help] [--version] [--port VAR] [--multithreaded] [--database-file VAR]

Optional arguments:
  -h, --help       shows help message and exits
  -v, --version    prints version information and exits
  --port           The port in which incoming connections are to be accepted. If not specified, uses the value defined via the CINE_TICKET_PORT environment variable.
  --multithreaded  Enable multithreaded mode
  --database-file  Load database data from the specified file. If not specified, tries to load from the path defined by the CINE_TICKET_DB_FILE environment variable.
```

To run a server listening at port 18080:
```
./bin/ct-backend --port 18080 --multithreaded --database-file </path/to/db/file.json>
```

This repository contains a sample file structure at `<repo-root>/example`. To run a demonstration server, you can take the following steps:
1. Copy the files to where the app will be located
```
cp -R <ct-backend-root-dir>/example/* /app/
```
2. Navigate to the directory and run the server
``` console
cd /app
./bin/ct-backend --port 18080 --multithreaded --database-file ./db/cine-ticket-db.json
```

## Command line client

The command line tool `ct-cli.py` located at `modules/ct-backend/scripts`, is a simple client that can connect to the backend and showcase its functionality.

To run it, assuming that the server is running locally and listening at the port 18080:
```
python <repo-root>/modules/ct-backend/scripts/ct-cli.py http://localhost:18080
```

<details>
<summary>Interaction example:</summary>

<pre><code>
************************
*                      *
*   CINE TICKET HOME   *
*                      *
************************

Options:
     movies: View movies
   theaters: View theaters
  booking-C: View booking (example booking-RT6U81)
       quit: Quit

Type option: movies

**************
*            *
*   MOVIES   *
*            *
**************

Movie #0
Title.........: Die Hard with a Vengeance
Synopsis......: John McClane is forced out of suspension to play a game of &quot;Simon Says&quot; by a terrorist who has planted bombs all around New York City and will detonate them if McClane doesn&#39;t do what he says.
Classification: R
Genre.........: Action Adventure Thriller
Runtime.......: 128 minutes

Movie #1
Title.........: Seven
Synopsis......: Two detectives try to track down a serial killer who chooses his victims based on the Seven Deadly Sins.
Classification: R
Genre.........: Drama Crime
Runtime.......: 127 minutes

Movie #2
Title.........: Braveheart
Synopsis......: Scottish warrior William Wallace leads his countrymen in a rebellion to free his homeland from the tyranny of King Edward I of England.
Classification: R
Genre.........: Drama War Epic
Runtime.......: 178 minutes

Movie #3
Title.........: Jumanji
Synopsis......: A magic board game summons jungle perils and a missing boy.
Classification: PG
Genre.........: Adventure Fantasy
Runtime.......: 104 minutes

Options:
    movie-N: View movie N (example: movie-2)
       back: Go back
       home: Go to Home
       quit: Quit

Type option: movie-3

*********************************************************
*                                                       *
*   Select day to view sessions for the movie Jumanji   *
*                                                       *
*********************************************************

#0: Thursday
#1: Friday
#2: Saturday
#3: Sunday
#4: Monday
#5: Tuesday
#6: Wednesday

Options:
      day-N: Select day N (example: day-4)
       back: Go back
       home: Go to Home
       quit: Quit

Type option: day-0

**********************
*                    *
*   MOVIE: Jumanji   *
*                    *
**********************

Synopsis......: A magic board game summons jungle perils and a missing boy.
Classification: PG
Genre.........: Adventure Fantasy
Runtime.......: 104 minutes

Showtimes on Thursday:

Theater: Cinemax North
Room 2 2D
Session #3 at 14:40 .. ticket price: $12.50

Theater: iMAX Boulevard
Room 3 3D iMAX
Session #10 at 13:50 .. ticket price: $19.90
Session #11 at 16:30 .. ticket price: $19.90
Session #12 at 19:10 .. ticket price: $19.90

Options:
  session-N: View session N (example: session-2)
       back: Go back
       home: Go to Home
       quit: Quit

Type option: session-11

*************************
*                       *
*   Jumanji             *
*                       *
*   at iMAX Boulevard   *
*                       *
*   Thursday 16:30      *
*                       *
*************************

               SCREEN
===================================

 [A00]  [A01]  [A02]  [A03]  [A04] 

 [B00]  [B01]  [B02]  [B03]  [B04]

 [C00]  [C01]  [C02]  :C03:  :C04:

 [D00]  [D01]  [D02]  [D03]  [D04]

Legend:
  [RCC] Available
  &gt;RCC&lt; Selected by you
  |RCC| Selected by another person
  :RCC: Booked (unavailable)

Options:
   select-S: Select seat (example: select-B12)
 deselect-S: Deselect seat (example: deselect-B12)
       book: Book selected tickets
       back: Go back
       home: Go to Home
       quit: Quit

Type option: select-B01

*************************
*                       *
*   Jumanji             *
*                       *
*   at iMAX Boulevard   *
*                       *
*   Thursday 16:30      *
*                       *
*************************

               SCREEN
===================================

 [A00]  [A01]  [A02]  [A03]  [A04]

 [B00]  &gt;B01&lt;  [B02]  [B03]  [B04]

 [C00]  [C01]  [C02]  :C03:  :C04:

 [D00]  [D01]  [D02]  [D03]  [D04] 

Legend:
  [RCC] Available
  &gt;RCC&lt; Selected by you
  |RCC| Selected by another person
  :RCC: Booked (unavailable)

Options:
   select-S: Select seat (example: select-B12)
 deselect-S: Deselect seat (example: deselect-B12)
       book: Book selected tickets
       back: Go back
       home: Go to Home
       quit: Quit

Type option: select-C02

*************************
*                       *
*   Jumanji             *
*                       *
*   at iMAX Boulevard   *
*                       *
*   Thursday 16:30      *
*                       *
*************************

               SCREEN
===================================

 [A00]  [A01]  [A02]  [A03]  [A04]

 [B00]  &gt;B01&lt;  [B02]  [B03]  [B04]

 [C00]  [C01]  &gt;C02&lt;  :C03:  :C04:

 [D00]  [D01]  [D02]  [D03]  [D04]

Legend:
  [RCC] Available
  &gt;RCC&lt; Selected by you
  |RCC| Selected by another person
  :RCC: Booked (unavailable)

Options:
   select-S: Select seat (example: select-B12)
 deselect-S: Deselect seat (example: deselect-B12)
       book: Book selected tickets
       back: Go back
       home: Go to Home
       quit: Quit

Type option: deselect-C02

*************************
*                       *
*   Jumanji             *
*                       *
*   at iMAX Boulevard   *
*                       *
*   Thursday 16:30      *
*                       *
*************************

               SCREEN
===================================

 [A00]  [A01]  [A02]  [A03]  [A04]

 [B00]  &gt;B01&lt;  [B02]  [B03]  [B04]

 [C00]  [C01]  [C02]  :C03:  :C04:

 [D00]  [D01]  [D02]  [D03]  [D04]

Legend:
  [RCC] Available
  &gt;RCC&lt; Selected by you
  |RCC| Selected by another person
  :RCC: Booked (unavailable)

Options:
   select-S: Select seat (example: select-B12)
 deselect-S: Deselect seat (example: deselect-B12)
       book: Book selected tickets
       back: Go back
       home: Go to Home
       quit: Quit

Type option: select-B02

*************************
*                       *
*   Jumanji             *
*                       *
*   at iMAX Boulevard   *
*                       *
*   Thursday 16:30      *
*                       *
*************************

               SCREEN
===================================

 [A00]  [A01]  [A02]  [A03]  [A04] 

 [B00]  &gt;B01&lt;  &gt;B02&lt;  [B03]  [B04]

 [C00]  [C01]  [C02]  :C03:  :C04:

 [D00]  [D01]  [D02]  [D03]  [D04]

Legend:
  [RCC] Available
  &gt;RCC&lt; Selected by you
  |RCC| Selected by another person
  :RCC: Booked (unavailable)

Options:
   select-S: Select seat (example: select-B12)
 deselect-S: Deselect seat (example: deselect-B12)
       book: Book selected tickets
       back: Go back
       home: Go to Home
       quit: Quit

Type option: book

************************
*                      *
*   BOOKING [83YQF3]   *
*                      *
************************

Jumanji
iMAX Boulevard
Room 3 3D iMAX
Thursday 16:30

Seats:  B01  B02

Options:
       back: Go back
       home: Go to Home
       quit: Quit

Type option: home

************************
*                      *
*   CINE TICKET HOME   *
*                      *
************************

Options:
     movies: View movies
   theaters: View theaters
  booking-C: View booking (example booking-RT6U81)
       quit: Quit

Type option: theaters

****************
*              *
*   THEATERS   *
*              *
****************

Theater #0: Cinemax North
Theater #1: iMAX Boulevard

Options:
  theater-N: View theater N (example: theater-1)
       back: Go back
       home: Go to Home
       quit: Quit

Type option: theater-0

*****************************************************************
*                                                               *
*   Select day to view sessions for the theater Cinemax North   *
*                                                               *
*****************************************************************

#0: Thursday
#1: Friday
#2: Saturday
#3: Sunday
#4: Monday
#5: Tuesday
#6: Wednesday

Options:
      day-N: Select day N (example: day-4)
       back: Go back
       home: Go to Home
       quit: Quit

Type option: day-0

******************************
*                            *
*   THEATER: Cinemax North   *
*                            *
******************************

Showtimes on Thursday:

Die Hard with a Vengeance
=========================

Room 1 2D
Session #0 at 17:10 .. ticket price: $12.50
Session #1 at 20:30 .. ticket price: $12.50

Seven
=====

Room 1 2D
Session #2 at 23:20 .. ticket price: $12.50

Braveheart
==========

Room 2 2D
Session #4 at 18:30 .. ticket price: $12.50
Session #5 at 22:20 .. ticket price: $12.50

Jumanji
=======

Room 2 2D
Session #3 at 14:40 .. ticket price: $12.50

Options:
  session-N: View session N (example: session-2)
       back: Go back
       home: Go to Home
       quit: Quit

Type option: session-5

************************
*                      *
*   Braveheart         *
*                      *
*   at Cinemax North   *
*                      *
*   Thursday 22:20     *
*                      *
************************

               SCREEN
===================================

 [A00]  [A01]  [A02]  [A03]  [A04]

 [B00]  :B01:  :B02:  :B03:  [B04]

 [C00]  [C01]  [C02]  [C03]  [C04]

 [D00]  [D01]  [D02]  [D03]  [D04]

Legend:
  [RCC] Available
  &gt;RCC&lt; Selected by you
  |RCC| Selected by another person
  :RCC: Booked (unavailable)

Options:
   select-S: Select seat (example: select-B12)
 deselect-S: Deselect seat (example: deselect-B12)
       book: Book selected tickets
       back: Go back
       home: Go to Home
       quit: Quit

Type option: select-D01

************************
*                      *
*   Braveheart         *
*                      *
*   at Cinemax North   *
*                      *
*   Thursday 22:20     *
*                      *
************************

               SCREEN
===================================

 [A00]  [A01]  [A02]  [A03]  [A04]

 [B00]  :B01:  :B02:  :B03:  [B04]

 [C00]  [C01]  [C02]  [C03]  [C04]

 [D00]  &gt;D01&lt;  [D02]  [D03]  [D04]

Legend:
  [RCC] Available
  &gt;RCC&lt; Selected by you
  |RCC| Selected by another person
  :RCC: Booked (unavailable)

Options:
   select-S: Select seat (example: select-B12)
 deselect-S: Deselect seat (example: deselect-B12)
       book: Book selected tickets
       back: Go back
       home: Go to Home
       quit: Quit

Type option: select-D02

************************
*                      *
*   Braveheart         *
*                      *
*   at Cinemax North   *
*                      *
*   Thursday 22:20     *
*                      *
************************

               SCREEN
===================================

 [A00]  [A01]  [A02]  [A03]  [A04]

 [B00]  :B01:  :B02:  :B03:  [B04]

 [C00]  [C01]  [C02]  [C03]  [C04] 

 [D00]  &gt;D01&lt;  &gt;D02&lt;  [D03]  [D04]

Legend:
  [RCC] Available
  &gt;RCC&lt; Selected by you
  |RCC| Selected by another person
  :RCC: Booked (unavailable)

Options:
   select-S: Select seat (example: select-B12)
 deselect-S: Deselect seat (example: deselect-B12)
       book: Book selected tickets
       back: Go back
       home: Go to Home
       quit: Quit

Type option: select-D03

************************
*                      *
*   Braveheart         *
*                      *
*   at Cinemax North   *
*                      *
*   Thursday 22:20     *
*                      *
************************

               SCREEN
===================================

 [A00]  [A01]  [A02]  [A03]  [A04]

 [B00]  :B01:  :B02:  :B03:  [B04]

 [C00]  [C01]  [C02]  [C03]  [C04] 

 [D00]  &gt;D01&lt;  &gt;D02&lt;  &gt;D03&lt;  [D04]

Legend:
  [RCC] Available
  &gt;RCC&lt; Selected by you
  |RCC| Selected by another person
  :RCC: Booked (unavailable)

Options:
   select-S: Select seat (example: select-B12)
 deselect-S: Deselect seat (example: deselect-B12)
       book: Book selected tickets
       back: Go back
       home: Go to Home
       quit: Quit

Type option: book

************************
*                      *
*   BOOKING [76C9FM]   *
*                      *
************************

Braveheart
Cinemax North
Room 2 2D
Thursday 22:20

Seats:  D01  D02  D03 

Options:
       back: Go back
       home: Go to Home
       quit: Quit

Type option: back

************************
*                      *
*   Braveheart         *
*                      *
*   at Cinemax North   *
*                      *
*   Thursday 22:20     *
*                      *
************************

               SCREEN
===================================

 [A00]  [A01]  [A02]  [A03]  [A04]

 [B00]  :B01:  :B02:  :B03:  [B04]

 [C00]  [C01]  [C02]  [C03]  [C04]

 [D00]  :D01:  :D02:  :D03:  [D04]

Legend:
  [RCC] Available
  &gt;RCC&lt; Selected by you
  |RCC| Selected by another person
  :RCC: Booked (unavailable)

Options:
   select-S: Select seat (example: select-B12)
 deselect-S: Deselect seat (example: deselect-B12)
       book: Book selected tickets
       back: Go back
       home: Go to Home
       quit: Quit

Type option: back

******************************
*                            *
*   THEATER: Cinemax North   *
*                            *
******************************

Showtimes on Thursday:

Die Hard with a Vengeance
=========================

Room 1 2D
Session #0 at 17:10 .. ticket price: $12.50
Session #1 at 20:30 .. ticket price: $12.50

Seven
=====

Room 1 2D
Session #2 at 23:20 .. ticket price: $12.50

Braveheart
==========

Room 2 2D
Session #4 at 18:30 .. ticket price: $12.50
Session #5 at 22:20 .. ticket price: $12.50

Jumanji
=======

Room 2 2D
Session #3 at 14:40 .. ticket price: $12.50

Options:
  session-N: View session N (example: session-2)
       back: Go back
       home: Go to Home
       quit: Quit

Type option: back

*****************************************************************
*                                                               *
*   Select day to view sessions for the theater Cinemax North   *
*                                                               *
*****************************************************************

#0: Thursday
#1: Friday
#2: Saturday
#3: Sunday
#4: Monday
#5: Tuesday
#6: Wednesday

Options:
      day-N: Select day N (example: day-4)
       back: Go back
       home: Go to Home
       quit: Quit

Type option: back

****************
*              *
*   THEATERS   *
*              *
****************

Theater #0: Cinemax North
Theater #1: iMAX Boulevard

Options:
  theater-N: View theater N (example: theater-1)
       back: Go back
       home: Go to Home
       quit: Quit

Type option: back

************************
*                      *
*   CINE TICKET HOME   *
*                      *
************************

Options:
     movies: View movies
   theaters: View theaters
  booking-C: View booking (example booking-RT6U81)
       quit: Quit

Type option: booking-83YQF3

************************
*                      *
*   BOOKING [83YQF3]   *
*                      *
************************

Jumanji
iMAX Boulevard
Room 3 3D iMAX
Thursday 16:30

Seats:  B01  B02

Options:
       back: Go back
       home: Go to Home
       quit: Quit

Type option: home

************************
*                      *
*   CINE TICKET HOME   *
*                      *
************************

Options:
     movies: View movies
   theaters: View theaters
  booking-C: View booking (example booking-RT6U81)
       quit: Quit

Type option: booking-76C9FM

************************
*                      *
*   BOOKING [76C9FM]   *
*                      *
************************

Braveheart
Cinemax North
Room 2 2D
Thursday 22:20

Seats:  D01  D02  D03

Options:
       back: Go back
       home: Go to Home
       quit: Quit

Type option: quit

</code></pre>
</details>
