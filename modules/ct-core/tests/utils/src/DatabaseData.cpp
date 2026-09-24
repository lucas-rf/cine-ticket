#include <test-utils/DatabaseData.h>

namespace ct::test::utils::db_data
{
    const char* DB_DATA_REALISTIC_SHORT = R"(
    {
      "settings": {
        "cartDuration": 300
      },
      "movies": [
        {
          "key": "die-hard",
          "title": "Die Hard with a Vengeance",
          "classification": "R",
          "synopsis": "John McClane is forced out of suspension to play a game of \"Simon Says\" by a terrorist who has planted bombs all around New York City and will detonate them if McClane doesn't do what he says.",
          "genre": ["Action", "Adventure", "Thriller"],
          "coverImage": "die-hard-cover.png",
          "backgroundImage": "die-hard-background.png",
          "runtime": 128
        }
      ],
      "theaters": [
        {
          "name": "Cinemax North",
          "rooms": [
            {
              "type": "2D",
              "rows": 4,
              "columns": 5,
              "sessions": [
                [
                  {
                    "movie-key": "die-hard",
                    "time-hour": 20,
                    "time-minute": 30,
                    "price": 12.5,
                    "bookings": [
                      {
                        "booking-key": "G63RS1",
                        "positions": [11, 12]
                      }
                    ]
                  }
                ]
              ]
            }
          ]
        }
      ],
      "bookings": [
        {
          "key": "G63RS1",
          "time": "2025-03-15 14:12:35",
          "email": "martha.ceccato@outlook.com"
        }
      ]
    }
    )";

    const char* DB_DATA_SIMPLE_LONG = R"(
    {
      "settings": {
        "cartDuration": 100
      },
      "movies": [
        {
          "key": "movie-0-key",
          "title": "movie-0-title",
          "classification": "movie-0-classification",
          "synopsis": "movie-0-synopsis",
          "genre": [
            "movie-0-genre-0",
            "movie-0-genre-1",
            "movie-0-genre-2"
          ],
          "coverImage": "movie-0-coverImage",
          "backgroundImage": "movie-0-backgroundImage",
          "runtime": 100
        },
        {
          "key": "movie-1-key",
          "title": "movie-1-title",
          "classification": "movie-1-classification",
          "synopsis": "movie-1-synopsis",
          "genre": [
            "movie-1-genre-0",
            "movie-1-genre-1",
            "movie-1-genre-2"
          ],
          "coverImage": "movie-1-coverImage",
          "backgroundImage": "movie-1-backgroundImage",
          "runtime": 101
        }
      ],
    )"
    R"(
      "theaters": [
        {
          "name": "theater-0-name",
          "rooms": [
            {
              "type": "theater-0-room-0-type",
              "rows": 2,
              "columns": 5,
              "sessions": [
                [
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 0,
                    "time-minute": 0,
                    "price": 0.0,
                    "bookings": [
                      {
                        "booking-key": "booking-0-key",
                        "positions": [
                          0,
                          1,
                          2,
                          3
                        ]
                      },
                      {
                        "booking-key": "booking-1-key",
                        "positions": [
                          5,
                          6,
                          7
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 0,
                    "time-minute": 1,
                    "price": 0.1,
                    "bookings": [
                      {
                        "booking-key": "booking-2-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-3-key",
                        "positions": [
                          5,
                          6,
                          7,
                          8
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 1,
                    "time-minute": 2,
                    "price": 1.0,
                    "bookings": [
                      {
                        "booking-key": "booking-4-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-5-key",
                        "positions": [
                          6,
                          7,
                          8
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 1,
                    "time-minute": 3,
                    "price": 1.1,
                    "bookings": [
                      {
                        "booking-key": "booking-6-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-7-key",
                        "positions": [
                          8,
                          9
                        ]
                      }
                    ]
                  }
                ],
                [
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 10,
                    "time-minute": 4,
                    "price": 10.0,
                    "bookings": [
                      {
                        "booking-key": "booking-8-key",
                        "positions": [
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-9-key",
                        "positions": [
                          5,
                          6
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 10,
                    "time-minute": 5,
                    "price": 10.1,
                    "bookings": [
                      {
                        "booking-key": "booking-10-key",
                        "positions": [
                          0
                        ]
                      },
                      {
                        "booking-key": "booking-11-key",
                        "positions": [
                          6,
                          7,
                          8,
                          9
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 11,
                    "time-minute": 6,
                    "price": 11.0,
                    "bookings": [
                      {
                        "booking-key": "booking-12-key",
                        "positions": [
                          0,
                          1,
                          2,
                          3
                        ]
                      },
                      {
                        "booking-key": "booking-13-key",
                        "positions": [
                          8,
                          9
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 11,
                    "time-minute": 7,
                    "price": 11.1,
                    "bookings": [
                      {
                        "booking-key": "booking-14-key",
                        "positions": [
                          0,
                          1,
                          2,
                          3
                        ]
                      },
                      {
                        "booking-key": "booking-15-key",
                        "positions": [
                          7,
                          8,
                          9
                        ]
                      }
                    ]
                  }
                ]
              ]
            },
            {
              "type": "theater-0-room-1-type",
              "rows": 2,
              "columns": 5,
              "sessions": [
                [
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 100,
                    "time-minute": 8,
                    "price": 100.0,
                    "bookings": [
                      {
                        "booking-key": "booking-16-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-17-key",
                        "positions": [
                          5,
                          6,
                          7,
                          8
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 100,
                    "time-minute": 9,
                    "price": 100.1,
                    "bookings": [
                      {
                        "booking-key": "booking-18-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-19-key",
                        "positions": [
                          7,
                          8,
                          9
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 101,
                    "time-minute": 10,
                    "price": 101.0,
                    "bookings": [
                      {
                        "booking-key": "booking-20-key",
                        "positions": [
                          2
                        ]
                      },
                      {
                        "booking-key": "booking-21-key",
                        "positions": [
                          6,
                          7,
                          8,
                          9
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 101,
                    "time-minute": 11,
                    "price": 101.1,
                    "bookings": [
                      {
                        "booking-key": "booking-22-key",
                        "positions": [
                          2
                        ]
                      },
                      {
                        "booking-key": "booking-23-key",
                        "positions": [
                          6,
                          7,
                          8,
                          9
                        ]
                      }
                    ]
                  }
                ],
                [
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 110,
                    "time-minute": 12,
                    "price": 110.0,
                    "bookings": [
                      {
                        "booking-key": "booking-24-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-25-key",
                        "positions": [
                          7
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 110,
                    "time-minute": 13,
                    "price": 110.1,
                    "bookings": [
                      {
                        "booking-key": "booking-26-key",
                        "positions": [
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-27-key",
                        "positions": [
                          6,
                          7
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 111,
                    "time-minute": 14,
                    "price": 111.0,
                    "bookings": [
                      {
                        "booking-key": "booking-28-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-29-key",
                        "positions": [
                          5,
                          6
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 111,
                    "time-minute": 15,
                    "price": 111.1,
                    "bookings": [
                      {
                        "booking-key": "booking-30-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-31-key",
                        "positions": [
                          7,
                          8,
                          9
                        ]
                      }
                    ]
                  }
                ]
              ]
            }
          ]
        },
    )"
    R"(
        {
          "name": "theater-1-name",
          "rooms": [
            {
              "type": "theater-1-room-2-type",
              "rows": 2,
              "columns": 5,
              "sessions": [
                [
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 1000,
                    "time-minute": 16,
                    "price": 1000.0,
                    "bookings": [
                      {
                        "booking-key": "booking-32-key",
                        "positions": [
                          1,
                          2,
                          3
                        ]
                      },
                      {
                        "booking-key": "booking-33-key",
                        "positions": [
                          6,
                          7,
                          8,
                          9
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 1000,
                    "time-minute": 17,
                    "price": 1000.1,
                    "bookings": [
                      {
                        "booking-key": "booking-34-key",
                        "positions": [
                          1
                        ]
                      },
                      {
                        "booking-key": "booking-35-key",
                        "positions": [
                          7,
                          8
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 1001,
                    "time-minute": 18,
                    "price": 1001.0,
                    "bookings": [
                      {
                        "booking-key": "booking-36-key",
                        "positions": [
                          2
                        ]
                      },
                      {
                        "booking-key": "booking-37-key",
                        "positions": [
                          5
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 1001,
                    "time-minute": 19,
                    "price": 1001.1,
                    "bookings": [
                      {
                        "booking-key": "booking-38-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-39-key",
                        "positions": [
                          6,
                          7
                        ]
                      }
                    ]
                  }
                ],
                [
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 1010,
                    "time-minute": 20,
                    "price": 1010.0,
                    "bookings": [
                      {
                        "booking-key": "booking-40-key",
                        "positions": [
                          1,
                          2,
                          3
                        ]
                      },
                      {
                        "booking-key": "booking-41-key",
                        "positions": [
                          7
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 1010,
                    "time-minute": 21,
                    "price": 1010.1,
                    "bookings": [
                      {
                        "booking-key": "booking-42-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-43-key",
                        "positions": [
                          5,
                          6,
                          7,
                          8
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 1011,
                    "time-minute": 22,
                    "price": 1011.0,
                    "bookings": [
                      {
                        "booking-key": "booking-44-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-45-key",
                        "positions": [
                          5
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 1011,
                    "time-minute": 23,
                    "price": 1011.1,
                    "bookings": [
                      {
                        "booking-key": "booking-46-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-47-key",
                        "positions": [
                          7
                        ]
                      }
                    ]
                  }
                ]
              ]
            },
            {
              "type": "theater-1-room-3-type",
              "rows": 2,
              "columns": 5,
              "sessions": [
                [
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 1100,
                    "time-minute": 24,
                    "price": 1100.0,
                    "bookings": [
                      {
                        "booking-key": "booking-48-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-49-key",
                        "positions": [
                          6,
                          7,
                          8,
                          9
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 1100,
                    "time-minute": 25,
                    "price": 1100.1,
                    "bookings": [
                      {
                        "booking-key": "booking-50-key",
                        "positions": [
                          2,
                          3
                        ]
                      },
                      {
                        "booking-key": "booking-51-key",
                        "positions": [
                          6,
                          7,
                          8,
                          9
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 1101,
                    "time-minute": 26,
                    "price": 1101.0,
                    "bookings": [
                      {
                        "booking-key": "booking-52-key",
                        "positions": [
                          2
                        ]
                      },
                      {
                        "booking-key": "booking-53-key",
                        "positions": [
                          9
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 1101,
                    "time-minute": 27,
                    "price": 1101.1,
                    "bookings": [
                      {
                        "booking-key": "booking-54-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-55-key",
                        "positions": [
                          7,
                          8,
                          9
                        ]
                      }
                    ]
                  }
                ],
                [
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 1110,
                    "time-minute": 28,
                    "price": 1110.0,
                    "bookings": [
                      {
                        "booking-key": "booking-56-key",
                        "positions": [
                          0,
                          1
                        ]
                      },
                      {
                        "booking-key": "booking-57-key",
                        "positions": [
                          7,
                          8
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-0-key",
                    "time-hour": 1110,
                    "time-minute": 29,
                    "price": 1110.1,
                    "bookings": [
                      {
                        "booking-key": "booking-58-key",
                        "positions": [
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "booking-key": "booking-59-key",
                        "positions": [
                          5,
                          6,
                          7,
                          8
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 1111,
                    "time-minute": 30,
                    "price": 1111.0,
                    "bookings": [
                      {
                        "booking-key": "booking-60-key",
                        "positions": [
                          2
                        ]
                      },
                      {
                        "booking-key": "booking-61-key",
                        "positions": [
                          8
                        ]
                      }
                    ]
                  },
                  {
                    "movie-key": "movie-1-key",
                    "time-hour": 1111,
                    "time-minute": 31,
                    "price": 1111.1,
                    "bookings": [
                      {
                        "booking-key": "booking-62-key",
                        "positions": [
                          0,
                          1,
                          2
                        ]
                      },
                      {
                        "booking-key": "booking-63-key",
                        "positions": [
                          7,
                          8
                        ]
                      }
                    ]
                  }
                ]
              ]
            }
          ]
        }
      ],
    )"
    R"(
      "bookings": [
        {
          "key": "booking-0-key",
          "time": "2000-01-01 10:00:00",
          "email": "booking-0-email"
        },
        {
          "key": "booking-1-key",
          "time": "2001-01-01 10:00:00",
          "email": "booking-1-email"
        },
        {
          "key": "booking-2-key",
          "time": "2002-01-01 10:00:00",
          "email": "booking-2-email"
        },
        {
          "key": "booking-3-key",
          "time": "2003-01-01 10:00:00",
          "email": "booking-3-email"
        },
        {
          "key": "booking-4-key",
          "time": "2004-01-01 10:00:00",
          "email": "booking-4-email"
        },
        {
          "key": "booking-5-key",
          "time": "2005-01-01 10:00:00",
          "email": "booking-5-email"
        },
        {
          "key": "booking-6-key",
          "time": "2006-01-01 10:00:00",
          "email": "booking-6-email"
        },
        {
          "key": "booking-7-key",
          "time": "2007-01-01 10:00:00",
          "email": "booking-7-email"
        },
        {
          "key": "booking-8-key",
          "time": "2008-01-01 10:00:00",
          "email": "booking-8-email"
        },
        {
          "key": "booking-9-key",
          "time": "2009-01-01 10:00:00",
          "email": "booking-9-email"
        },
        {
          "key": "booking-10-key",
          "time": "2010-01-01 10:00:00",
          "email": "booking-10-email"
        },
        {
          "key": "booking-11-key",
          "time": "2011-01-01 10:00:00",
          "email": "booking-11-email"
        },
        {
          "key": "booking-12-key",
          "time": "2012-01-01 10:00:00",
          "email": "booking-12-email"
        },
        {
          "key": "booking-13-key",
          "time": "2013-01-01 10:00:00",
          "email": "booking-13-email"
        },
        {
          "key": "booking-14-key",
          "time": "2014-01-01 10:00:00",
          "email": "booking-14-email"
        },
        {
          "key": "booking-15-key",
          "time": "2015-01-01 10:00:00",
          "email": "booking-15-email"
        },
        {
          "key": "booking-16-key",
          "time": "2016-01-01 10:00:00",
          "email": "booking-16-email"
        },
        {
          "key": "booking-17-key",
          "time": "2017-01-01 10:00:00",
          "email": "booking-17-email"
        },
        {
          "key": "booking-18-key",
          "time": "2018-01-01 10:00:00",
          "email": "booking-18-email"
        },
        {
          "key": "booking-19-key",
          "time": "2019-01-01 10:00:00",
          "email": "booking-19-email"
        },
        {
          "key": "booking-20-key",
          "time": "2020-01-01 10:00:00",
          "email": "booking-20-email"
        },
        {
          "key": "booking-21-key",
          "time": "2021-01-01 10:00:00",
          "email": "booking-21-email"
        },
        {
          "key": "booking-22-key",
          "time": "2022-01-01 10:00:00",
          "email": "booking-22-email"
        },
        {
          "key": "booking-23-key",
          "time": "2023-01-01 10:00:00",
          "email": "booking-23-email"
        },
        {
          "key": "booking-24-key",
          "time": "2024-01-01 10:00:00",
          "email": "booking-24-email"
        },
        {
          "key": "booking-25-key",
          "time": "2025-01-01 10:00:00",
          "email": "booking-25-email"
        },
        {
          "key": "booking-26-key",
          "time": "2026-01-01 10:00:00",
          "email": "booking-26-email"
        },
        {
          "key": "booking-27-key",
          "time": "2027-01-01 10:00:00",
          "email": "booking-27-email"
        },
        {
          "key": "booking-28-key",
          "time": "2028-01-01 10:00:00",
          "email": "booking-28-email"
        },
        {
          "key": "booking-29-key",
          "time": "2029-01-01 10:00:00",
          "email": "booking-29-email"
        },
        {
          "key": "booking-30-key",
          "time": "2030-01-01 10:00:00",
          "email": "booking-30-email"
        },
        {
          "key": "booking-31-key",
          "time": "2031-01-01 10:00:00",
          "email": "booking-31-email"
        },
        {
          "key": "booking-32-key",
          "time": "2032-01-01 10:00:00",
          "email": "booking-32-email"
        },
        {
          "key": "booking-33-key",
          "time": "2033-01-01 10:00:00",
          "email": "booking-33-email"
        },
        {
          "key": "booking-34-key",
          "time": "2034-01-01 10:00:00",
          "email": "booking-34-email"
        },
        {
          "key": "booking-35-key",
          "time": "2035-01-01 10:00:00",
          "email": "booking-35-email"
        },
        {
          "key": "booking-36-key",
          "time": "2036-01-01 10:00:00",
          "email": "booking-36-email"
        },
        {
          "key": "booking-37-key",
          "time": "2037-01-01 10:00:00",
          "email": "booking-37-email"
        },
        {
          "key": "booking-38-key",
          "time": "2038-01-01 10:00:00",
          "email": "booking-38-email"
        },
        {
          "key": "booking-39-key",
          "time": "2039-01-01 10:00:00",
          "email": "booking-39-email"
        },
        {
          "key": "booking-40-key",
          "time": "2040-01-01 10:00:00",
          "email": "booking-40-email"
        },
        {
          "key": "booking-41-key",
          "time": "2041-01-01 10:00:00",
          "email": "booking-41-email"
        },
        {
          "key": "booking-42-key",
          "time": "2042-01-01 10:00:00",
          "email": "booking-42-email"
        },
        {
          "key": "booking-43-key",
          "time": "2043-01-01 10:00:00",
          "email": "booking-43-email"
        },
        {
          "key": "booking-44-key",
          "time": "2044-01-01 10:00:00",
          "email": "booking-44-email"
        },
        {
          "key": "booking-45-key",
          "time": "2045-01-01 10:00:00",
          "email": "booking-45-email"
        },
        {
          "key": "booking-46-key",
          "time": "2046-01-01 10:00:00",
          "email": "booking-46-email"
        },
        {
          "key": "booking-47-key",
          "time": "2047-01-01 10:00:00",
          "email": "booking-47-email"
        },
        {
          "key": "booking-48-key",
          "time": "2048-01-01 10:00:00",
          "email": "booking-48-email"
        },
        {
          "key": "booking-49-key",
          "time": "2049-01-01 10:00:00",
          "email": "booking-49-email"
        },
        {
          "key": "booking-50-key",
          "time": "2050-01-01 10:00:00",
          "email": "booking-50-email"
        },
        {
          "key": "booking-51-key",
          "time": "2051-01-01 10:00:00",
          "email": "booking-51-email"
        },
        {
          "key": "booking-52-key",
          "time": "2052-01-01 10:00:00",
          "email": "booking-52-email"
        },
        {
          "key": "booking-53-key",
          "time": "2053-01-01 10:00:00",
          "email": "booking-53-email"
        },
        {
          "key": "booking-54-key",
          "time": "2054-01-01 10:00:00",
          "email": "booking-54-email"
        },
        {
          "key": "booking-55-key",
          "time": "2055-01-01 10:00:00",
          "email": "booking-55-email"
        },
        {
          "key": "booking-56-key",
          "time": "2056-01-01 10:00:00",
          "email": "booking-56-email"
        },
        {
          "key": "booking-57-key",
          "time": "2057-01-01 10:00:00",
          "email": "booking-57-email"
        },
        {
          "key": "booking-58-key",
          "time": "2058-01-01 10:00:00",
          "email": "booking-58-email"
        },
        {
          "key": "booking-59-key",
          "time": "2059-01-01 10:00:00",
          "email": "booking-59-email"
        },
        {
          "key": "booking-60-key",
          "time": "2060-01-01 10:00:00",
          "email": "booking-60-email"
        },
        {
          "key": "booking-61-key",
          "time": "2061-01-01 10:00:00",
          "email": "booking-61-email"
        },
        {
          "key": "booking-62-key",
          "time": "2062-01-01 10:00:00",
          "email": "booking-62-email"
        },
        {
          "key": "booking-63-key",
          "time": "2063-01-01 10:00:00",
          "email": "booking-63-email"
        }
      ]
    }
    )";
}
