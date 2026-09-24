import requests
from urllib.parse import urljoin
import re
import argparse
from rich import print as richprint


DAYS = ("Thursday", "Friday", "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday")


class ViewBase:
  def __init__(self, app, isHome=False):
    self.app = app
    self.isHome = isHome

  def GetOptions(self):
    if self.isHome:
      opts = []
    else:
      opts = [
        (r"^back$", "       back: Go back", "back", {}),
        (r"^home$", "       home: Go to Home", "home", {})
      ]
    opts.extend([
      (r"^quit$", "       quit: Quit", "quit", {})
    ])
    return opts
  
  def Action_home(self):
    self.app.GoHome()

  def Action_back(self):
    self.app.GoBack()

  def Action_quit(self):
    self.app.Quit()


class ViewHome(ViewBase):
  def __init__(self, app):
    super().__init__(app, True)

  def Render(self):
    print()
    print("************************")
    print("*                      *")
    print("*   CINE TICKET HOME   *")
    print("*                      *")
    print("************************")
    print()

  def GetPath(self):
    return "home"
  
  def GetOptions(self):
    return [
      (r"^\s*movies\s*$", "     movies: View movies", "movies", {}),
      (r"^\s*theaters\s*$", "   theaters: View theaters", "theaters", {}),
      (r"^\s*booking-([0-9A-Z]+)\s*$", "  booking-C: View booking (example booking-RT6U81)", "booking", {"bookingKey": str}),
    ] + super().GetOptions()
  
  def Action_movies(self):
    self.app.NavigateTo("movies")

  def Action_theaters(self):
    self.app.NavigateTo("theaters")

  def Action_booking(self, bookingKey):
    self.app.NavigateTo("booking", bookingKey)


class ViewMovies(ViewBase):
  def __init__(self, app):
    super().__init__(app)

  def Render(self):
    movies = self.app.GET("movies")
    print()
    print("**************")
    print("*            *")
    print("*   MOVIES   *")
    print("*            *")
    print("**************")
    print()

    self.titles = {}
    for movie in movies:
      self.titles[movie["id"]] = movie["title"]
      print(f"Movie #{movie['id']}")
      print(f"Title.........: {movie['title']}")
      print(f"Synopsis......: {movie['synopsis']}")
      print(f"Classification: {movie['classification']}")
      print(f"Genre.........: {' '.join(movie['genre'])}")
      print(f"Runtime.......: {movie['runtime']} minutes\n")

  def GetPath(self):
    return "movies"

  def GetOptions(self):
    return [
      (r"^\s*movie-(\d+)\s*$", "    movie-N: View movie N (example: movie-2)", "movie", {"movieId": int})
    ] + super().GetOptions()
  
  def Action_movie(self, movieId):
    self.app.NavigateTo("days", text=f"the movie {self.titles[movieId]}", redirPage="movie", movieId=movieId)


class ViewTheaters(ViewBase):
  def __init__(self, app):
    super().__init__(app)

  def Render(self):
    theaters = self.app.GET("theaters")
    print()
    print("****************")
    print("*              *")
    print("*   THEATERS   *")
    print("*              *")
    print("****************")
    print()

    self.names = {}
    for theater in theaters:
      self.names[theater["id"]] = theater["name"]
      print(f"Theater #{theater['id']}: {theater['name']}")
    print()

  def GetPath(self):
    return "theaters"

  def GetOptions(self):
    return [
      (r"^\s*theater-(\d+)\s*$", "  theater-N: View theater N (example: theater-1)", "theater", {"theaterId": int})
    ] + super().GetOptions()
  
  def Action_theater(self, theaterId):
    self.app.NavigateTo("days", text=f"the theater {self.names[theaterId]}", redirPage="theater", theaterId=theaterId, name=self.names[theaterId])


