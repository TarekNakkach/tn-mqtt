# Description
MQTT daemon to sub/pub to specifics topics

# Dependency
libjson-c-dev <br/>
libmosquitto-dev

# Build
mkdir build <br/>
cd build <br/>
cmake .. <br/>
sudo make install

# Run
Usage: tn-mqtt [ARGS] <br/>

Args: <br/>
 -h Print this help <br/>
 -d Run as daemon <br/>
 -c Json config file path : i.e config/config.json <br/>
 -l Syslog debug level : 0 to disable logs, 7 to enable all logs
