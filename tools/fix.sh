#!/bin/bash
echo "Reconfigurando libcurl y path..."
sudo rm -rf /usr/local/lib/libcurl.so.4
sudo ln -s /usr/lib/x86_64-linux-gnu/libcurl.so.4.3.0 /usr/local/lib/libcurl.so.4
echo "#!/usr/bin/latino" >> .temp.lat
sleep 0.5s
sed -i -e 's/\r$//' .temp.lat
rm .temp.lat
echo "Path y libcurl reconfigurados"
exit
