function doGet() {
  var now = new Date();

  var startSearch = new Date(now.getTime() - 7 * 24 * 60 * 60 * 1000);
  var endSearch = new Date(now.getTime() + 7 * 24 * 60 * 60 * 1000);

  var calendar = CalendarApp.getDefaultCalendar();
  var events = calendar.getEvents(startSearch, endSearch);

  var nextEvent = null;

  for (var i = 0; i < events.length; i++) {
    var start = events[i].getStartTime();

    if (start > now) {
      nextEvent = events[i];
      break;
    }
  }

  if (!nextEvent) {
    return ContentService.createTextOutput("No upcoming events");
  }

  // Event info
  var eventTitle = nextEvent.getTitle();
  var eventLocation = nextEvent.getLocation() || "No location";

  var eventDate = Utilities.formatDate(
    nextEvent.getStartTime(),
    Session.getScriptTimeZone(),
    "EEEE MMM d"
  );

  var eventTime = Utilities.formatDate(
    nextEvent.getStartTime(),
    Session.getScriptTimeZone(),
    "h:mm a"
  );

  var diffMs = nextEvent.getStartTime() - now;

  var totalHours = diffMs / (1000 * 60 * 60);

  var timeUntil;

  if (totalHours < 1) {
    timeUntil = "Now";
  } else {
    var days = Math.floor(diffMs / (1000 * 60 * 60 * 24));
    var hours = Math.floor(diffMs / (1000 * 60 * 60));

    if (days >= 1) {
      var remainingHours = hours % 24;
      timeUntil = days + "d " + remainingHours + "h";
    } else {
      timeUntil = hours + "h";
    }
  }

  var lat = 43.653482;
  var lon = -79.383935;

  var weatherUrl =
    "https://api.open-meteo.com/v1/forecast?latitude=" +
    lat +
    "&longitude=" +
    lon +
    "&current_weather=true";

  var response = UrlFetchApp.fetch(weatherUrl);
  var data = JSON.parse(response.getContentText());

  var temp = data.current_weather.temperature;
  var windSpeed = data.current_weather.windspeed;
  var code = data.current_weather.weathercode;

  var weatherState = getWeatherDesc(code);

  var result =
    timeUntil + ", " +
    eventTitle + ", " +
    eventLocation + ", " +
    eventDate + ", " +
    eventTime + ", " +
    weatherState + ", " +
    temp + " C, " +
    windSpeed + " km/h";

  return ContentService.createTextOutput(result);
}

function getWeatherDesc(code) {
  var map = {
    0: "Clear",
    1: "Mostly clear",
    2: "Partly cloudy",
    3: "Cloudy",
    45: "Fog",
    48: "Fog",
    51: "Light drizzle",
    53: "Drizzle",
    55: "Heavy drizzle",
    61: "Light rain",
    63: "Rain",
    65: "Heavy rain",
    71: "Light snow",
    73: "Snow",
    75: "Heavy snow",
    95: "Thunderstorm"
  };

  return map[code] || "Unknown";
}