class ViewDays(ViewBase):
  def __init__(self, app):
    super().__init__(app)

  def Render(self, text, redirPage, **kwargs):
    self.kwargs = kwargs
    self.kwargs["page"] = redirPage

    print()
    print("*" * (len(text) + 40))
    print("*" + " " * (len(text) + 38) + "*")
    print(f"*   Select day to view sessions for {text}   *")
    print("*" + " " * (len(text) + 38) + "*")
    print("*" * (len(text) + 40))
    print()
    print('\n'.join([f"#{day}: {name}" for day, name in enumerate(DAYS)]))
    print()

  def GetPath(self):
    return "days"

  def GetOptions(self):
    return [
      (r"^\s*day-(\d+)\s*$", "      day-N: Select day N (example: day-4)", "day", {"day": int})
    ] + super().GetOptions()
  
  def Action_day(self, day):
    if day >= len(DAYS):
      raise Exception("Invalid day!")
    self.app.NavigateTo(day=day, **self.kwargs)


class ViewMovieByDay(ViewBase):
  def __init__(self, app):
    super().__init__(app)

  def Render(self, movieId, day):
    self.movieId = movieId
    self.day = day

    movie = self.app.GET(f"movie/{movieId}/{day}")

    print()
    print("*" * (len(movie['title']) + 15))
    print("*" + " " * (len(movie['title']) + 13) + "*")
    print(f"*   MOVIE: {movie['title']}   *")
    print("*" + " " * (len(movie['title']) + 13) + "*")
    print("*" * (len(movie['title']) + 15))
    print()

    print(f"Synopsis......: {movie['synopsis']}")
    print(f"Classification: {movie['classification']}")
    print(f"Genre.........: {' '.join(movie['genre'])}")
    print(f"Runtime.......: {movie['runtime']} minutes\n")

    if not movie["theaters"]:
      print(f"There are now showtimes for this movie on {DAYS[day]}.\n")
      return

    print(f"Showtimes on {DAYS[day]}:\n")

    for theater in movie["theaters"]:
      print(f"Theater: {theater['name']}")
      for room in theater["rooms"]:
        print(f"Room {room['number']} {room['type']}")
        for session in room["sessions"]:
          print(f"Session #{session['id']} at {session['time'] // 60}:{session['time'] % 60} .. ticket price: ${session['price']:.2f}")
        print()

  def GetPath(self):
    return "movie"

  def GetOptions(self):
    return [
      (r"^\s*session-(\d+)\s*$", "  session-N: View session N (example: session-2)", "session", {"movieSessionId": int})
    ] + super().GetOptions()
  
  def Action_session(self, movieSessionId):
    self.app.NavigateTo("session", movieSessionId)


class ViewTheaterByDay(ViewBase):
  def __init__(self, app):
    super().__init__(app)

  def Render(self, theaterId, name, day):

    movies = self.app.GET(f"movies/{theaterId}/{day}")

    print()
    print("*" * (len(name) + 17))
    print("*" + " " * (len(name) + 15) + "*")
    print(f"*  THEATER: {name}  *")
    print("*" + " " * (len(name) + 15) + "*")
    print("*" * (len(name) + 17))
    print()

    if not movies:
      print(f"There are now showtimes in this theater on {DAYS[day]}.\n")
      return

    print(f"Showtimes on {DAYS[day]}:\n")

    for movie in movies:
      print(movie["title"])
      print("=" * len(movie["title"]))
      print()
      for room in movie["theaters"][0]["rooms"]:
        print(f"Room {room['number']} {room['type']}")
        for session in room["sessions"]:
          print(f"Session #{session['id']} at {session['time'] // 60}:{session['time'] % 60} .. ticket price: ${session['price']:.2f}")
        print()

  def GetPath(self):
    return "theater"

  def GetOptions(self):
    return [
      (r"^\s*session-(\d+)\s*$", "  session-N: View session N (example: session-2)", "session", {"movieSessionId": int})
    ] + super().GetOptions()
  
  def Action_session(self, movieSessionId):
    self.app.NavigateTo("session", movieSessionId)


