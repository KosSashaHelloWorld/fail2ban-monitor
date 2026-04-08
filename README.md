```bash
# TO BUILD
git clone git@github.com:KosSashaHelloWorld/fail2ban-monitor.git
cd fail2ban-monitor
mkdir build
cd build
cmake ../
cmake --build .

# TO INSTALL
sudo pacman -S sqlite3 # you already has it in case of fail2ban still use it
cmake --install .
# intalled to /usr/local/bin/fail2ban-monitor-banned (look CMakeLists.txt)
watch -cn1 fail2ban-monitor-banned
```

![](screencast.gif)
