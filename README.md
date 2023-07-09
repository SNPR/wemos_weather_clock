# Wemos Weather Clock

The Wemos Weather Clock is a digital clock and weather display built on the ESP8266 microcontroller. It also utilizes the MD_Parola library for LED Matrix Display and a specific version of the NTPClient library (included in this repository) for getting the current time from an NTP server. The device displays the current time, date, and temperature information.

## Getting Started

These instructions will help you get your Wemos Weather Clock up and running. Before you get started, ensure you have the following:

- An ESP8266 microcontroller.
- A compatible WiFi network.
- An API key from [WeatherAPI](https://www.weatherapi.com/).

## Features

- Displays time, date, and temperature information.
- WiFi-enabled, for syncing the current time and getting weather data.
- Configurable display switch intervals.
- Adjustable display intensity.

## Dependencies

This project depends on the following Arduino libraries:

- ESP8266WiFi.h
- ESP8266HTTPClient.h
- ArduinoJson.h
- MD_Parola.h
- MD_MAX72xx.h
- NTPClient.h (a specific version is included in the project repository)
- WiFiUdp.h

You can install these libraries through the Arduino Library Manager. For ArduinoJson, make sure to install version 6.x or later. For the NTPClient, use the version provided in the project repository as it includes the `getFormattedDate` function, which is not available in the library manager version.

## Setup Instructions

1. **Install the dependencies.** Open the Arduino IDE and install the required libraries using the Library Manager. For the NTPClient library, use the version provided in the project repository.

2. **Set up the hardware.** Connect the ESP8266 and LED matrix as defined in the pin configuration.

3. **Register for a WeatherAPI key.** Go to [WeatherAPI](https://www.weatherapi.com/) and register for a free API key.

4. **Configure the project.** Open the source code in the Arduino IDE and configure the following parameters:

    - `ssid` and `password`: Enter your WiFi network credentials here.
    - `WEATHER_API_URL`: Replace the API key and the city name with your WeatherAPI key and your desired city, respectively.
    - `UTC_OFFSET_IN_SECONDS`: Replace this with the UTC offset (in seconds) for your location.
    
5. **Upload the code.** Connect your ESP8266 to your computer and upload the code.

## Usage

Once set up, the Wemos Weather Clock will begin displaying the time, date, and temperature information for the configured city. The display will switch between these different pieces of information according to the set intervals.

## Troubleshooting

- If you encounter issues with WiFi connection, ensure your network credentials are correct and your ESP8266 is within range of the router.
- If the weather information is not displayed correctly, ensure your WeatherAPI key is valid and you have correctly entered your city in the API URL.

## Code Structure

- `setup()`: Initializes the WiFi connection and begins the NTP client for time synchronization.
- `loop()`: Main function that handles the switching of the display between time, date, and weather information. It also ensures the weather information is updated periodically.
- `updateWeather()`: Sends a request to the WeatherAPI to fetch the latest weather information.
- `displayWeather()`, `displayDate()`, `updateTime()`: These functions handle the displaying of the weather, date, and time information respectively.

## Contributors

- [snpr](https://github.com/SNPR)
