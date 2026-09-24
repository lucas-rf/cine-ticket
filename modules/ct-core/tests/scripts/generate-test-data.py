import argparse
import json
import random
import os

def generate_data(
    movie_count,
    genre_count,
    theater_count,
    room_count,
    day_count,
    session_count,
    room_rows,
    room_columns,
    booking_seat_min,
    booking_seat_max):
  
  movies = [{
      "key": f"movie-{movie_id}-key",
      "title": f"movie-{movie_id}-title",
      "classification": f"movie-{movie_id}-classification",
      "synopsis": f"movie-{movie_id}-synopsis",
      "genre": [f"movie-{movie_id}-genre-{g}" for g in range(genre_count)],
      "coverImage": f"movie-{movie_id}-coverImage",
      "backgroundImage": f"movie-{movie_id}-backgroundImage",
      "runtime": 100 + movie_id,
    } for movie_id in range(movie_count)]

  next_room_id = 0
  def get_next_room_id():
    nonlocal next_room_id
    id = next_room_id
    next_room_id += 1
    return id

  next_session_id = 0
  def get_next_session_id():
    nonlocal next_session_id
    id = next_session_id
    next_session_id += 1
    return id

  theaters = [{
    "name": f"theater-{theater_id}-name",
    "rooms": [{
      "type": f"theater-{theater_id}-room-{get_next_room_id()}-type",
      "rows": room_rows,
      "columns": room_columns,
      "sessions": [
        [{
          "movie-key": f"movie-{s // movie_count}-key",
          "time-hour": theater_id * 1000 + r * 100 + d * 10 + s // movie_count,
          "time-minute": get_next_session_id(),
          "price": theater_id * 1000 + r * 100 + d * 10 + s // movie_count + 0.1 * (s % movie_count),
          "bookings": []
        } for s in range(movie_count * session_count)
      ] for d in range(day_count)]
    } for r in range(room_count)]
  } for theater_id in range(theater_count)]

  bookings = [{
    "key": f"booking-{booking_id}-key",
    "time": f"2{booking_id:03d}-01-01 10:00:00",
    "email": f"booking-{booking_id}-email"
  } for booking_id in range(theater_count * room_count * day_count * movie_count * session_count * room_rows)]

  next_booking_id = 0
  for theater in theaters:
    for room in theater["rooms"]:
      for day in room["sessions"]:
        for session in day:
          for row in range(room_rows):
            seat_count = random.randint(booking_seat_min, booking_seat_max)
            start = random.randint(0, room_columns - seat_count)
            session["bookings"].append({
              "booking-key": f"booking-{next_booking_id}-key",
              "positions": [row * room_columns + start + p for p in range(seat_count)]
            })
            next_booking_id += 1
  
  return {
      "settings": {
        "cartDuration": 100
      },
      "movies": movies,
      "theaters": theaters,
      "bookings" : bookings
  }

def main():
  parser = argparse.ArgumentParser(prog="Cine-Ticket test data generator")
  parser.add_argument("output_file", help="Output file name")
  parser.add_argument("--movie-count", type=int, default=2, help="Number of movies")
  parser.add_argument("--genre-count", type=int, default=3, help="Number of genres each movie has")
  parser.add_argument("--theater-count", type=int, default=2, help="Number of theaters")
  parser.add_argument("--room-count", type=int, default=2, help="Number of rooms per theater")
  parser.add_argument("--day-count", type=int, default=2, help="Number of days")
  parser.add_argument("--session-count", type=int, default=2, help="Number of sessions per movie per day")
  parser.add_argument("--room-rows", type=int, default=2, help="Number of seat rows in the room")
  parser.add_argument("--room-columns", type=int, default=5, help="Number of seats per row in the room")
  parser.add_argument("--booking-seat-min", type=int, default=1, help="Minimum number of seats per booking")
  parser.add_argument("--booking-seat-max", type=int, default=4, help="Maximum number of seats per booking")

  args = parser.parse_args()

  data = generate_data(
    args.movie_count,
    args.genre_count,
    args.theater_count,
    args.room_count,
    args.day_count,
    args.session_count,
    args.room_rows,
    args.room_columns,
    args.booking_seat_min,
    args.booking_seat_max
  )

  with open(args.output_file, "w", encoding="utf-8") as file:
    json.dump(data, file, indent=2)

if __name__ == "__main__":
    main()