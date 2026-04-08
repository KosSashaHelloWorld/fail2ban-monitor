# Fail2ban-monitor
### fail2ban-monitor-banned
TO BUILD:
```bash
git clone git@github.com:KosSashaHelloWorld/fail2ban-monitor.git
cd fail2ban-monitor
mkdir build
cd build
cmake ../
cmake --build .
```
TO INSTALL:
```bash
sudo pacman -S sqlite3 # you already has it in case of fail2ban still use it
cmake --install .
# intalled to /usr/local/bin/fail2ban-monitor-banned (look CMakeLists.txt)
watch -cn1 fail2ban-monitor-banned
```
if you want to see these ones that not banned (bantime expired):
```bash
fail2ban-monitor-banned -a
```
if you need to get a little more logs:
```bash
fail2ban-monitor-banned -v
```
Recommended:
```bash
watch -cn5 fail2ban-monitor-banned
```

P.S. Designed to use with tmux

![](screencast.gif)