class ViewMovieSession(ViewBase):
  def __init__(self, app):
    super().__init__(app)

  FORMAT = {
    "FREE": "[bright_yellow]{}[/bright_yellow]",
    "SELECTED_BY_CURRENT_USER": "[bright_blue]{}[/bright_blue]",
    "SELECTED_BY_OTHER_USER": "[red]{}[/red]",
    "BOOKED": "[bright_black]{}[/bright_black]",
  }

  def Render(self, movieSessionId):

    session = self.app.GET(f"roomSession/{movieSessionId}")

    movieTitle = session["movie"]["title"]
    theaterName = session["theater"]["name"]
    when = f"{DAYS[session["day"]]} {session["time"] // 60}:{session["time"] % 60}"

    maxlen = max(max(len(movieTitle), len(theaterName) + 3), len(when))

    print()
    print("*" * (maxlen + 8))
    print("*" + " " * (maxlen + 6) + "*")
    print(f"*   {movieTitle}   " + " " * (maxlen - len(movieTitle)) + "*")
    print("*" + " " * (maxlen + 6) + "*")
    print(f"*   at {theaterName}   " + " " * (maxlen - len(theaterName) - 3) + "*")
    print("*" + " " * (maxlen + 6) + "*")
    print(f"*   {when}   " + " " * (maxlen - len(when)) + "*")
    print("*" + " " * (maxlen + 6) + "*")
    print("*" * (maxlen + 8))
    print()

    rows = session["room"]["rows"]
    columns = session["room"]["columns"]

    screenSize = 7 * columns
    print(" " * ((screenSize - 6) // 2), "SCREEN")
    print("=" * screenSize)
    print()

    format = [["" for c in range(columns)] for r in range(rows)]
    self.seatIds = [[0 for c in range(columns)] for r in range(rows)]
    for seat in session["seats"]:
      format[seat["row"]][seat["column"]] = ViewMovieSession.FORMAT[seat["state"]]
      self.seatIds[seat["row"]][seat["column"]] = seat["id"]

    for row in range(rows):
      for column in range(columns):
        richprint(format[row][column].format(f" [{chr(ord('A') + row)}{column:02d}] "), end="")
      print("\n")

    print("Seat states:")
    richprint("[bright_yellow]  Available[/bright_yellow]")
    richprint("[bright_blue]  Selected by you[/bright_blue]")
    richprint("[red]  Selected by another person[/red]")
    richprint("[bright_black]  Booked (unavailable)[/bright_black]")
    print()

  def GetPath(self):
    return "session"

  def GetOptions(self):
    return [
      (r"^\s*select-([A-Z])(\d+)\s*$", "   select-S: Select seat (example: select-B12)", "select", {"row": str, "column": int}),
      (r"^\s*deselect-([A-Z])(\d+)\s*$", " deselect-S: Deselect seat (example: deselect-B12)", "deselect", {"row": str, "column": int}),
      (r"^\s*book\s*$", "       book: Book selected tickets", "book", {}),
    ] + super().GetOptions()
  
  def _getSeatId(self, row, column):
    rowNum = ord(row) - ord('A')
    if rowNum >= len(self.seatIds) or column >= len(self.seatIds[0]):
      raise Exception("Invalid seat!")
    return self.seatIds[rowNum][column]

  def Action_select(self, row, column):
    id = self._getSeatId(row, column)
    self.app.POST(f"seat/{id}/select")

  def Action_deselect(self, row, column):
    id = self._getSeatId(row, column)
    self.app.POST(f"seat/{id}/deselect")

  def Action_book(self):
    booking = self.app.POST(f"book/email")
    if not booking:
      print("Not possible to book, cart is empty or expired!")
      return
    self.app.NavigateTo("booking", booking["bookingKey"])


class ViewBooking(ViewBase):
  def __init__(self, app):
    super().__init__(app)

  def Render(self, bookingKey):

    booking = self.app.GET(f"booking/{bookingKey}")
    movieSession = self.app.GET(f"roomSession/{booking["movieSessionId"]}")

    print()
    print("*" * (len(bookingKey) + 18))
    print("*" + " " * (len(bookingKey) + 16) + "*")
    print(f"*   BOOKING [{bookingKey}]   *")
    print("*" + " " * (len(bookingKey) + 16) + "*")
    print("*" * (len(bookingKey) + 18))
    print()

    print(movieSession["movie"]["title"])
    print(movieSession["theater"]["name"])
    print("Room", movieSession["room"]["number"], movieSession["room"]["type"])
    print(DAYS[movieSession["day"]], f"{movieSession["time"] // 60}:{movieSession["time"] % 60}")
    print()
    print("Seats: ", end="")

    for seat in booking["seats"]:
      seatNumber = f"{chr(ord('A') + seat['row'])}{seat['column']:02d}"
      richprint(f" [bright_blue]{seatNumber}[/bright_blue] ", end="")
    print("\n")

  def GetPath(self):
    return "booking"


class Application:
  def __init__(self, url):
    self.url = url
    self.session = requests.Session()
    self.views = {}
    self.home = ViewHome(self)
    self.stack = [(self.home, (), {})]
    self._addView(self.home)
    self._addView(ViewMovies(self))
    self._addView(ViewTheaters(self))
    self._addView(ViewDays(self))
    self._addView(ViewMovieByDay(self))
    self._addView(ViewTheaterByDay(self))
    self._addView(ViewMovieSession(self))
    self._addView(ViewBooking(self))

  def _addView(self, view):
    self.views[view.GetPath()] = view

  def NavigateTo(self, page, *args, **kwargs):
    self.stack.append((self.views[page], args, kwargs))

  def RenderView(self, view, args, kwargs):
    view.Render(*args, **kwargs)
    self._renderOptions(view)

  def _renderOptions(self, view):
    print("Options:")
    options = view.GetOptions()
    for option in options:
      print(option[1])
    print("\nType option: ", end="")

  def GoHome(self):
    self.stack = [(self.home, (), {})]

  def GoBack(self):
    if len(self.stack) > 1:
      self.stack.pop()

  def Quit(self):
    self.stack.clear()

  def GET(self, path):
    get_response = self.session.get(urljoin(self.url, path))
    if get_response.status_code == 200:
      return get_response.json()
    raise Exception(f"Error {get_response.status_code}: '{get_response.text}'")
  
  def POST(self, path):
    post_response = self.session.post(urljoin(self.url, path))
    if post_response.status_code == 200:
      return post_response.json()
    raise Exception(f"Error {post_response.status_code}: '{post_response.text}'")
  
  def _captureOption(self, view):
      cmd = input()
      options = view.GetOptions()
      for regex, descr, action, argtypes in options:
        match = re.search(regex, cmd)
        if match:
          nextgrp = 1
          outargs = {}
          for argname, argtype in argtypes.items():
            outargs[argname] = argtype(match.group(nextgrp))
            nextgrp += 1
          return action, outargs
      print(f"\nInvalid option!\n")
      return None, None
  
  def _execAction(self, view, action, kwargs):
    method = getattr(view, f"Action_{action}")
    method(**kwargs)

  def Run(self):
    while True:
      if not self.stack:
        break

      view, args, kwargs = self.stack[-1]

      try:
        self.RenderView(view, args, kwargs)
      except Exception as e:
        print(f"Failed to render view: {e}")
        self.GoBack()
        continue

      action, outargs = self._captureOption(view)
      if action:
        try:
          self._execAction(view, action, outargs)
        except Exception as e:
          print(f"Failed to execute action: {e}")


def main():
  parser = argparse.ArgumentParser(prog="Cine-Ticket CLI")
  parser.add_argument("url", help="API URL")
  args = parser.parse_args()
  app = Application(args.url)
  app.Run()

if __name__ == "__main__":
  main()
