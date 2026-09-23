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
                    "orders": [
                      {
                        "order-key": "G63RS1",
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
      "orders": [
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
                    "orders": [
                      {
                        "order-key": "order-0-key",
                        "positions": [
                          0,
                          1,
                          2,
                          3
                        ]
                      },
                      {
                        "order-key": "order-1-key",
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
                    "orders": [
                      {
                        "order-key": "order-2-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-3-key",
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
                    "orders": [
                      {
                        "order-key": "order-4-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-5-key",
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
                    "orders": [
                      {
                        "order-key": "order-6-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-7-key",
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
                    "orders": [
                      {
                        "order-key": "order-8-key",
                        "positions": [
                          4
                        ]
                      },
                      {
                        "order-key": "order-9-key",
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
                    "orders": [
                      {
                        "order-key": "order-10-key",
                        "positions": [
                          0
                        ]
                      },
                      {
                        "order-key": "order-11-key",
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
                    "orders": [
                      {
                        "order-key": "order-12-key",
                        "positions": [
                          0,
                          1,
                          2,
                          3
                        ]
                      },
                      {
                        "order-key": "order-13-key",
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
                    "orders": [
                      {
                        "order-key": "order-14-key",
                        "positions": [
                          0,
                          1,
                          2,
                          3
                        ]
                      },
                      {
                        "order-key": "order-15-key",
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
                    "orders": [
                      {
                        "order-key": "order-16-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-17-key",
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
                    "orders": [
                      {
                        "order-key": "order-18-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-19-key",
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
                    "orders": [
                      {
                        "order-key": "order-20-key",
                        "positions": [
                          2
                        ]
                      },
                      {
                        "order-key": "order-21-key",
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
                    "orders": [
                      {
                        "order-key": "order-22-key",
                        "positions": [
                          2
                        ]
                      },
                      {
                        "order-key": "order-23-key",
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
                    "orders": [
                      {
                        "order-key": "order-24-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-25-key",
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
                    "orders": [
                      {
                        "order-key": "order-26-key",
                        "positions": [
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-27-key",
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
                    "orders": [
                      {
                        "order-key": "order-28-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-29-key",
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
                    "orders": [
                      {
                        "order-key": "order-30-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-31-key",
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
                    "orders": [
                      {
                        "order-key": "order-32-key",
                        "positions": [
                          1,
                          2,
                          3
                        ]
                      },
                      {
                        "order-key": "order-33-key",
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
                    "orders": [
                      {
                        "order-key": "order-34-key",
                        "positions": [
                          1
                        ]
                      },
                      {
                        "order-key": "order-35-key",
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
                    "orders": [
                      {
                        "order-key": "order-36-key",
                        "positions": [
                          2
                        ]
                      },
                      {
                        "order-key": "order-37-key",
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
                    "orders": [
                      {
                        "order-key": "order-38-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-39-key",
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
                    "orders": [
                      {
                        "order-key": "order-40-key",
                        "positions": [
                          1,
                          2,
                          3
                        ]
                      },
                      {
                        "order-key": "order-41-key",
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
                    "orders": [
                      {
                        "order-key": "order-42-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-43-key",
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
                    "orders": [
                      {
                        "order-key": "order-44-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-45-key",
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
                    "orders": [
                      {
                        "order-key": "order-46-key",
                        "positions": [
                          1,
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-47-key",
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
                    "orders": [
                      {
                        "order-key": "order-48-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-49-key",
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
                    "orders": [
                      {
                        "order-key": "order-50-key",
                        "positions": [
                          2,
                          3
                        ]
                      },
                      {
                        "order-key": "order-51-key",
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
                    "orders": [
                      {
                        "order-key": "order-52-key",
                        "positions": [
                          2
                        ]
                      },
                      {
                        "order-key": "order-53-key",
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
                    "orders": [
                      {
                        "order-key": "order-54-key",
                        "positions": [
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-55-key",
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
                    "orders": [
                      {
                        "order-key": "order-56-key",
                        "positions": [
                          0,
                          1
                        ]
                      },
                      {
                        "order-key": "order-57-key",
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
                    "orders": [
                      {
                        "order-key": "order-58-key",
                        "positions": [
                          2,
                          3,
                          4
                        ]
                      },
                      {
                        "order-key": "order-59-key",
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
                    "orders": [
                      {
                        "order-key": "order-60-key",
                        "positions": [
                          2
                        ]
                      },
                      {
                        "order-key": "order-61-key",
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
                    "orders": [
                      {
                        "order-key": "order-62-key",
                        "positions": [
                          0,
                          1,
                          2
                        ]
                      },
                      {
                        "order-key": "order-63-key",
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
      "orders": [
        {
          "key": "order-0-key",
          "time": "2000-01-01 10:00:00",
          "email": "order-0-email"
        },
        {
          "key": "order-1-key",
          "time": "2001-01-01 10:00:00",
          "email": "order-1-email"
        },
        {
          "key": "order-2-key",
          "time": "2002-01-01 10:00:00",
          "email": "order-2-email"
        },
        {
          "key": "order-3-key",
          "time": "2003-01-01 10:00:00",
          "email": "order-3-email"
        },
        {
          "key": "order-4-key",
          "time": "2004-01-01 10:00:00",
          "email": "order-4-email"
        },
        {
          "key": "order-5-key",
          "time": "2005-01-01 10:00:00",
          "email": "order-5-email"
        },
        {
          "key": "order-6-key",
          "time": "2006-01-01 10:00:00",
          "email": "order-6-email"
        },
        {
          "key": "order-7-key",
          "time": "2007-01-01 10:00:00",
          "email": "order-7-email"
        },
        {
          "key": "order-8-key",
          "time": "2008-01-01 10:00:00",
          "email": "order-8-email"
        },
        {
          "key": "order-9-key",
          "time": "2009-01-01 10:00:00",
          "email": "order-9-email"
        },
        {
          "key": "order-10-key",
          "time": "2010-01-01 10:00:00",
          "email": "order-10-email"
        },
        {
          "key": "order-11-key",
          "time": "2011-01-01 10:00:00",
          "email": "order-11-email"
        },
        {
          "key": "order-12-key",
          "time": "2012-01-01 10:00:00",
          "email": "order-12-email"
        },
        {
          "key": "order-13-key",
          "time": "2013-01-01 10:00:00",
          "email": "order-13-email"
        },
        {
          "key": "order-14-key",
          "time": "2014-01-01 10:00:00",
          "email": "order-14-email"
        },
        {
          "key": "order-15-key",
          "time": "2015-01-01 10:00:00",
          "email": "order-15-email"
        },
        {
          "key": "order-16-key",
          "time": "2016-01-01 10:00:00",
          "email": "order-16-email"
        },
        {
          "key": "order-17-key",
          "time": "2017-01-01 10:00:00",
          "email": "order-17-email"
        },
        {
          "key": "order-18-key",
          "time": "2018-01-01 10:00:00",
          "email": "order-18-email"
        },
        {
          "key": "order-19-key",
          "time": "2019-01-01 10:00:00",
          "email": "order-19-email"
        },
        {
          "key": "order-20-key",
          "time": "2020-01-01 10:00:00",
          "email": "order-20-email"
        },
        {
          "key": "order-21-key",
          "time": "2021-01-01 10:00:00",
          "email": "order-21-email"
        },
        {
          "key": "order-22-key",
          "time": "2022-01-01 10:00:00",
          "email": "order-22-email"
        },
        {
          "key": "order-23-key",
          "time": "2023-01-01 10:00:00",
          "email": "order-23-email"
        },
        {
          "key": "order-24-key",
          "time": "2024-01-01 10:00:00",
          "email": "order-24-email"
        },
        {
          "key": "order-25-key",
          "time": "2025-01-01 10:00:00",
          "email": "order-25-email"
        },
        {
          "key": "order-26-key",
          "time": "2026-01-01 10:00:00",
          "email": "order-26-email"
        },
        {
          "key": "order-27-key",
          "time": "2027-01-01 10:00:00",
          "email": "order-27-email"
        },
        {
          "key": "order-28-key",
          "time": "2028-01-01 10:00:00",
          "email": "order-28-email"
        },
        {
          "key": "order-29-key",
          "time": "2029-01-01 10:00:00",
          "email": "order-29-email"
        },
        {
          "key": "order-30-key",
          "time": "2030-01-01 10:00:00",
          "email": "order-30-email"
        },
        {
          "key": "order-31-key",
          "time": "2031-01-01 10:00:00",
          "email": "order-31-email"
        },
        {
          "key": "order-32-key",
          "time": "2032-01-01 10:00:00",
          "email": "order-32-email"
        },
        {
          "key": "order-33-key",
          "time": "2033-01-01 10:00:00",
          "email": "order-33-email"
        },
        {
          "key": "order-34-key",
          "time": "2034-01-01 10:00:00",
          "email": "order-34-email"
        },
        {
          "key": "order-35-key",
          "time": "2035-01-01 10:00:00",
          "email": "order-35-email"
        },
        {
          "key": "order-36-key",
          "time": "2036-01-01 10:00:00",
          "email": "order-36-email"
        },
        {
          "key": "order-37-key",
          "time": "2037-01-01 10:00:00",
          "email": "order-37-email"
        },
        {
          "key": "order-38-key",
          "time": "2038-01-01 10:00:00",
          "email": "order-38-email"
        },
        {
          "key": "order-39-key",
          "time": "2039-01-01 10:00:00",
          "email": "order-39-email"
        },
        {
          "key": "order-40-key",
          "time": "2040-01-01 10:00:00",
          "email": "order-40-email"
        },
        {
          "key": "order-41-key",
          "time": "2041-01-01 10:00:00",
          "email": "order-41-email"
        },
        {
          "key": "order-42-key",
          "time": "2042-01-01 10:00:00",
          "email": "order-42-email"
        },
        {
          "key": "order-43-key",
          "time": "2043-01-01 10:00:00",
          "email": "order-43-email"
        },
        {
          "key": "order-44-key",
          "time": "2044-01-01 10:00:00",
          "email": "order-44-email"
        },
        {
          "key": "order-45-key",
          "time": "2045-01-01 10:00:00",
          "email": "order-45-email"
        },
        {
          "key": "order-46-key",
          "time": "2046-01-01 10:00:00",
          "email": "order-46-email"
        },
        {
          "key": "order-47-key",
          "time": "2047-01-01 10:00:00",
          "email": "order-47-email"
        },
        {
          "key": "order-48-key",
          "time": "2048-01-01 10:00:00",
          "email": "order-48-email"
        },
        {
          "key": "order-49-key",
          "time": "2049-01-01 10:00:00",
          "email": "order-49-email"
        },
        {
          "key": "order-50-key",
          "time": "2050-01-01 10:00:00",
          "email": "order-50-email"
        },
        {
          "key": "order-51-key",
          "time": "2051-01-01 10:00:00",
          "email": "order-51-email"
        },
        {
          "key": "order-52-key",
          "time": "2052-01-01 10:00:00",
          "email": "order-52-email"
        },
        {
          "key": "order-53-key",
          "time": "2053-01-01 10:00:00",
          "email": "order-53-email"
        },
        {
          "key": "order-54-key",
          "time": "2054-01-01 10:00:00",
          "email": "order-54-email"
        },
        {
          "key": "order-55-key",
          "time": "2055-01-01 10:00:00",
          "email": "order-55-email"
        },
        {
          "key": "order-56-key",
          "time": "2056-01-01 10:00:00",
          "email": "order-56-email"
        },
        {
          "key": "order-57-key",
          "time": "2057-01-01 10:00:00",
          "email": "order-57-email"
        },
        {
          "key": "order-58-key",
          "time": "2058-01-01 10:00:00",
          "email": "order-58-email"
        },
        {
          "key": "order-59-key",
          "time": "2059-01-01 10:00:00",
          "email": "order-59-email"
        },
        {
          "key": "order-60-key",
          "time": "2060-01-01 10:00:00",
          "email": "order-60-email"
        },
        {
          "key": "order-61-key",
          "time": "2061-01-01 10:00:00",
          "email": "order-61-email"
        },
        {
          "key": "order-62-key",
          "time": "2062-01-01 10:00:00",
          "email": "order-62-email"
        },
        {
          "key": "order-63-key",
          "time": "2063-01-01 10:00:00",
          "email": "order-63-email"
        }
      ]
    }
    )";
}
